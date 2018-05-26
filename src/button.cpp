#include <Arduino.h>
#include <OneButton.h>
#include "button.h"

BUTTON_STATUS buttonEvent = BUTTON_IDLE;
OneButton button(D3, true);

void ButtonInit(void)
{
	button.setDebounceTicks(20);
	button.setClickTicks(300);
	button.attachClick(ButtonClick);
	button.attachDoubleClick(ButtonDoubleClick);
}

BUTTON_STATUS ButtonEventGet(void)
{
    BUTTON_STATUS i = buttonEvent;
    buttonEvent = BUTTON_IDLE;
    return i;
}

void ButtonClick(void)
{
    Serial.println("BUTTON_RIGHT");
    buttonEvent = BUTTON_RIGHT;
}

void ButtonDoubleClick(void)
{
    Serial.println("BUTTON_RIGHT_TWICE");
    buttonEvent = BUTTON_RIGHT_TWICE;
}