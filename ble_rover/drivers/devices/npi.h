/*
 * npi.h
 *
 *  Created on: Dec 12, 2017
 *      Author: bigbywolf
 */

#ifndef DRIVERS_DEVICES_NPI_H_
#define DRIVERS_DEVICES_NPI_H_

#include <stdint.h>

#define SOF 0xFE

#define SYNCHRONOUS_REQUEST 0x1
#define ASYNCHRONOUS_REQUEST 0x2
#define SYNCHRONOUS_RESPONSE 0x3

#define SUBSYSTEM 0x15

typedef struct {
    uint8_t sof;
    uint16_t len;
    uint8_t cmd0;
    uint8_t cmd1;
    uint8_t payload[256];
    uint8_t fcs;

} npi_frame;

typedef enum{
    OK,
    MISSING_SOF,
    INCOMPLETE_PACKET,
    INCORRECT_FCS,
    ERROR
} npi_error_t; 

npi_error_t npi_parse_msg(npi_frame *frame, uint8_t *data);

npi_error_t  npi_contruct_msg(uint8_t *buf, uint8_t cmd_0, uint8_t cmd_1, uint8_t *pay_load); 


#endif /* DRIVERS_DEVICES_NPI_H_ */
