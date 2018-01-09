/*
 * uart1.h
 *
 *  Created on: Dec 7, 2017
 *      Author: bigbywolf
 */

#ifndef DRIVERS_PERIPHERALS_UART1_H_
#define DRIVERS_PERIPHERALS_UART1_H_

#include <stdint.h>
#include <stdbool.h>

void uart1_init(void);

uint8_t uart1_tx_free_space(void);

uint8_t uart1_send(const char *buf, uint32_t count);

uint8_t uart1_rx_available(void);

uint8_t uart1_get(char *buf);

uint8_t uart1_get_arr(char *buf, uint8_t count);

void uart1_int_handler(void);

void uart1_set_tx_cb_func(void (*funptr)()); 

#endif /* DRIVERS_PERIPHERALS_UART1_H_ */
