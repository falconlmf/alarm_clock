#ifndef MENU_H_
#define MENU_H_

#include "global.h"

enum {
    MENU_FIRST = 0,
    MENU_MAIN = MENU_FIRST,
    MENU_ALARM,
    MENU_2,
    MENU_3,
    MENU_LAST = MENU_3
};

extern void menu(void);

#endif