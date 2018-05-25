
#include <Ticker.h>
#include "clock.h"

typedef enum {
    ALARM_INIT = 0,
    ALARM_RING,
    ALARM_END
} ALARM_TYPEDEF;

struct TimeData time;
struct AlarmData alarm;
Ticker ticker_alarm_timeout;

String getTime(void);
void setTime(String str);
void setRing (uint8_t ring);

void TimeInit(void)
{
    time.zone = 8;
	time.minutesZone = 0;
	time.ntpOK = false;
	time.wifiOK = false;
    time.str = "12:34";
    time.ssid = "TCAP";
    time.password = "pp00000000000";
    time.ntpServer = "stdtime.gov.hk";
}

String getTime(void)
{   
    return time.str;
}

void setTime(String str) 
{
    time.str = str;
}

void AlarmInit(void)
{
    alarm.time = 0;
    alarm.stage = ALARM_INIT;
}

String getAlarm(void)
{
    return alarm_enable ? alarmTimeStr : 0;
}

void setAlarm(String t)
{
    alarmTimeStr = t;
    Serial.println("Alarm set: " + String(alarmTimeStr));
}

void setRing (void)
{
    alarm_ring = 1;
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

void Alarm_handler(void)
{
    switch(alarm_stage) {
        case ALARM_INIT:
            ticker_alarm_timeout.attach(30, setRing);
            alarm_stage = ALARM_RING;
            Serial.println("ALARM_INIT");
            Serial.println("ALARM_RING");
        break;
        case ALARM_RING:
            if (alarm_ring == true) {
                return;
            }
            Serial.println("ALARM_END");            
            alarm_stage = ALARM_END;
        break;
        case ALARM_END:        
        break; 
    }
}

boolean syncEventTriggered = false; // True if a time even has been triggered

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

void clock(void)
{
    if (syncEventTriggered) {
        processSyncEvent (ntpEvent);
        syncEventTriggered = false;
    }

    if (NTP.getTimeStr().substring(0, 5) != getTime()) {
        New_time_handler();
    }
}





void onSTAConnected (WiFiEventStationModeConnected ipInfo) {
    Serial.printf ("Connected to %s\r\n", ipInfo.ssid.c_str ());
}

void onSTAGotIP (WiFiEventStationModeGotIP ipInfo) {
    Serial.printf ("Got IP: %s\r\n", ipInfo.ip.toString ().c_str ());
    Serial.printf ("Connected: %s\r\n", WiFi.status () == WL_CONNECTED ? "yes" : "no");
    time.wifiOK = true;
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
        if (time.ntpOK == false) {
            time.ntpOK = true;
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