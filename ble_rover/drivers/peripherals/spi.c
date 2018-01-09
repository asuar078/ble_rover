/*
 * spi.c
 *
 *  Created on: Aug 25, 2016
 *      Author: asuarez
 */

#include "spi.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"

#include "utils/ringbuffer.h"

struct ring_buffer_t tx_buff;
struct ring_buffer_t rx_buff;
volatile char c; 

void (*cb_func)(void);   // callback function pointer for finished tx

void (*cb_func)(void);   // callback function pointer for finished rx

bool waiting_for_ok = false;

void spi_tx_funcb(void (*funptr)())
{
    cb_func = funptr;
}

void spi_rx_funcb(void (*funptr)())
{
    cb_func = funptr;
}

void spi_init(void)
{
    ring_buffer_init(&tx_buff); 
    ring_buffer_init(&rx_buff); 

    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5);

    /* enable ssi function on port B, enable port B */
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    /* configure pins for ssi mode */
    ROM_GPIOPinConfigure(GPIO_PB4_SSI2CLK);
    //ROM_GPIOPinConfigure(GPIO_PB5_SSI2FSS);
    ROM_GPIOPinConfigure(GPIO_PB6_SSI2RX);
    ROM_GPIOPinConfigure(GPIO_PB7_SSI2TX);

    /* configure pins for ssi mode */
    ROM_GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7);

	/* ssi settings */
	ROM_SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 4000000, 8);

    SSIIntEnable(SSI2_BASE, SSI_RXFF|SSI_RXTO);

    // enable uart interrupt
    IntEnable(INT_SSI2);

    SSIIntRegister(SSI2_BASE, ssi2_int_handler);

    IntMasterEnable();

	/* enable function */
	ROM_SSIEnable(SSI2_BASE);
}


uint8_t spi_send(const uint8_t *buf, uint32_t len)
{
    SSIIntDisable(SSI2_BASE, SSI_TXFF);

    ring_buffer_queue_arr(&tx_buff, (char *)buf, len);

    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0);

    // send first byte to kickstart transmission
    ring_buffer_dequeue(&tx_buff, (char *)&c);
    SSIDataPutNonBlocking(SSI2_BASE, c); 

    SSIIntEnable(SSI2_BASE, SSI_TXFF);

    return 1;
}

uint8_t spi_wait_for_ok(void){

    SSIIntDisable(SSI2_BASE, SSI_TXFF);

    waiting_for_ok = true;

    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0);

    SSIDataPutNonBlocking(SSI2_BASE, '0');

    SSIIntEnable(SSI2_BASE, SSI_TXFF);

    return 1;
}

uint8_t spi_get(uint8_t *buf, uint32_t len)
{

    SSIIntDisable(SSI2_BASE, SSI_RXFF|SSI_RXTO);

    uint8_t chars_returned = 0;

    chars_returned = (uint8_t)ring_buffer_dequeue_arr(&rx_buff, (char *)buf, len);

    SSIIntEnable(SSI2_BASE, SSI_RXFF|SSI_RXTO);

    return chars_returned;
}

void ssi2_int_handler(void)
{
    uint32_t status;
    status = SSIIntStatus(SSI2_BASE, true); //get interrupt status
    SSIIntClear(SSI2_BASE, status); //clear the asserted interrupts

    if( ( (status & SSI_RXFF) == SSI_RXFF && (status & SSI_RXTO) == SSI_RXTO))
    {
        if(ring_buffer_is_full(&rx_buff))
        {
            SSIIntDisable(SSI2_BASE, SSI_RXFF|SSI_RXTO);
            return;
        }

        while( SSIDataGetNonBlocking(SSI2_BASE, &c) && !ring_buffer_is_full(&rx_buff) )
        {
            ring_buffer_queue(&rx_buff, (char)c);
        }
    }

    // when half empty interrupt occurs send at most 4 bytes to fifo
    if( (status & SSI_TXFF) == SSI_TXFF)
    {
        if(waiting_for_ok)
        {
            SSIDataPutNonBlocking(SSI2_BASE, '0');
        }
        else
        {
            int i = 0;
            for(i = 0; i < 4; i++)
            {
                if(ring_buffer_is_empty(&tx_buff))
                {
                    break;
                }
                ring_buffer_dequeue(&tx_buff, (char *)&c);
                SSIDataPutNonBlocking(SSI2_BASE, c);
            }
        }

        if(!SSIBusy(SSI2_BASE))
        {
            SSIIntDisable(SSI2_BASE, SSI_TXFF);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5);
            (*cb_func)();  // Go ahead and call back to the application
        }

    }

    // interrupt does not appear to work
//    if( (status & SSI_TXEOT) == SSI_TXEOT){
//        SSIIntDisable(SSI2_BASE, SSI_TXFF|SSI_TXEOT);
//        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5);
////      (*done_tx_cb)();  // Go ahead and call back to the application
//    }
}



void spi_transfer(uint32_t *data_tx, uint32_t *data_rx, uint32_t num_of_packets)
{
    uint32_t index;		/* Used to count the number of bytes Tx or Rx */

	/* For loop used to send bytes of data */
	for(index = 0; index < num_of_packets; index++)
	{
		/* Send the data using the "blocking" put function */
		SSIDataPut(SSI2_BASE, data_tx[index]);

		while(SSIBusy(SSI2_BASE)){}

		/* Receive the data using the "blocking" get function */
		SSIDataGet(SSI2_BASE, &data_rx[index]);

	}
}
