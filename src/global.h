#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <Arduino.h>
#include <Ticker.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <NtpClientLib.h>

typedef struct{
    uint8_t x;
    uint8_t y;
} XY;

typedef struct {
    uint8_t x; uint8_t y; uint16_t c; String str;
} TYPE_LOGO;

extern NTPSyncEvent_t ntpEvent; // Last triggered event


#endif