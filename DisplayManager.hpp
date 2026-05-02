#pragma once

#include <ArduinoJson.h>
#include <U8g2lib.h>
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
    unsigned long _lastModalTimeOut = millis();
    unsigned long _modalTimeOut = 10000;
    int _screenWidth = 128;
    int _screenHeight = 64;

    U8G2* _u8g2 = nullptr;

    // Private Zeichen-Methoden (Interne Helfer)
    void drawHeader(const char* viewName);
    void drawHeader(const char* viewName, float temp);
    void showOverview(const JsonDocument& data);
    void showNetworkStatus(const JsonDocument& data);

public:
    enum class DisplayType {
        SSD1306,
        SH1106,
        SSD1309
    };
    bool modal = true;
    bool dataTimeout = false;   // no Data
    DisplayManager();
    ~DisplayManager();
    void begin(int sda, int scl, int reset, int addr, DisplayType type);
    void nextView();
    void drawImage(const unsigned char* bitmap, int w, int h);
    void showMessage(const char *title, const char *line1, const char *line2);
    void showError(const JsonDocument& data);
    void draw(const JsonDocument& data);
};