#ifndef ALARM_H_
#define ALARM_H_

#include <Arduino.h>

struct TimeData {
    char ssid[20];
    char password[20];
    char ntpServer[20];
    int8_t zone;
    int8_t minutesZone;
    bool ntpOK;
    bool wifiOK;
    String str;
};

struct AlarmData {
    union {
        struct {
            bool enable;
            bool ring;
        };
        uint8_t status;
    };
    uint8_t stage;
    String str;
};


extern timeData time;
extern AlarmData alarm;

void TimeInit(void);
void AlarmInit(void);
String getAlarm(void);
void setAlarm(String t);
void Tone(uint8_t _pin, unsigned int frequency, unsigned long duration);
void Alarm_handler(void);
void New_time_handler(void);

#endif