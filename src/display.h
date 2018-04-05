#ifndef DISPLAY_H_
#define DISPLAY_H_

extern const uint16_t c_white;
extern const uint16_t c_grey;
extern const uint16_t c_red;

void displayInit(void);
void displayReset(void);
void displayAddObj(uint8_t x, uint8_t y, uint16_t c, String str);
void displayUpdate();

#endif