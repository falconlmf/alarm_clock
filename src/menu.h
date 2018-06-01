#ifndef MENU_H_
#define MENU_H_

typedef enum menus {
    MENU_FIRST = 0,
    MENU_MAIN = 0,
    MENU_ALARM,
    MENU_2,
    MENU_3,
    MENU_LAST,
};

static void menuChange(void);
static void menuUpdate(void);

extern volatile menus menuSelect;
extern void menu(void);

#endif