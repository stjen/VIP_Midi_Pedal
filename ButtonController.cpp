/*
 * ButtonController.c
 *
 *  Created on: Dec 17, 2017
 *      Author: stefan
 */

#include "ButtonController.h"
#include "Arduino.h"

uint8_t readButtons() {
	uint8_t buttons = 0;
	/* Trigger a parallel Load to latch the state of the data lines,
	 */
	digitalWrite(PIN_CE, HIGH);
	digitalWrite(PIN_SH, LOW);
	delayMicroseconds(1);
	digitalWrite(PIN_SH, HIGH);
	digitalWrite(PIN_CE, LOW);


	long bt = digitalRead(PIN_QH);

	//  loop through the shift register bits
	for (uint8_t i = 0; i < 8; i++) {
		buttons |= (bt << ((8 - 1) - i));

		// Clock pulse
		digitalWrite(PIN_CLK, HIGH);
		delayMicroseconds(1);
		digitalWrite(PIN_CLK, LOW);
	}

	return (buttons);
}

void buttonInit() {
	// Setup buttons
	pinMode(PIN_CLK, OUTPUT);
	pinMode(PIN_SH, OUTPUT);
	pinMode(PIN_CE, OUTPUT);
	pinMode(PIN_QH, INPUT);

    digitalWrite(PIN_CLK, LOW);
    digitalWrite(PIN_SH, HIGH);


}
