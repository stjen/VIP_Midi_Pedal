#include "LEDController.h"



LEDController::LEDController() : leds(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800)  {

}

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
void LEDController::setup() {
	leds.begin();  // Call this to start up the LED strip.
	clearLeds();
	leds.show();   // ...but the LEDs don't actually update until you call this.
}

void LEDController::setButton(int num, LEDRGBCOLORS color) {
	if (num < LED_COUNT) {
		buttons[6-1-num] = color;
	}
	// Update leds
	clearLeds();
	for (int i = 0; i < LED_COUNT; ++i) {
		leds.setPixelColor(i, buttons[i]);
	}
	leds.show();
}

void LEDController::clearLeds() {

	// Clearleds
	for (int i = 0; i < LED_COUNT; i++) {
		leds.setPixelColor(i, 0);
	}

}
