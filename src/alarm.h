#ifndef ALARM_H_
#define ALARM_H_

#include <Arduino.h>

uint8_t Alarm_en(void);
String Alarm_time(void);
void Alarm_set(String t);
void Tone(uint8_t _pin, unsigned int frequency, unsigned long duration);
void Alarm_handler(void);

#endif