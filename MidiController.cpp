/*
 * MidiController.cpp
 *
 *  Created on: Nov 28, 2017
 *      Author: stefan
 */

#include "MidiController.h"
#include "common.h"
#include "Arduino.h"

/*
 * Midi cable wire colours:
 * Yellow: 4 (+5V)
 * Orange: 2 (GND)
 * White: 5 (TX)
 * Red: 1 (Unused)
 * Grey: 3 (Unused)
 * */

MidiController::MidiController() : midi(12, 9) /*rx,tx*/ {

	midi.begin(31250); // Midi is serial at baud 31250

	// Attempt to toggle reverb
    //Effect on/off
	const char manualToggle[] = {0x90, 0x0C, 0x7F, 0x90, 0x0C, 0x00};
	const char reverbToggle[] = {0x90, 0x04, 0x7F, 0x90, 0x04, 0x00};
	Serial.println("Toggling reverb");
	midi.write(manualToggle, 6);
	midi.write(reverbToggle, 6);
	midi.write(manualToggle, 6);
}

