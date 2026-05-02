#pragma once

#include <ArduinoJson.h>
#include <U8g2lib.h>
#include "Logo.hpp"

#define MAX_VIEWS 2

class DisplayManager {
public:
    enum class DisplayType {
        SSD1306,
        SH1106,
        SSD1309
    };

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
    int _sda;
    int _scl;
    int _reset; 
    int _addr;
    DisplayType _type;
    int _trackLED;
    bool _isTrackLED = false;
    int _trackLedOn = HIGH;
    int _trackLedOff = LOW;

    U8G2* _u8g2 = nullptr;

    // Private Zeichen-Methoden (Interne Helfer)
    void drawHeader(const char* viewName);
    void drawHeader(const char* viewName, float temp);
    void showOverview(const JsonDocument& data);
    void showNetworkStatus(const JsonDocument& data);

public:
    bool modal = true;
    bool dataTimeout = false;   // no Data
    DisplayManager();
    ~DisplayManager();
    void init(int sda, int scl, int reset, int addr, DisplayType type, int trackLED = 0);
    void begin();
    void nextView();
    void drawImage(const unsigned char* bitmap, int w, int h);
    void showMessage(const char *title, const char *line1, const char *line2);
    void showError(const JsonDocument& data);
    void draw(const JsonDocument& data);
};