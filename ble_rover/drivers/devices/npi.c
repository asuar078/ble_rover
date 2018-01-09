#include "npi.h"

// returns the size of an NPI message
// Inputs:  pointer to NPI message
// Outputs: size of the message
// extracted little Endian from byte 1 and byte 2
uint16_t get_msg_size(uint8_t *pt)
{
  uint8_t msb,lsb = 0;

  lsb = (uint8_t)pt[1];
  msb = (uint8_t)pt[2];

  return (msb<<8)+lsb;
}

// helper function, add check byte to message
// assumes every byte in the message has been set except the FCS
// used the length field, assumes less than 256 bytes
// FCS = 8-bit EOR(all bytes except SOF and the FCS itself)
// Inputs: pointer to message
//         stores the FCS into message itself
// Outputs: none
uint8_t calculate_fcs(uint8_t *msg)
{
    uint8_t i = 0;
    uint8_t fcs = 0;
    uint8_t frame_check_length = 0;

    frame_check_length = get_msg_size(msg) + 4; // is equal to the message data length + 4 (2x length byte + 2x command byte)

    for (i = 1; i <= frame_check_length; i++) {
        fcs ^= msg[i];
    }

    return fcs;
}

npi_error_t npi_parse_msg(npi_frame *frame, uint8_t *data)
{
    uint8_t i = 0;
    if(data[0] != SOF){ return MISSING_SOF; }

    frame->sof = data[0];
    frame->len = get_msg_size(data);
    frame->cmd0 = data[3];
    frame->cmd1 = data[4];

    for (i = 0; i < frame->len; i++) {
        frame->payload[i] = data[i + 5]; 
    }


    uint8_t cal_fcs = calculate_fcs(data);

    if(cal_fcs != data[ frame->len + 5]) { return INCORRECT_FCS; }

    frame->fcs = cal_fcs; 

    return OK;
}

npi_error_t npi_contruct_msg(uint8_t *buf, uint8_t cmd_0, uint8_t cmd_1, uint8_t *pay_load)
{

    buf[0] = SOF;

    return OK;
}



