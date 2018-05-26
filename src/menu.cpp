#include "menu.h"
#include "clock.h"
#include "display.h"
#include "button.h"

uint8_t menuSelect;
uint8_t menuChange;

void menu_handler(void)
{
    switch (ButtonEventGet()) {
        case BUTTON_RIGHT:
            if (++menuSelect >= MENU_LAST) {
                menuSelect = MENU_MAIN;
            }
            menuChange = true;
            break;
        case BUTTON_RIGHT_TWICE:
            if (menuSelect > MENU_MAIN) {
                menuSelect--;
            } else {
                menuSelect = MENU_LAST;
            }
            menuChange = true;
            break;
        default:
            break;
    }

    if (!menuChange) {
        return;
    }

    switch (menuSelect) {
        case MENU_MAIN:
            displayReset();
            displayAddObj(0, 0, c_grey, String(char(128)));
            displayAddObj(0, 0, c_red, String(char(129)));
            displayAddObj(10, 1, c_grey, getTime());
            break;
        case MENU_ALARM:
            displayReset();
            displayAddObj(10, 1, c_grey, getAlarm());
            break;
        case MENU_2:
            displayReset();
            displayAddObj(10, 1, c_grey, "2");
            break;
        case MENU_3:
            displayReset();
            displayAddObj(10, 1, c_grey, "3");
            break;
        default:
            break;
    }
    menuSelect = false;
}