// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include "global.h"
#include <Arduino.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <NtpClientLib.h>
#include <TimeLib.h>
#include <Wire.h>
#include <SparkFunHTU21D.h>
#include <OneButton.h>
#include "display.h"
#include "button.h"
#include "alarm.h"
#include "time.h"

char ssid[] = "TCAP";
char pass[] = "pp00000000000";
const char* ntpServerName = "stdtime.gov.hk";
byte packetBuffer[48]; // NTP time stamp is in the first 48 bytes of the message
int8_t timeZone;
int8_t minutesTimeZone;
bool ntpFirstGet;
bool wifiFirstConnected;

OneButton button(D3, true);

TwoWire myWire;
HTU21D myHumidity;
Ticker ticker_display;


void New_time_handler(void);

void onSTAConnected (WiFiEventStationModeConnected ipInfo) {
    Serial.printf ("Connected to %s\r\n", ipInfo.ssid.c_str ());
}

void onSTAGotIP (WiFiEventStationModeGotIP ipInfo) {
    Serial.printf ("Got IP: %s\r\n", ipInfo.ip.toString ().c_str ());
    Serial.printf ("Connected: %s\r\n", WiFi.status () == WL_CONNECTED ? "yes" : "no");
    wifiFirstConnected = true;
}

void onSTADisconnected (WiFiEventStationModeDisconnected event_info) {
    Serial.printf ("Disconnected from SSID: %s\n", event_info.ssid.c_str ());
    Serial.printf ("Reason: %d\n", event_info.reason);
    NTP.stop(); // NTP sync can be disabled to avoid sync errors
}

void processSyncEvent (NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {
        Serial.print ("Time Sync error: ");
        if (ntpEvent == noResponse)
            Serial.println ("NTP server not reachable");
        else if (ntpEvent == invalidAddress)
            Serial.println ("Invalid NTP server address");
    } else {
        if (ntpFirstGet == false) {
            ntpFirstGet = true;
            NTP.setDayLight(false);
            New_time_handler();
            Alarm_set(getTime().substring(0, 4) + String(getTime().substring(4, 5).toInt() + 1));
        }
        Serial.print ("Got NTP time: ");
        Serial.print (NTP.getTimeDateString (NTP.getLastNTPSync ()));    
        Serial.print (" Uptime: ");
        Serial.print (NTP.getUptimeString ()); Serial.print (" since ");
        Serial.println (NTP.getTimeDateString (NTP.getFirstSync ()).c_str ());

    }
}

boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event

void New_time_handler(void)
{
    static int i;

    // float humd = myHumidity.readHumidity();
    // float temp = myHumidity.readTemperature();
    // Serial.print("Time:");
    // Serial.print(millis());
    // Serial.print(" Temperature:");
    // Serial.print(temp, 1);
    // Serial.print("C");
    // Serial.print(" Humidity:");
    // Serial.print(humd, 1);
    // Serial.print("%");

    // Serial.println();

    Serial.print (i++); Serial.print (" ");
    Serial.print (NTP.getTimeStr ()); Serial.print (" "); Serial.print (NTP.getDateStr ()); Serial.print (" ");
    Serial.print ("WiFi is ");
    Serial.print (WiFi.isConnected () ? "connected" : "not connected"); Serial.print (". ");
    Serial.print ("Uptime: ");
    Serial.print (NTP.getUptimeString ()); Serial.print (" since ");
    Serial.println (NTP.getTimeDateString (NTP.getFirstSync ()).c_str ());

    setTime(NTP.getTimeStr().substring(0, 5));
    
    displayReset();
    displayAddObj(0, 0, c_grey, String(char(128)));
    displayAddObj(0, 0, c_red, String(char(129)));
    displayAddObj(10, 1, c_grey, getTime());

    if (Alarm_en() && getTime() == Alarm_time()) {
        Alarm_handler();
    }
}

void setup() {
    ntpFirstGet = false;
    timeZone = 8;
    minutesTimeZone = 0;
    ntpFirstGet = false;
    wifiFirstConnected = false;
    delay(400);

    ticker_display.attach_ms(20, displayUpdate);

    button.setDebounceTicks(20);
    button.setClickTicks(300);
    button.attachClick(ButtonClick);
    button.attachDoubleClick(ButtonDoubleClick);

    myWire.begin(D2, D1);
    myHumidity.begin(myWire);

    displayInit();

    Serial.begin (230400);
    Serial.println ();
    WiFi.mode (WIFI_STA);
    WiFi.begin (ssid, pass);

    NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
        ntpEvent = event;
        syncEventTriggered = true;
    });

    static WiFiEventHandler e1, e2, e3;
    e1 = WiFi.onStationModeGotIP (onSTAGotIP);// As soon WiFi is connected, start NTP Client
    e2 = WiFi.onStationModeDisconnected (onSTADisconnected);
    e3 = WiFi.onStationModeConnected (onSTAConnected);
}

void loop() {

    button.tick();

    if (wifiFirstConnected) {
        wifiFirstConnected = false;
        NTP.begin (ntpServerName, timeZone, true, minutesTimeZone);
        NTP.setInterval (63);
    }

    if (syncEventTriggered) {
        processSyncEvent (ntpEvent);
        syncEventTriggered = false;
    }

    if (NTP.getTimeStr().substring(0, 5) != getTime()) {
        New_time_handler();
    }
}
