/*
 * spi.h
 *
 *  Created on: Aug 25, 2016
 *      Author: asuarez
 */

#ifndef DRIVERS_COMPONENTS_SPI_H_
#define DRIVERS_COMPONENTS_SPI_H_

#include <stdint.h>


void spi_init(void);

void ssi2_int_handler(void);

uint8_t spi_send(const uint8_t *buf, uint32_t len);

uint8_t spi_get(uint8_t *buf, uint32_t len);

void spi_cb_func(void (*funptr)());

/*
 * Transfer data SSI internal FIFO buffer
 * Buffer allows up to eight 16-bit values to be stored
 * independly.
 */
void spi_transfer(uint32_t *data_tx, uint32_t *data_rx, uint32_t num_of_packets);

#endif /* DRIVERS_COMPONENTS_SPI_H_ */
