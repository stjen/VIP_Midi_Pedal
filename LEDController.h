/*
 * LEDDriver.h
 *
 *  Created on: Dec 3, 2017
 *      Author: stefan
 */

#include <Adafruit_NeoPixel.h>

#ifndef LEDCONTROLLER_H_
#define LEDCONTROLLER_H_

#define PIN 8
#define LED_COUNT 6

class LEDController {

public:
	enum LEDRGBCOLORS {
		LEDRGB_GREEN  = 0x00FF00,
		LEDRGB_RED 	  = 0xFF0000,
		LEDRGB_BLUE   = 0x0000FF,
		LEDRGB_YELLOW = 0xFFFF00,
		LEDRGB_NONE	  = 0x000000
	};
	void clearLeds();

	void setup();
	void setButton(int button, LEDRGBCOLORS color);
	LEDController();
private:
	Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
	LEDRGBCOLORS buttons[6];
};


#endif /* LEDCONTROLLER_H_ */
