#pragma once

#include <ArduinoJson.h>
#include <U8g2lib.h>
#include "config.hpp"
#include "Logo.hpp"

#define MAX_VIEWS 2

class DisplayManager {
private:
    enum class wifi_status : uint8_t {
        DISCONNECTED,
        STA_CONNECTED,
        AP_ACTIVE,
        AP_CONNECTED
    };

    int _currentView = 0;
    unsigned long lastModalTimeOut = millis();

    // Display-Treiber Auswahl
    #if defined(USE_SSD1306)
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C _u8g2;
    #elif defined(USE_SH1106)
        U8G2_SH1106_128X64_NONAME_F_HW_I2C _u8g2;
    #elif defined(USE_SSD1309)
        U8G2_SSD1309_128X64_NONAME0_F_HW_I2C _u8g2;
    #endif

    // Private Zeichen-Methoden (Interne Helfer)
    void drawHeader(const char* viewName);
    void drawHeader(const char* viewName, float temp);
    void showOverview(const JsonDocument& data);
    void showNetworkStatus(const JsonDocument& data);

public:
    bool modal = true;
    bool dataTimeout = false;   // no Data
    DisplayManager();
    void begin();
    void nextView();
    void drawImage(const unsigned char* bitmap, int w, int h);
    void showMessage(const char *title, const char *line1, const char *line2);
    void showError(const JsonDocument& data);
    void draw(const JsonDocument& data);
};