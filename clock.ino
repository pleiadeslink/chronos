#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Wire.h>
#include "ds3231.h"

#define BUFF_MAX 128
uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 5000;
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


void setup() {
    Serial.begin(9600);
    Wire.begin();
    DS3231_init(DS3231_CONTROL_INTCN);
    memset(recv, 0, BUFF_MAX);
    tft.begin();
    tft.setRotation(3);

    // read diagnostics (optional but can help debug problems)
    uint8_t x = tft.readcommand8(ILI9341_RDMODE);
    Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDMADCTL);
    Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDPIXFMT);
    Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDIMGFMT);
    Serial.print("Image Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDSELFDIAG);
    Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
}


void loop(void) {
    testText();
    delay(1000);
}

unsigned long testText() {
    char in;
    char buff[BUFF_MAX];
    unsigned long now = millis();
    struct ts t;

    tft.fillScreen(ILI9341_BLACK);
    unsigned long start = micros();
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(4);
    tft.println("");
    tft.println("Ese");
    tft.println("retrojuanker");
    tft.println("ahi");
    tft.println("");
    tft.setTextSize(5);
    tft.setTextColor(ILI9341_GREEN);
    tft.println("   :)");

    /*if ((Serial.available() <= 0)) {
        DS3231_get(&t);
        #ifdef CONFIG_UNIXTIME
        #ifdef __AVR__
            snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d %ld", t.year,
        #error AVR
        #else
            snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d %d", t.year,
        #endif
            t.mon, t.mday, t.hour, t.min, t.sec, t.unixtime);
        #else
            snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year,
            t.mon, t.mday, t.hour, t.min, t.sec);
        #endif
        tft.println(buff);
        prev = now;
    }*/

    return micros() - start;
}

