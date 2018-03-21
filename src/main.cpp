// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <NtpClientLib.h>
#include <TimeLib.h>
#include "alarm.h"
#include "c.h"

char ssid[] = "TCAP";
char pass[] = "pp00000000000";
const char* ntpServerName = "stdtime.gov.hk";
byte packetBuffer[48]; // NTP time stamp is in the first 48 bytes of the message
String timeStr = "";

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, D4,
    NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
    NEO_GRB            + NEO_KHZ800);


int8_t timeZone = 8;
int8_t minutesTimeZone = 0;
bool ntpFirstGet = false;
bool wifiFirstConnected = false;

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
            timeStr = NTP.getTimeStr().substring(0, 5);
            Alarm_set(timeStr.substring(0, 4) + String(timeStr.substring(4, 5).toInt() + 1));
            New_time_handler();
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

const uint16_t c_white = matrix.Color(100, 100,100);
const uint16_t c_grey = matrix.Color(60, 40, 50);
const uint16_t c_red = matrix.Color(30, 0, 0);

void setup() {
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(100);
    matrix.setFont(&c);
    matrix.fillScreen(0);    

    matrix.setCursor(10, 01);   
    matrix.setTextColor(c_grey);     
    matrix.print("99:99");
    matrix.show();

    static WiFiEventHandler e1, e2, e3;

    Serial.begin (230400);
    Serial.println ();
    WiFi.mode (WIFI_STA);
    WiFi.begin (ssid, pass);

    NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
        ntpEvent = event;
        syncEventTriggered = true;
    });

    e1 = WiFi.onStationModeGotIP (onSTAGotIP);// As soon WiFi is connected, start NTP Client
    e2 = WiFi.onStationModeDisconnected (onSTADisconnected);
    e3 = WiFi.onStationModeConnected (onSTAConnected);

}

#define MATRIX_LOGO_X   0
#define MATRIX_LOGO_Y   0
#define MATRIX_LOGO_W   8
#define MATRIX_LOGO_H   8
#define TIME_CUR_X  10
#define TIME_CUR_Y  1

typedef struct{
    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
} M_GRAPHIC;

const M_GRAPHIC m_logo = {MATRIX_LOGO_X, MATRIX_LOGO_Y, MATRIX_LOGO_W, MATRIX_LOGO_H};
const M_GRAPHIC m_time = {TIME_CUR_X, TIME_CUR_Y, 0, 0};

void matrix_update_logo(void)
{
    matrix.setCursor(m_logo.x, m_logo.y);
    matrix.setTextColor(c_red);
    matrix.print(char(129));
    
    matrix.setCursor(m_logo.x, m_logo.y);
    matrix.setTextColor(c_grey);
    matrix.print(char(128));
}

void matrix_update_time(void)
{
    matrix.setCursor(m_time.x, m_time.y);
    matrix.setTextColor(c_grey);
    matrix.print(timeStr);
}

void New_time_handler(void)
{
    static int i;
    Serial.print (i++); Serial.print (" ");
    Serial.print (NTP.getTimeStr ()); Serial.print (" "); Serial.print (NTP.getDateStr ()); Serial.print (" ");
    Serial.print (NTP.isSummerTime () ? "Summer Time. " : "Winter Time. ");
    Serial.print ("WiFi is ");
    Serial.print (WiFi.isConnected () ? "connected" : "not connected"); Serial.print (". ");
    Serial.print ("Uptime: ");
    Serial.print (NTP.getUptimeString ()); Serial.print (" since ");
    Serial.println (NTP.getTimeDateString (NTP.getFirstSync ()).c_str ());

    timeStr = NTP.getTimeStr().substring(0, 5);

    if (Alarm_en() && (timeStr == Alarm_time())) {
        Alarm_handler();
    }
    matrix.fillScreen(0);
    matrix_update_time();
    matrix_update_logo();
    matrix.show();
}

void loop() {

    if (wifiFirstConnected) {
        wifiFirstConnected = false;
        NTP.begin (ntpServerName, timeZone, true, minutesTimeZone);
        NTP.setInterval (63);
    }

    if (syncEventTriggered) {
        processSyncEvent (ntpEvent);
        syncEventTriggered = false;
    }

    if (NTP.getTimeStr().substring(0, 5) != timeStr) {
        New_time_handler();
    }
}
