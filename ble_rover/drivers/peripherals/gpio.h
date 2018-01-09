/*
 * gpio.h
 *
 *  Created on: Dec 13, 2017
 *      Author: bigbywolf
 */

#ifndef DRIVERS_PERIPHERALS_GPIO_H_
#define DRIVERS_PERIPHERALS_GPIO_H_

#define NRESET_PORT GPIO_PORTC_BASE
#define NRESET_PIN GPIO_PIN_6

#define MRDY_PORT GPIO_PORTB_BASE
#define MRDY_PIN GPIO_PIN_5

#define SRDY_PORT GPIO_PORTB_BASE
#define SRDY_PIN GPIO_PIN_2

void gpio_init(void);



#endif /* DRIVERS_PERIPHERALS_GPIO_H_ */
