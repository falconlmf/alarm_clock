#ifndef BUTTON_H_
#define BUTTON_H_
#include <OneButton.h>

enum BUTTON_STATUS {
    BUTTON_IDLE = 0,
    BUTTON_RIGHT,
    BUTTON_RIGHT_TWICE,
    BUTTON_LEFT,
    BUTTON_LEFT_TWICE,
};

extern OneButton button;

void ButtonInit(void);
BUTTON_STATUS ButtonEventGet(void);
void ButtonClick(void);
void ButtonDoubleClick(void);

#endif