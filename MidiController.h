/*
 * MidiController.h
 *
 *  Created on: Nov 28, 2017
 *      Author: stefan
 */

#include <SoftwareSerial.h>

#ifndef MIDICONTROLLER_H_
#define MIDICONTROLLER_H_


class MidiController {
private:
	SoftwareSerial midi; // RX, TX

public:
	MidiController();
};

#endif /* MIDICONTROLLER_H_ */
