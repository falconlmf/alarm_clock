
#include <Ticker.h>
#include "alarm.h"

typedef enum {
    ALARM_INIT = 0,
    ALARM_RING,
    ALARM_END
} ALARM_TYPEDEF;

static uint8_t alarm_enble = 1;
static uint8_t alarm_ring = 0;
static uint8_t alarm_stage = ALARM_INIT;
static String alarmTimeStr = "";

Ticker ticker_alarm_timeout;
void setRing (uint8_t ring);

uint8_t Alarm_en(void)
{
    return alarm_enble;
}

String Alarm_time(void)
{
    return alarmTimeStr;
}

void Alarm_set(String t)
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
            ticker_alarm_timeout.attach(300, setRing);
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