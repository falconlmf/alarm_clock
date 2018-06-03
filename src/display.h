#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "global.h"

extern const uint16_t c_white;
extern const uint16_t c_grey;
extern const uint16_t c_red;
extern Ticker ticker_display;

extern void displayInit(void);
extern void displayReset(void);
extern void displayAddObj(uint8_t x, uint8_t y, uint16_t c, String str);
extern void displayUpdate();

#endif