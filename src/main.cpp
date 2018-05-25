// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include "button.h"
#include "clock.h"
#include "display.h"
#include "global.h"
#include "time.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NtpClientLib.h>
#include <OneButton.h>
#include <SparkFunHTU21D.h>
#include <Ticker.h>
#include <TimeLib.h>
#include <Wire.h>


TwoWire myWire;
HTU21D myHumidity;

void setup()
{
    TimeInit();
    AlarmInit();
    ButtonInit();
	displayInit();

	myWire.begin(D2, D1);
	myHumidity.begin(myWire);

	Serial.begin(230400);
	Serial.println();
	WiFi.mode(WIFI_STA);
	WiFi.begin(time.ssid, time.password);

	NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
		ntpEvent = event;
		syncEventTriggered = true;
	});

	static WiFiEventHandler e1, e2, e3;
	e1 = WiFi.onStationModeGotIP(onSTAGotIP); // As soon WiFi is connected, start NTP Client
	e2 = WiFi.onStationModeDisconnected(onSTADisconnected);
	e3 = WiFi.onStationModeConnected(onSTAConnected);
}

void loop()
{

	button.tick();

	if (time.wifiOK) {
		time.wifiOK = false;
		NTP.begin(time.ntpServer, time.zone, true, time.minutesZone);
		NTP.setInterval(63);
	}

	menu();
}
