/*
 * LEDDriver.h
 *
 *  Created on: Dec 3, 2017
 *      Author: stefan
 */
#include <stdint.h>


#ifndef BUTTONCONTROLLER_H_
#define BUTTONCONTROLLER_H_

/* Returns a bitmask of button status */
uint8_t readButtons();
void buttonInit();


#define PIN_SH 16 // Parallel load
#define PIN_CLK 5 // Clock
#define PIN_QH 4 // Output (Q7)
#define PIN_CE 0 // Clock enable






#endif /* BUTTONCONTROLLER_H_ */
