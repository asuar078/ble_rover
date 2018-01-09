/*
 * CC2650.h
 *
 *  Created on: Dec 5, 2017
 *      Author: bigbywolf
 */

#ifndef DRIVERS_DEVICES_CC2650_H_
#define DRIVERS_DEVICES_CC2650_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.h"


#define NRESET_PERIPH SYSCTL_PERIPH_GPIOC
#define NRESET_PORT GPIO_PORTC_BASE
#define NRESET_PIN GPIO_PIN_6

#define MRDY_PERIPH SYSCTL_PERIPH_GPIOB
#define MRDY_PORT GPIO_PORTB_BASE
#define MRDY_PIN GPIO_PIN_5

#define SRDY_PERIPH SYSCTL_PERIPH_GPIOB
#define SRDY_PORT GPIO_PORTB_BASE
#define SRDY_PIN GPIO_PIN_2

typedef enum {
    CC2650_TRANSMITTING,
    CC2650_IDLE,
    CC2650_RECEIVING
} CC2650_state; 

void CC2650_init(void);

// Get status of connection, used in Lab 6
// Input:  none
// Output: status 0xAABBCCDD
// AA is GAPRole Status
// BB is Advertising Status
// CC is ATT Status
// DD is ATT method in progress
uint32_t CC2650_get_status(void);

// Get version of the SNP application running on the CC2650, used in Lab 6
// Input:  none
// Output: version
uint32_t CC2650_get_version(void);

// Add a service, used in Lab 6
// Inputs uuid is 0xFFF0, 0xFFF1, ...
// Output APOK if successful,
//        APFAIL if SNP failure
int CC2650_add_service(uint16_t uuid);

// Register a service, used in Lab 6
// Inputs none
// Output APOK if successful,
//        APFAIL if SNP failure
int CC2650_register_service(void);

// Add a read, write, or read/write characteristic, used in Lab 6
//        for notify properties, call AP_AddNotifyCharacteristic
// Inputs uuid is 0xFFF0, 0xFFF1, ...
//        thesize is the number of bytes in the user data 1,2,4, or 8
//        pt is a pointer to the user data, stored little endian
//        permission is GATT Permission, 0=none,1=read,2=write, 3=Read+write
//        properties is GATT Properties, 2=read,8=write,0x0A=read+write
//        name is a null-terminated string, maximum length of name is 20 bytes
//        (*ReadFunc) called before it responses with data from internal structure
//        (*WriteFunc) called after it accepts data into internal structure
// Output APOK if successful,
//        APFAIL if name is empty, more than 8 characteristics, or if SNP failure
int CC2650_add_characteristic(uint16_t uuid, uint16_t thesize, void *pt, uint8_t permission,
uint8_t properties, char name[], void(*ReadFunc)(void), void(*WriteFunc)(void));

// Add a notify characteristic, used in Lab 6
//        for read, write, or read/write characteristic, call AP_AddCharacteristic
// Inputs uuid is 0xFFF0, 0xFFF1, ...
//        thesize is the number of bytes in the user data 1,2,4, or 8
//        pt is a pointer to the user data, stored little endian
//        name is a null-terminated string, maximum length of name is 20 bytes
//        (*CCCDfunc) called after it accepts , changing CCCDvalue
// Output APOK if successful,
//        APFAIL if name is empty, more than 4 notify characteristics, or if SNP failure
int CC2650_add_notify_characteristic(uint16_t uuid, uint16_t thesize, void *pt,   
char name[], void(*CCCDfunc)(void));

// Start advertisement, used in Lab 6
// Input:  none
// Output: APOK if successful,
//         APFAIL if notification not configured, or if SNP failure
int CC2650_start_advertisement(void);

void srdy_int_hanlder(void);




//------------AP_SendMessage------------
// sends a message to the Bluetooth module
// calculates/sends FCS at end
// FCS is the 8-bit EOR of all bytes except SOF and FCS itself
// 1) Send NPI package (it will calculate fcs)
// 2) Wait for entire message to be sent
// Input: pointer to NPI encoded array
// Output: APOK on success, APFAIL on timeout
/* int AP_SendMessage(uint8_t *pt){ */
/*   uint8_t fcs; uint32_t waitCount; uint8_t data; uint32_t size; */
/* // 1) Make MRDY=0 */
/*   ClearMRDY(); */
/* // 2) wait for SRDY to be low */
/*   waitCount = 0; */
/*   while(ReadSRDY()){ */
/*     waitCount++; */
/*     if(waitCount>APTIMEOUT){ */
/*       TimeOutErr++;  // no response error */
/*       return APFAIL; // timeout?? */
/*     } */
/*   } */
/* // 3) Send NPI package */
/*   size = AP_GetSize(pt); */
/*   fcs=0; */
/*   UART1_OutChar(SOF); pt++; */
/*   data=*pt; UART1_OutChar(data); fcs=fcs^data; pt++;   // LSB length */
/*   data=*pt; UART1_OutChar(data); fcs=fcs^data; pt++;   // MSB length */
/*   data=*pt; UART1_OutChar(data); fcs=fcs^data; pt++;   // CMD0 */
/*   data=*pt; UART1_OutChar(data); fcs=fcs^data; pt++;   // CMD1 */
/*   for(int i=0;i<size;i++){ */
/*     data=*pt; UART1_OutChar(data); fcs=fcs^data; pt++; // payload */
/*   } */
/*   UART1_OutChar(fcs);                                  // FCS */

/* // 4) Wait for entire message to be sent */
/*   UART1_FinishOutput(); */
/* // 5) Make MRDY=1 */
/*   SetMRDY();        //   MRDY=1 */
/* // 6) wait for SRDY to be high */
/*   waitCount = 0; */
/*   while(ReadSRDY()==0){ */
/*     waitCount++; */
/*     if(waitCount>APTIMEOUT){ */
/*       TimeOutErr++;  // no response error */
/*       return APFAIL; // timeout?? */
/*     } */
/*   } */
/*   return APOK; */
/* } */



#endif /* drivers_devices_cc2650_h_ */
