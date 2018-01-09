/*
 * uart1.c
 *
 *  Created on: Dec 7, 2017
 *      Author: bigbywolf
 */

#include <drivers/peripherals/uart1.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"

#include "utils/ringbuffer.h"

struct ring_buffer_t tx_buff;
struct ring_buffer_t rx_buff;
volatile char c; 

void (*done_tx_cb)(void);	// callback function pointer

void uart1_init(void){

    ring_buffer_init(&tx_buff); 
    ring_buffer_init(&rx_buff); 
    
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Enable UART1
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);

    UARTConfigSetExpClk(UART1_BASE, 16000000, 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
        UART_CONFIG_PAR_NONE));

    // enable fifo
    UARTFIFOEnable(UART1_BASE);

    // Set FIFO interrupt trigger at tx 1/8 & rx 4/8
    UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX1_8, UART_FIFO_RX4_8);

    // set tx interrupt to idle transmitter
    UARTTxIntModeSet(UART1_BASE, UART_TXINT_MODE_EOT);

    // set interrupt handler
    UARTIntRegister(UART1_BASE, uart1_int_handler);

    // enable uart interrupt
    IntEnable(INT_UART1);

    // enable rx, and rx timeout interrupts
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX);

    // Enable the UART operation.
    UARTEnable(UART1_BASE);

    IntMasterEnable();
}

void uart1_int_handler(void)
{
    uint32_t status;
    status = UARTIntStatus(UART1_BASE, true); //get interrupt status
    UARTIntClear(UART1_BASE, status); //clear the asserted interrupts

    if( (status & UART_INT_RX) == UART_INT_RX ){

        if(ring_buffer_is_full(&rx_buff)){
            UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);
            return;
        }

        while(  UARTCharsAvail(UART1_BASE) && !ring_buffer_is_full(&rx_buff) ){
            ring_buffer_queue(&rx_buff, (char)UARTCharGetNonBlocking(UART1_BASE));
        }
    }

    if( (status & UART_INT_RT) == UART_INT_RT){

        if(ring_buffer_is_full(&rx_buff)){
            UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);
            return;
        }

        while(  UARTCharsAvail(UART1_BASE) && !ring_buffer_is_full(&rx_buff) ){
            ring_buffer_queue(&rx_buff, (char)UARTCharGetNonBlocking(UART1_BASE));
        }
    }

    if( (status & UART_INT_TX) == UART_INT_TX){

        if(ring_buffer_is_empty(&tx_buff)){
            UARTIntDisable(UART1_BASE, UART_INT_TX);
//            (*done_tx_cb)();  // Go ahead and call back to the application
            return;
        }

        while( UARTSpaceAvail(UART1_BASE) && !ring_buffer_is_empty(&tx_buff) ){
            ring_buffer_dequeue(&tx_buff, &c);
            ROM_UARTCharPutNonBlocking(UART1_BASE, c);
        }

    }
}

void uart1_set_tx_cb_func(void (*funptr)())
{
	done_tx_cb = funptr;
}

uint8_t uart1_tx_free_space(void){
    return RING_BUFFER_SIZE - ring_buffer_num_items(&tx_buff);
}

uint8_t uart1_send(const char *buf, uint32_t count){

    if( uart1_tx_free_space() > count){

        UARTIntDisable(UART1_BASE, UART_INT_TX);

        ring_buffer_queue_arr(&tx_buff, buf, count);

        // send first byte to kickstart transmission
        ring_buffer_dequeue(&tx_buff, &c);
        ROM_UARTCharPutNonBlocking(UART1_BASE, c);

        UARTIntEnable(UART1_BASE, UART_INT_TX);

        return 1;
    }
    else{
        return 0;
    }

}

uint8_t uart1_rx_available(void){
    return ring_buffer_num_items(&rx_buff);
}

uint8_t uart1_get(char *buf){

    if( uart1_rx_available() > 0 ){

        UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);

        ring_buffer_dequeue(&rx_buff, buf);

        UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);

        return 1;
    }
    else{
        return 0;
    }

}

uint8_t uart1_get_arr(char *buf, uint8_t count){

    UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);

    if( ring_buffer_is_empty(&rx_buff)){

        UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
        return 0;
    }

    uint8_t char_returned = 0;

    char_returned = (uint8_t)ring_buffer_dequeue_arr(&rx_buff, buf, count);

    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);

    return char_returned;
}

uint8_t uart1_get_till(char *buf, char end_char, uint8_t max_len){

    UARTIntDisable(UART1_BASE, UART_INT_RX | UART_INT_RT);

    if( max_len > RING_BUFFER_SIZE){
        UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
        return 0;
    }

    if( ring_buffer_is_empty(&rx_buff)){

        UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
        return 0;
    }

    uint8_t cnt = 0;
    char *data_ptr = buf; 

    while( (cnt < max_len) && ring_buffer_dequeue(&rx_buff, data_ptr )){
        if(*data_ptr == end_char){
            break; 
        }

        cnt++;
        data_ptr++; 
    }

    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
    return 1; 
}
