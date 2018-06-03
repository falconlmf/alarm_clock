// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include "global.h"
#include "menu.h"
#include "button.h"
#include "clock.h"
#include "display.h"
#include <Arduino.h>
#include <SparkFunHTU21D.h>
#include <Wire.h>


TwoWire myWire;
HTU21D myHumidity;

void setup()
{
	Serial.begin(230400);
	Serial.println();
	// myWire.begin(D2, D1);
	// myHumidity.begin(myWire);

    AlarmInit();
    ButtonInit();
	displayInit();
    ClockInit();
}

void loop()
{

	button.tick();

	if (clock.wifiOK) {
		clock.wifiOK = false;
		NTP.begin(clock.ntpServer, clock.zone, true, clock.minutesZone);
		NTP.setInterval(63);
	}
    Clock_handler();
	menu();
}
