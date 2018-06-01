#include "global.h"
#include "display.h"
#include "c.h"

#define MATRIX_LOGO_X   0
#define MATRIX_LOGO_Y   0
#define MATRIX_LOGO_W   8
#define MATRIX_LOGO_H   8
#define TIME_CUR_X  10
#define TIME_CUR_Y  1

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, D4,
    NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
    NEO_GRB            + NEO_KHZ800);

const uint16_t c_white = matrix.Color(100, 100,100);
const uint16_t c_grey = matrix.Color(60, 40, 50);
const uint16_t c_red = matrix.Color(60, 0, 0);

const XY m_logo = {MATRIX_LOGO_X, MATRIX_LOGO_Y};
const XY m_time = {TIME_CUR_X, TIME_CUR_Y};
XY m_scroll = {0, 0};

static TYPE_LOGO logo_to_print;
static TYPE_LOGO obj_to_print[5];
static uint8_t objCnt;

Ticker ticker_display;

static void matrix_update_time(String _t);
static void matrix_update_logo(uint8_t _logo);

void displayInit(void)
{
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(80);
    matrix.setFont(&c);
    matrix.fillScreen(0);    
    matrix.setCursor(10, 1);   
    matrix.setTextColor(c_grey);     
    matrix.print("12:34");
    matrix.show();

	ticker_display.attach_ms(20, displayUpdate);
}

static void matrix_update_logo(uint8_t _logo)
{
    matrix.setCursor(m_logo.x, m_logo.y);
    matrix.setTextColor(c_red);
    matrix.print(char(_logo));
    
    // matrix.setCursor(m_logo.x, m_logo.y);
    // matrix.setTextColor(c_grey);
    // matrix.print(char(128));
}

static void matrix_update_time(String _t)
{
    matrix.setCursor(m_time.x + m_scroll.x, m_time.y + m_scroll.y);
    matrix.setTextColor(c_grey);
    matrix.print(_t);
}

void displayReset(void)
{
    objCnt = 0;
}

void displayAddObj(uint8_t id, uint8_t x, uint8_t y, uint16_t c, String str)
{
    obj_to_print[objCnt].x = x;
    obj_to_print[objCnt].y = y;
    obj_to_print[objCnt].c = c;
    obj_to_print[objCnt].str = str;
    objCnt ++;
}

void displayUpdate(void)
{
    matrix.fillScreen(0);
    
    for(uint8_t i = 0; i < objCnt; i++) {
        // Serial.print("obj:"+String(i)+" text:"+String(obj_to_print[i].str)+" color:"+String(obj_to_print[i].c)+", ");
        matrix.setCursor(obj_to_print[i].x, obj_to_print[i].y);
        matrix.setTextColor(obj_to_print[i].c);
        matrix.print(obj_to_print[i].str);
    }
    // Serial.println();
    matrix.show();

}
