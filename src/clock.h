#ifndef ALARM_H_
#define ALARM_H_

#include "global.h"

struct ClockData {
    int8_t zone;
    int8_t minutesZone;
    bool ntpOK;
    bool wifiOK;
    String ssid;
    String password;
    String ntpServer;
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


extern struct ClockData clock;
extern struct AlarmData alarm;

void ClockInit(void);
void AlarmInit(void);
String getTime(void);
void setTime(String str);
String getAlarm(void);
void setAlarm(String t);
void Tone(uint8_t _pin, unsigned int frequency, unsigned long duration);
void New_time_handler(void);
void Alarm_handler(void);
void Clock_handler(void);

#endif