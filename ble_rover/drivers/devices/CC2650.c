/*
 * CC2650.c
 *
 *  Created on: Dec 5, 2017
 *      Author: bigbywolf
 */

#include <drivers/devices/CC2650.h>
#include "../peripherals/uart1.h"
#include "npi.h"


#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

CC2650_state cc_state = CC2650_IDLE;

/* Helper functions */
void set_mrdy(void){
    GPIOPinWrite(MRDY_PORT, MRDY_PIN, MRDY_PIN);
}

void clear_mrdy(void){
    GPIOPinWrite(MRDY_PORT, MRDY_PIN, 0);
}

void tx_finished(){
    set_mrdy(); 
}

void CC2650_init(void){

    // Enable the GPIO port 
    ROM_SysCtlPeripheralEnable(NRESET_PERIPH);
    ROM_SysCtlPeripheralEnable(MRDY_PERIPH);
    ROM_SysCtlPeripheralEnable(SRDY_PERIPH);

    // set as output type
    ROM_GPIOPinTypeGPIOOutput(MRDY_PORT, MRDY_PIN);
    ROM_GPIOPinTypeGPIOOutput(NRESET_PORT, NRESET_PIN);

    // set reset high
    GPIOPinWrite(MRDY_PORT, MRDY_PIN, MRDY_PIN);
    GPIOPinWrite(NRESET_PORT, NRESET_PIN, NRESET_PIN); 

    // set as input
    GPIOPinTypeGPIOInput(SRDY_PORT, SRDY_PIN);

    GPIOPadConfigSet(SRDY_PORT, SRDY_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    // set interrupt to trigger on both rising and falling
    GPIOIntTypeSet(SRDY_PORT, SRDY_PIN, GPIO_BOTH_EDGES);

    GPIOIntRegister(SRDY_PORT, srdy_int_hanlder);

    // Enable the pin interrupts.
    GPIOIntEnable(SRDY_PORT, SRDY_PIN);

    uart1_init();
    uart1_set_tx_cb_func(tx_finished);

//    GPIOPinWrite(NRESET_PORT, NRESET_PIN, 0);
//    SysCtlDelay(SysCtlClockGet());
//    GPIOPinWrite(NRESET_PORT, NRESET_PIN, NRESET_PIN);
}

volatile bool value = false;
volatile uint32_t srdy_pin_state = 0; 
volatile bool cc_ready_to_receive = false; 

void srdy_int_hanlder(void){
    uint32_t status=0;

	status = GPIOIntStatus(SRDY_PORT, true);
	GPIOIntClear(SRDY_PORT,status);

	if( (status & SRDY_PIN) == SRDY_PIN){

		value = !value;

        // get pin state to know if done or starting
        srdy_pin_state = GPIOPinRead(SRDY_PORT, SRDY_PIN); 

		if(value){
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}
		else{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
		}

        // alert that cc2650 is ready to receive
        if(cc_state == CC2650_RECEIVING && srdy_pin_state == 0){
            cc_ready_to_receive = true; 
            /* uart1_send(data, count); */
        }

        // alert that cc2650 is done receiving
        if(cc_state == CC2650_RECEIVING && srdy_pin_state > 0){
            cc_ready_to_receive = false; 
            cc_state = CC2650_IDLE; 
        }

        // alert that cc2650 is transmitting a packet
        if(cc_state == CC2650_IDLE && srdy_pin_state == 0){
            // starting to receive 
            cc_state = CC2650_TRANSMITTING;
            clear_mrdy(); 
        }

        if(cc_state == CC2650_TRANSMITTING && srdy_pin_state > 0){
            // callback function for message received
            // todo
            
            cc_state = CC2650_IDLE;
            set_mrdy(); 
        }
	}

	/* if( (status & GPIO_INT_PIN_5) == GPIO_INT_PIN_5){ */
	  //Then there was a pin5 interrupt
	/* } */
}


uint8_t cc_send_msg_blocking(uint8_t *data, uint32_t count){

//    cc_state = CC2650_RECEIVING;
//    clear_mrdy();
//
//    while(!cc_ready_to_receive){
//        // add timeout
//        // if timeout set state back to idle
//        // cc_state = CC2650_IDLE;
//    }

    // data is being sent
//    uart1_send(data, count);
    
//    while(cc_state != CC2650_IDLE){
//        // add timeout
//        // if timeout set state back to idle
//        // cc_state = CC2650_IDLE;
//    }

//            set_mrdy();
    return 1; 
}


uint32_t CC2650_get_version(void){
    uint8_t NPI_GetVersion[] =  {SOF,0x00,0x00,0x35,0x03,0x36}; 

    cc_send_msg_blocking(NPI_GetVersion, 6);

    return 1; 
}
