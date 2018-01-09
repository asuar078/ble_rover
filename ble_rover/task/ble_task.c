/*
 * ble_task.c
 *
 *  Created on: Dec 19, 2017
 *      Author: bigbywolf
 */

#include <task/ble_task.h>

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "drivers/rgb.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "../drivers/peripherals/spi.h"

SemaphoreHandle_t spi_sp = NULL;

//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define BLETASKSTACKSIZE        128         // Stack size in words

void
PortEIntHandler(void)
{
    uint32_t status=0;

    status = GPIOIntStatus(GPIO_PORTE_BASE,true);
    GPIOIntClear(GPIO_PORTE_BASE,status);

    if( (status & GPIO_INT_PIN_4) == GPIO_INT_PIN_4){

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0);
    }


}

void spi_complete(void){

}

extern xSemaphoreHandle g_pUARTSemaphore;

static void ble_Task(void *pvParameters){

}

uint32_t ble_task_init(void){

    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // Enable input on pin pf4 / sw1
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

    // Allow interrupt on falling edge for sw1
    GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);

    // Interrupt handler
    GPIOIntRegister(GPIO_PORTE_BASE, PortEIntHandler);

    // Enable interrupt
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_4);

    spi_init();

    spi_send("AT+BLESTARTADV=?", sizeof("AT+BLESTARTADV=?"));

    //spi_cb_func()
    while(SSIBusy(SSI2_BASE)){}

    //
    // Create task.
    //
    if(xTaskCreate(ble_Task, (const portCHAR *)"BLE", BLETASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_BLE_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
