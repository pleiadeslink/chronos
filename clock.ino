#include "SPI.h"
#include "Adafruit_GFX.h"
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include "Adafruit_ILI9341.h"
#include <Wire.h>
#include "ds3231.h"

#define ABS(X)  ((X>=0)? X : -(X) ) 
#define BUFF_MAX 128
uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 5000;
struct ts t;
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10
#define LEFT 0
#define CENTER 1
#define RIGHT 2

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

struct matrix {
    int x;
    int y;
};
static matrix clockMap[66] = {{10 * 16,0},
{11 * 16,16},
{12 * 16,16},
{13 * 16,16},
{14 * 16,16},
{15 * 16,16},
{16 * 16,16},
{17 * 16,16},
{18 * 16,16},

{304,1 * 16},
{304,2 * 16},
{304,3 * 16},
{304,4 * 16},
{304,5 * 16},
{304,6 * 16},
{304,7 * 16},
{304,8 * 16},
{304,9 * 16},
{304,10 * 16},
{304,11 * 16},
{304,12 * 16},
{304,13 * 16},

{18 * 16,224},
{17 * 16,224},
{16 * 16,224},
{15 * 16,224},
{14 * 16,224},
{13 * 16,224},
{12 * 16,224},
{11 * 16,224},
{10 * 16,224},
{9 * 16,224},
{8 * 16,224},
{7 * 16,224},
{6 * 16,224},
{5 * 16,224},
{4 * 16,224},
{3 * 16,224},
{2 * 16,224},
{1 * 16,224},

{16,13 * 16},
{16,12 * 16},
{16,11 * 16},
{16,10 * 16},
{16,9 * 16},
{16,8 * 16},
{16,7 * 16},
{16,6 * 16},
{16,5 * 16},
{16,4 * 16},
{16,3 * 16},
{16,2 * 16},
{16,1 * 16},

{1 * 16,16},
{2 * 16,16},
{3 * 16,16},
{4 * 16,16},
{5 * 16,16},
{6 * 16,16},
{7 * 16,16},
{8 * 16,16},
{9 * 16,16}};

void setup() {
    Serial.begin(9600);
    Wire.begin();
    DS3231_init(DS3231_CONTROL_INTCN);
    memset(recv, 0, BUFF_MAX);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);
}

int iter;
int mem;
int lastMinute;
int lastSecond;

void loop(void) {
    unsigned long now = millis();
    unsigned long start = micros();
    DS3231_get(&t);

    if(lastMinute != t.min) {
        lastMinute = t.min;
        tft.fillScreen(ILI9341_BLACK);
    }

    drawTime();

    // Draw progress bar
    Serial.println(t.sec);

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 8; ++j) {
            tft.drawPixel(40 + (t.sec * 4) + i, 200 + j, ILI9341_WHITE);
        }
    }

    prev = now;
    return micros() - start;
}

unsigned long drawTime() {
    char timeBuff[BUFF_MAX];
    char dateBuff[BUFF_MAX];
    //snprintf(timeBuff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year, t.mon, t.mday, t.hour, t.min, t.sec);
    snprintf(timeBuff, BUFF_MAX, "%02d:%02d", t.hour, t.min);
    snprintf(dateBuff, BUFF_MAX, "%02d/%02d/%d", t.mday, t.mon, t.year);

    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE);
    tft.setFont(&FreeSans12pt7b);
    tft.setTextSize(1);
    drawText(dateBuff, 190, 30, LEFT);
    tft.setFont(&FreeSans24pt7b);
    tft.setTextSize(2);
    drawText(timeBuff, 204, 150, CENTER);
}

void drawText(const char *buf, const int x, const int y, const int align) {
    int16_t x1, y1;
    uint16_t w, h;
    tft.setTextColor(ILI9341_WHITE);
    switch(align) {
        case LEFT:
            tft.setCursor(x, y);
            break;       
        case CENTER:
            tft.getTextBounds(buf, x, y, &x1, &y1, &w, &h);
            tft.setCursor(x - (w / 2), y);
            break;
        default:
            tft.setCursor(x, y);
            break;   
    }
    tft.print(buf);   
}