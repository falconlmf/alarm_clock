
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <TimeLib.h>
#include "global.h"
#include "display.h"
#include "clock.h"

typedef enum {
    ALARM_INIT = 0,
    ALARM_RING,
    ALARM_END
} ALARM_TYPEDEF;

struct ClockData clock;
struct AlarmData alarm;
Ticker ticker_alarm_timeout;
bool syncEventTriggered = false;
NTPSyncEvent_t ntpEvent; // Last triggered event

void onSTAConnected (WiFiEventStationModeConnected ipInfo);
void onSTAGotIP (WiFiEventStationModeGotIP ipInfo);
void onSTADisconnected (WiFiEventStationModeDisconnected event_info);
String getTime(void);
void setTime(String str);
void setRing (uint8_t ring);
void processSyncEvent (NTPSyncEvent_t ntpEvent);

void ClockInit(void)
{
    clock.zone = 8;
	clock.minutesZone = 0;
	clock.ntpOK = false;
	clock.wifiOK = false;
    clock.str = "12:34";
    clock.ssid = "TCAP";
    clock.password = "pp00000000000";
    clock.ntpServer = "stdtime.gov.hk";
    
	WiFi.mode(WIFI_STA);
	WiFi.begin(clock.ssid.c_str(), clock.password.c_str());

    	NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
		ntpEvent = event;
		syncEventTriggered = true;
	});

	static WiFiEventHandler e1, e2, e3;
	e1 = WiFi.onStationModeGotIP(onSTAGotIP); // As soon WiFi is connected, start NTP Client
	e2 = WiFi.onStationModeDisconnected(onSTADisconnected);
	e3 = WiFi.onStationModeConnected(onSTAConnected);
}

void onSTAConnected (WiFiEventStationModeConnected ipInfo) {
    Serial.printf ("Connected to %s\r\n", ipInfo.ssid.c_str ());
}

void onSTAGotIP (WiFiEventStationModeGotIP ipInfo) {
    Serial.printf ("Got IP: %s\r\n", ipInfo.ip.toString ().c_str ());
    Serial.printf ("Connected: %s\r\n", WiFi.status () == WL_CONNECTED ? "yes" : "no");
    clock.wifiOK = true;
}

void onSTADisconnected (WiFiEventStationModeDisconnected event_info) {
    Serial.printf ("Disconnected from SSID: %s\n", event_info.ssid.c_str ());
    Serial.printf ("Reason: %d\n", event_info.reason);
    NTP.stop(); // NTP sync can be disabled to avoid sync errors
}

String getTime(void)
{   
    return clock.str;
}

void setTime(String str) 
{
    clock.str = str;
}

void AlarmInit(void)
{
    alarm.str = "\0";
    alarm.stage = ALARM_INIT;
}

String getAlarm(void)
{
    return alarm.enable ? alarm.str : "\0";
}

void setAlarm(String t)
{
    alarm.str = t;
    Serial.println("Alarm set: " + String(alarm.str));
}

void setRing (void)
{
    alarm.ring = 1;
    Serial.println("setRing: 1");
}

void Tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
    pinMode(_pin, OUTPUT);
    analogWriteFreq(frequency);
    analogWrite(_pin, 500);
    delay(duration);
    analogWrite(_pin, 0);
}

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

    if (getTime() == getAlarm()) {
        Alarm_handler();
    }
}


void processSyncEvent (NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {
        Serial.print ("Time Sync error: ");
        if (ntpEvent == noResponse)
            Serial.println ("NTP server not reachable");
        else if (ntpEvent == invalidAddress)
            Serial.println ("Invalid NTP server address");
    } else {
        if (clock.ntpOK == false) {
            clock.ntpOK = true;
            NTP.setDayLight(false);
            New_time_handler();
            setAlarm(getTime().substring(0, 4) + String(getTime().substring(4, 5).toInt() + 1));
        }
        Serial.print ("Got NTP time: ");
        Serial.print (NTP.getTimeDateString (NTP.getLastNTPSync ()));    
        Serial.print (" Uptime: ");
        Serial.print (NTP.getUptimeString ()); Serial.print (" since ");
        Serial.println (NTP.getTimeDateString (NTP.getFirstSync ()).c_str ());

    }
}

void Alarm_handler(void)
{
    switch(alarm.status) {
        case ALARM_INIT:
            ticker_alarm_timeout.attach(30, setRing);
            alarm.status = ALARM_RING;
            Serial.println("ALARM_INIT");
            Serial.println("ALARM_RING");
        break;
        case ALARM_RING:
            if (alarm.ring == true) {
                return;
            }
            Serial.println("ALARM_END");            
            alarm.status = ALARM_END;
        break;
        case ALARM_END:        
        break; 
    }
}

void Clock_handler(void)
{
    if (syncEventTriggered) {
        processSyncEvent (ntpEvent);
        syncEventTriggered = false;
    }

    if (NTP.getTimeStr().substring(0, 5) != getTime()) {
        New_time_handler();
    }
}


