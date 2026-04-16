#include "DisplayManager.h"

/*
ToDo: Data

#define JSON_SUP_VOLTAGE  "sup_voltage" // Supply voltage
#define JSON_Service      "service"   // Service mode POM// Direct cv


/*Unsuported yet
#define JSON_ERROR_CODE     "error_code"        // Error code
*/

DisplayManager::DisplayManager() : _u8g2(U8G2_R0, OLED_RST) {}

void DisplayManager::begin()
{
    _u8g2.begin();
    _u8g2.enableUTF8Print();
}

void DisplayManager::nextView()
{
    _currentView++;
    if (_currentView >= MAX_VIEWS)
    {
        _currentView = 0; // Zurück zum ersten Screen
    }
}

void DisplayManager::drawHeader(const char *title)
{
    _u8g2.setFont(u8g2_font_6x12_tf);
    _u8g2.setCursor(0, 10);
    _u8g2.print(title);

    _u8g2.drawHLine(0, 13, SCREEN_WIDTH);
}

void DisplayManager::drawHeader(const char *title, float temp)
{
    _u8g2.setFont(u8g2_font_6x12_tf);
    _u8g2.setCursor(0, 10);
    _u8g2.print(title);

    // Temperatur rechtsbündig (bei 128px Breite)
    if (temp != 0.0f)
    { // Nur zeichnen, wenn ein Wert vorliegt
        char tempBuf[10];
        snprintf(tempBuf, sizeof(tempBuf), "%.0f°C", temp);
        uint16_t width = _u8g2.getStrWidth(tempBuf);
        _u8g2.setCursor(128 - width, 10);
        _u8g2.print(tempBuf);
    }

    _u8g2.drawHLine(0, 13, SCREEN_WIDTH);
}

void DisplayManager::showWelcome()
{
    _u8g2.clearBuffer();

    // _u8g2.drawFrame(0, 0, 128, 64);
    _u8g2.drawXBMP(12, 10, 103, 21, openeremise_logo_103_21);

    _u8g2.setFont(u8g2_font_6x12_tf);
    const char *msg = "waiting for data ...";
    int x = (128 - _u8g2.getStrWidth(msg)) / 2;
    _u8g2.drawUTF8(x, 50, msg);

    _u8g2.sendBuffer();
}

void DisplayManager::showOverview(const JsonDocument &data)
{
    _u8g2.clearBuffer();

    if (data.containsKey("temperature"))
    {
        float currentTemp = data["temperature"] | 0.0f;
        drawHeader("SYSTEM", currentTemp);
    }
    else
    {
        drawHeader("SYSTEM"); // SCREEN_MAIN Titel
    }

    _u8g2.setFont(u8g2_font_helvR08_tf);

    _u8g2.drawUTF8(0, 25, "Voltage:");
    _u8g2.setCursor(50, 25);
    _u8g2.printf("%.2f V", (data["voltage"] | 0) / 1000.0);

    _u8g2.drawUTF8(0, 37, "Current:");
    _u8g2.setCursor(50, 37);
    _u8g2.printf("%d mA", data["current"] | 0);

    _u8g2.drawUTF8(0, 49, "State:");
    _u8g2.drawUTF8(50, 49, data["state"] | "---");

    _u8g2.drawUTF8(0, 61, "Version:");
    _u8g2.drawUTF8(50, 61, data["version"] | "x.x.x");

    _u8g2.sendBuffer();
}

void DisplayManager::showNetworkStatus(const JsonDocument &data)
{

    bool eth = false;
    _u8g2.clearBuffer();

    if (data.containsKey("wifi_status"))
    {
        // erweiterung Status ETH/WIFI vorhanden
        auto status = static_cast<wifi_status>(data["wifi_status"].as<uint8_t>());
        eth = data["eth_connected"].as<bool>();

        if (eth)
        {
            // LAN
            drawHeader("LAN: Connected");
        }
        else if (status == wifi_status::STA_CONNECTED)
        {
            // Logik für verbunden
            drawHeader("WiFi: Connected");
        }
        else if (status == wifi_status::AP_ACTIVE)
        {
            // Logik für Access Point
            drawHeader("AP: Active");
        }
        else if (status == wifi_status::AP_CONNECTED)
        {
            // Logik für Access Point
            drawHeader("AP: Connected");
        }
        else if (status == wifi_status::DISCONNECTED)
        {
            // Logik für getrennt
            drawHeader("WiFi: Disconnected");
        }
    }
    else
    {
        drawHeader("NETWORK");
        if (data["ssid"] == "")
            eth = true;
    }

    _u8g2.setFont(u8g2_font_helvR08_tf);

    _u8g2.drawUTF8(0, 25, "IP:");
    _u8g2.drawUTF8(35, 25, data["ip"] | "0.0.0.0");

    _u8g2.drawUTF8(0, 37, "mDNS:");
    String mdnsName = data["mdns"] | "---";
    if (mdnsName != "---" && mdnsName != "")
        mdnsName += ".local";
    _u8g2.drawUTF8(35, 37, mdnsName.c_str());

    if (!eth)
    {
        _u8g2.drawUTF8(0, 49, "SSID:");
        _u8g2.drawUTF8(35, 49, data["ssid"] | "---");

        _u8g2.drawUTF8(0, 61, "RSSI:");
        _u8g2.setCursor(35, 61);
        _u8g2.printf("%d dBm", data["rssi"] | 0);
    }

    _u8g2.sendBuffer();
}

void DisplayManager::showError(const JsonDocument &data)
{
    _u8g2.clearBuffer();

    drawHeader("ERROR");

    _u8g2.setFont(u8g2_font_helvR08_tf);
    _u8g2.drawUTF8(0, 28, "CODE:");
    _u8g2.setCursor(50, 28);
    _u8g2.print(data["error_code"] | 0);

    _u8g2.drawUTF8(0, 45, "INFO:");
    _u8g2.drawUTF8(0, 57, data["error_msg"] | "Unknown error");

    _u8g2.sendBuffer();
}

void DisplayManager::draw(const JsonDocument &data)
{
    _u8g2.clearBuffer();
    
    if (data.containsKey("error_msg"))
    {
        showError(data);
        return;
    }

    if (_currentView < 0 || _currentView >= MAX_VIEWS)
    {
        _currentView = 0;
    }

    // Priorität 2: Normale Navigation (0-3)
    switch (_currentView)
    {
    case 0:
        showNetworkStatus(data);
        break;
    case 1:
        showOverview(data);
        break;
    default:
        showNetworkStatus(data);
        _currentView = 0;
        break;
    }
}

void DisplayManager::drawImage(const unsigned char *bitmap, int w, int h)
{
    // Automatische Zentrierung
    int x = (SCREEN_WIDTH - w) / 2;
    int y = (SCREEN_HEIGHT - h) / 2;

    _u8g2.clearBuffer();
    _u8g2.drawXBMP(x, y, w, h, bitmap);
    _u8g2.sendBuffer();
}