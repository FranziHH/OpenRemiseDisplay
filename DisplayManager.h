#pragma once

#include <ArduinoJson.h>
#include <U8g2lib.h>
#include "config.h"

class DisplayManager {
private:
    // Hier wählen wir den passenden Konstruktor basierend auf der config.h
    #if defined(USE_SSD1306)
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C _u8g2;
    #elif defined(USE_SH1106)
        U8G2_SH1106_128X64_NONAME_F_HW_I2C _u8g2;
    #elif defined(USE_SSD1309)
        U8G2_SSD1309_128X64_NONAME0_F_HW_I2C _u8g2;
    #else
        #error "Bitte einen Display-Typ in der config.h definieren!"
    #endif

public:
    // Konstruktor
    DisplayManager() : _u8g2(U8G2_R0, OLED_RST) {}

    void begin() {
        _u8g2.begin();
    }

    // Die von dir gewünschte Funktion
    void drawImage(const unsigned char* bitmap, int w, int h) {
        // Automatische Zentrierung
        int x = (SCREEN_WIDTH - w) / 2;
        int y = (SCREEN_HEIGHT - h) / 2;
        
        _u8g2.clearBuffer();
        _u8g2.drawXBMP(x, y, w, h, bitmap);
        _u8g2.sendBuffer();
    }

    // Platzhalter für deine 4 Ansichten
    void drawView(int viewIndex, const JsonDocument& data) {
        _u8g2.clearBuffer();
        switch(viewIndex) {
            case 0: drawOverview(data); break;
            // case 1: drawVoltageDetails(data); break;
            // ... usw.
        }
        _u8g2.sendBuffer();
    }

private:
    void drawOverview(const JsonDocument& data) {
        _u8g2.setFont(u8g2_font_6x12_tf);
        _u8g2.setCursor(0, 12);
        _u8g2.print("Übersicht");
        // Hier weitere Details zeichnen...
    }
};