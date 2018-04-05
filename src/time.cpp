#include "global.h"
#include "time.h"

String timeStr = "";

String getTime(void)
{   
    return timeStr;
}

void setTime(String str) 
{
    timeStr = str;
}