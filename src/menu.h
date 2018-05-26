#ifndef MENU_H_
#define MENU_H_

#include "global.h"

enum {
    MENU_FIRST = 0,
    MENU_MAIN = 0,
    MENU_ALARM,
    MENU_2,
    MENU_3,
    MENU_LAST,
};

extern uint8_t menuSelect;

void menu_handler(void);

#endif