/*
    Copyright 2018 Stefan Jensen (stefan@upx.dk)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <Arduino.h>
#include <unistd.h>
#include "MidiController.h"
#include "LEDController.h"
#include "ButtonController.h"
#include <stdarg.h>
#include "AltSoftSerial.h"

#ifdef DEBUG
// A simple printf for arduino
void p(char *fmt, ...) {
	char buf[128]; // resulting string limited to 128 chars
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, 128, fmt, args);
	va_end(args);
	Serial.print(buf);
}
#endif /* DEBUG */


uint8_t buttonVals[6];

long buttonDebounce[6];
uint8_t debounceDelay = 5;
uint8_t lastButtonVals[6]; // For debouncing
uint8_t buttonState = 0;         // variable for reading the pushbutton status

AltSoftSerial midi;
LEDController led = LEDController();


#define COMMAND_MAX_LENGTH 20
#define SETTINGS_MAX_MODES 4


// The settings for a single mode in a pedal
struct SingleMode {
	uint8_t command[COMMAND_MAX_LENGTH];
	uint8_t commandLen; // Refers to the actual length of the command array
	LEDController::LEDRGBCOLORS color;
};


// The settings for a single pedal, containing a number of modes that can be switched through
struct PedalSettings {
	struct SingleMode modes[SETTINGS_MAX_MODES];
	uint8_t numModes; // Refers to the actual number of SingleModes
	uint8_t currentMode;
	uint8_t exclusive; // Should the activation of this pedal turn off all others
} pedals[6];




// Mapping for all the programs available in the Vypyr VIP 1
enum AMP_PROGRAM {
	prog_A1 = 0x00,
	prog_A2 = 0x01,
	prog_A3 = 0x02,
	prog_A4 = 0x03,
	prog_B1 = 0x04,
	prog_B2 = 0x05,
	prog_B3 = 0x06,
	prog_B4 = 0x07,
	prog_C1 = 0x08,
	prog_C2 = 0x09,
	prog_C3 = 0x0A,
	prog_C4 = 0x0B,
	prog_D1 = 0x0C,
	prog_D2 = 0x0D,
	prog_D3 = 0x0E,
	prog_D4 = 0x0F,
};



void bootLightShow() {
	// Light show when booting - running light
	for (int i = 0; i < 6; ++i) {
		led.setButton(i, LEDController::LEDRGB_RED);
		delay(50);
	}
	for (int i = 0; i < 6; ++i) {
		led.setButton(i, LEDController::LEDRGB_BLUE);
		delay(50);
	}
	for (int i = 0; i < 6; ++i) {
		led.setButton(i, LEDController::LEDRGB_YELLOW);
		delay(50);
	}

	// Light show - blink
	for (int j = 0; j < 6; ++j) {
		for (int i = 0; i < 6; ++i) {
			led.setButton(i, LEDController::LEDRGB_BLUE);
		}
		delay(100);
		for (int i = 0; i < 6; ++i) {
			led.setButton(i, LEDController::LEDRGB_NONE);
		}
		delay(100);
	}



}

void setup(void) {

#ifdef DEBUG
	Serial.begin(115200);
#endif /* DEBUG */

	midi.begin(31250); // Midi is serial at baud 31250

	led.setup();

	// Setup buttons
	pinMode(2, INPUT);
	pinMode(3, INPUT);
	pinMode(4, INPUT);
	pinMode(5, INPUT);
	pinMode(6, INPUT);
	pinMode(7, INPUT);

	/** SETUP THE PEDALS **/

	/* PEDAL 0 */
	// acoustic
	pedals[0].modes[0].color = LEDController::LEDRGB_GREEN;
	pedals[0].modes[0].command[0] = 0xC0; // Change program
	pedals[0].modes[0].command[1] = prog_C1; // Acoustic
	pedals[0].modes[0].commandLen = 2;

	pedals[0].numModes = 1;
	pedals[0].exclusive = 1;

	/* /PEDAL 0/ */

	/* PEDAL 1 */
	// clean
	pedals[1].modes[0].color = LEDController::LEDRGB_GREEN;
	pedals[1].modes[0].command[0] = 0xC0; // Change program
	pedals[1].modes[0].command[1] = prog_A1; // Clean
	pedals[1].modes[0].commandLen = 2;

	pedals[1].numModes = 1;
	pedals[1].exclusive = 1;
	/* /PEDAL 1/ */

	/* PEDAL 2 */
	// rock
	pedals[2].modes[0].color = LEDController::LEDRGB_GREEN;
	pedals[2].modes[0].command[0] = 0xC0; // Change program
	pedals[2].modes[0].command[1] = prog_B2; // Metal distortion
	pedals[2].modes[0].commandLen = 2;

	pedals[2].numModes = 1;
	pedals[2].exclusive = 1;
	/* /PEDAL 2/ */

	/* PEDAL 3 */
	// space
	pedals[3].modes[0].color = LEDController::LEDRGB_GREEN;
	pedals[3].modes[0].command[0] = 0xC0; // Change program
	pedals[3].modes[0].command[1] = prog_A2; // Acoustic
	pedals[3].modes[0].commandLen = 2;

	pedals[3].numModes = 1;
	pedals[3].exclusive = 1;

	/* /PEDAL 3/ */

	/* PEDAL 4 */
	// metal
	pedals[4].modes[0].color = LEDController::LEDRGB_GREEN;
	pedals[4].modes[0].command[0] = 0xC0; // Change program
	pedals[4].modes[0].command[1] = prog_B1; // Acoustic
	pedals[4].modes[0].commandLen = 2;

	pedals[4].numModes = 1;
	pedals[4].exclusive = 1;

	/* /PEDAL 4/ */

	/* PEDAL 5 */
	// free
	pedals[5].modes[0].color = LEDController::LEDRGB_GREEN;
	pedals[5].modes[0].command[0] = 0xC0; // Change program
	pedals[5].modes[0].command[1] = prog_B3; // Acoustic
	pedals[5].modes[0].commandLen = 2;

	pedals[5].numModes = 1;
	pedals[5].exclusive = 1;
	/* /PEDAL 5/ */

	// Get initial values of buttons
	for (int i = 0; i < 6; ++i) {
		buttonVals[i] = digitalRead(2 + i);
	}

	// Bootup light show
	bootLightShow();

}

// Sends the appropriate midi command for the specified pedal
// The pedals[pedal] must be updated prior to calling
void midiUpdate(uint8_t pedal) {

	uint8_t mode = pedals[pedal].currentMode;

	// Iterate over the settings array and send it
	for (int i = 0; i < pedals[pedal].modes[mode].commandLen; ++i) {
		midi.write((uint8_t) pedals[pedal].modes[mode].command[i]);
	}

}

void loop(void) {

	for (int i = 0; i < 6; ++i) {
		uint8_t newButtonVal = digitalRead(2 + i);
		// Debouncing timer
		if (newButtonVal != lastButtonVals[i]) {
			buttonDebounce[i] = millis(); // Set the time at which the button last changed
			lastButtonVals[i] = newButtonVal;
		}

		// Update the button if debounce has expired
		if ((millis() - buttonDebounce[i]) > debounceDelay
				&& newButtonVal != buttonVals[i]) {
			// Switch to next mode
			pedals[i].currentMode++;
			if (pedals[i].currentMode > pedals[i].numModes - 1)
				pedals[i].currentMode = 0;

			// Check if the pedal is EXCLUSIVE, in which case clear all leds
			if (pedals[i].exclusive == 1) {
				// Clear all other leds
				for (int j = 0; j < 6; ++j) {
					led.setButton(j, led.LEDRGB_NONE);

					if (i != j)
						pedals[i].currentMode = 0;
				}
			}
#ifdef DEBUG
			p("Changing btn %d to %d with col %06X\n", i + 2,
					pedals[i].currentMode,
					pedals[i].modes[pedals[i].currentMode].color);
#endif /* DEBUG */
			// Update LED for the button
			led.setButton(i, pedals[i].modes[pedals[i].currentMode].color);

			// Update buttonval
			buttonVals[i] = newButtonVal;

			// Send the appropriate midi command for the pedal
			midiUpdate(i);

		}

	}
}

