#include "DisplayManager.hpp"
#include <Wire.h>

/*
ToDo: Data

#define JSON_SUP_VOLTAGE  "sup_voltage" // Supply voltage
#define JSON_Service      "service"   // Service mode POM// Direct cv


/*Unsuported yet
#define JSON_ERROR_CODE     "error_code"        // Error code
*/

DisplayManager::DisplayManager() : _u8g2(nullptr) {}
DisplayManager::~DisplayManager()
{
    if (_u8g2)
        delete _u8g2;
}

void DisplayManager::init(int sda, int scl, int reset, int addr, DisplayType type, int trackLED)
{
    _sda = sda;
    _scl = scl;
    _reset = reset;
    _addr = addr;
    _type = type;
    _trackLED = trackLED;
    if (trackLED != 0)
    {
        _isTrackLED = true;
        if (trackLED == 8)
        {
            // internal LED is inverted
            _trackLedOn = LOW;
            _trackLedOff = HIGH;
        }
    }
}

void DisplayManager::begin()
{
    Wire.begin(_sda, _scl);

    if (_u8g2)
        delete _u8g2;

    switch (_type)
    {
    case DisplayType::SH1106:
        _u8g2 = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, _reset);
        break;
    case DisplayType::SSD1309:
        _u8g2 = new U8G2_SSD1309_128X64_NONAME0_F_HW_I2C(U8G2_R0, _reset);
        break;
    case DisplayType::SSD1306:
    default:
        _u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, _reset);
        break;
    }

    if (_u8g2)
    {
        _u8g2->setI2CAddress(_addr * 2);
        _u8g2->begin();
        _u8g2->enableUTF8Print();
    }
    if (_isTrackLED)
    {
        pinMode(_trackLED, OUTPUT);
        digitalWrite(_trackLED, _trackLedOff);
    }
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
    _u8g2->setFont(u8g2_font_6x12_tf);
    _u8g2->setCursor(0, 10);
    _u8g2->print(title);

    _u8g2->drawHLine(0, 13, _screenWidth);
}

void DisplayManager::drawHeader(const char *title, const char *hwrev, float temp)
{
    _u8g2->setFont(u8g2_font_6x12_tf);
    _u8g2->setCursor(0, 10);
    _u8g2->print(title);

    if (hwrev != "") {
        _u8g2->setCursor(50, 10);
        _u8g2->print(hwrev);
    }
    
    // Temperatur rechtsbündig (bei 128px Breite)
    if (temp != 0.0f)
    { // Nur zeichnen, wenn ein Wert vorliegt
        uint16_t offset = 5;
        char tempBuf[10];
        snprintf(tempBuf, sizeof(tempBuf), "%.0f°C", temp);
        uint16_t width = _u8g2->getStrWidth(tempBuf);
        _u8g2->setCursor(128 - width + offset, 10);
        _u8g2->print(tempBuf);
    }

    _u8g2->drawHLine(0, 13, _screenWidth);
}

void DisplayManager::showMessage(const char *title, const char *line1, const char *line2)
{
    _u8g2->clearBuffer();
    _u8g2->setFont(u8g2_font_6x12_tf);

    if (title == "")
    {
        _u8g2->drawXBMP(12, 5, 103, 21, openeremise_logo_103_21);
    }
    else
    {
        int x = (128 - _u8g2->getStrWidth(title)) / 2;
        _u8g2->drawUTF8(x, 40, title);
    }

    if (line1 != "")
    {

        int x = (128 - _u8g2->getStrWidth(line1)) / 2;
        _u8g2->drawUTF8(x, 40, line1);
    }

    if (line2 != "")
    {
        int x = (128 - _u8g2->getStrWidth(line2)) / 2;
        _u8g2->drawUTF8(x, 57, line2);
    }

    _u8g2->sendBuffer();
}

void DisplayManager::showOverview(const JsonDocument &data)
{
    _u8g2->clearBuffer();
    bool legacy = true;

    if (data.containsKey("temperature"))
    {   
        String revision = "";
        if (data.containsKey("revision")) {
            revision = data["revision"] | "";
            // if (revision != "") revision = "hw:" + revision;
            if (revision != "0.1.0") legacy = false;
        }
        
        float currentTemp = data["temperature"] | 0.0f;
        drawHeader("SYSTEM:", revision.c_str(), currentTemp);
    }
    else
    {
        drawHeader("SYSTEM"); // SCREEN_MAIN Titel
    }

    _u8g2->setFont(u8g2_font_helvR08_tf);

    if (data.containsKey("voltage"))
    {
        _u8g2->drawUTF8(0, 25, "Voltage:");
        _u8g2->setCursor(40, 25);
        _u8g2->printf("%.2f V", (data["voltage"] | 0) / 1000.0);
    }
    else if (data.containsKey("vcc_voltage") && data.containsKey("supply_voltage"))
    {
        _u8g2->drawUTF8(0, 25, "Voltage:");
        _u8g2->setCursor(40, 25);

        // Beide Werte kompakt in einer Zeile ausgeben
        float vcc = (data["vcc_voltage"] | 0) / 1000.0;
        float supply = (data["supply_voltage"] | 0) / 1000.0;

        if (legacy)
        {
            _u8g2->printf("%.2fV", supply);
        }
        else
        {
            _u8g2->printf("%.2fV / %.2fV", supply, vcc);
        }
    }

    _u8g2->drawUTF8(0, 37, "Current:");
    _u8g2->setCursor(40, 37);
    _u8g2->printf("%d mA", data["current"] | 0);

    _u8g2->drawUTF8(0, 49, "State:");
    _u8g2->drawUTF8(40, 49, data["state"] | "---");

    _u8g2->drawUTF8(0, 61, "Version:");
    if (strcmp(data["version"] | "", "debug") != 0 && data.containsKey("compile_date")) {
        char buffer[64];
        char monthStr[4];
        int day, year;
        sscanf(data["compile_date"], "%3s %d %d", monthStr, &day, &year);
        String v = data["version"] | "x.x.x";
        v.replace(" FW", "FW");

        int month = 0;
        const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        for(int i = 0; i < 12; i++) {
            if(strcmp(monthStr, months[i]) == 0) {
                month = i + 1;
                break;
            }
        }

        // String aus Version und Datum zusammenbauen
        snprintf(buffer, sizeof(buffer), "%s, %02d-%02d-%02d", v.c_str(), year % 100, month, day);

        _u8g2->drawUTF8(40, 61, buffer);
    } else {
        _u8g2->drawUTF8(40, 61, data["version"] | "x.x.x");
    }

    _u8g2->sendBuffer();
}

void DisplayManager::showNetworkStatus(const JsonDocument &data)
{

    bool eth = false;
    _u8g2->clearBuffer();

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

    _u8g2->setFont(u8g2_font_helvR08_tf);

    _u8g2->drawUTF8(0, 25, "IP:");
    _u8g2->drawUTF8(35, 25, data["ip"] | "0.0.0.0");

    _u8g2->drawUTF8(0, 37, "mDNS:");
    String mdnsName = data["mdns"] | "---";
    if (mdnsName != "---" && mdnsName != "")
        mdnsName += ".local";
    _u8g2->drawUTF8(35, 37, mdnsName.c_str());

    if (!eth)
    {
        _u8g2->drawUTF8(0, 49, "SSID:");
        _u8g2->drawUTF8(35, 49, data["ssid"] | "---");

        _u8g2->drawUTF8(0, 61, "RSSI:");
        _u8g2->setCursor(35, 61);
        _u8g2->printf("%d dBm", data["rssi"] | 0);
    }

    _u8g2->sendBuffer();
}

void DisplayManager::showError(const JsonDocument &data)
{
    _u8g2->clearBuffer();

    drawHeader("ERROR");

    _u8g2->setFont(u8g2_font_helvR08_tf);
    _u8g2->drawUTF8(0, 28, "CODE:");
    _u8g2->setCursor(40, 28);
    _u8g2->print(data["error_code"] | 0);

    _u8g2->drawUTF8(0, 45, "INFO:");
    _u8g2->drawUTF8(0, 57, data["error_msg"] | "Unknown error");

    _u8g2->sendBuffer();
}

/*
Es gibt ein Timeout für bestimmte Nachrichten
Wie Reboot oder Error
die nicht durch andere Nachrichten in dieser
Zeit überschrieben werden dürfen
*/
void DisplayManager::draw(const JsonDocument &data)
{
    // im Moment 5 Sekunden fix
    if (millis() - _lastModalTimeOut < _modalTimeOut)
        return;

    _u8g2->clearBuffer();
    modal = false;

    if (_isTrackLED && data.containsKey("state"))
    {
        String stateValue = data["state"] | "";
        if (stateValue.equalsIgnoreCase("dccoperations"))
        {
            digitalWrite(_trackLED, _trackLedOn);
        }
        else
        {
            digitalWrite(_trackLED, _trackLedOff);
        }
    }

    if (data.containsKey("error_msg"))
    {
        showError(data);
        _lastModalTimeOut = millis();
        modal = true;
        return;
    }

    /*
    // idee war den Reset Button abzufangen,
    // das fkt aber nicht wie gewünscht
    bool isMissingData = (data["ip"].isNull() || data["ip"] == "") &&
                         (data["mdns"].isNull() || data["mdns"] == "");
    if (dataTimeout || isMissingData)
    */
    if (dataTimeout)
    {
        showMessage("", "Data Connection Lost", "waiting for data ..."); // show logo
        _lastModalTimeOut = millis();
        modal = true;
        return;
    }

    if (data.containsKey("is_restarting"))
    {
        bool restart = data["is_restarting"].as<bool>();
        if (restart)
        {
            if (_isTrackLED)
                digitalWrite(_trackLED, _trackLedOff);
            showMessage("", "Restart", ""); // show logo
            _lastModalTimeOut = millis();
            modal = true;
            return;
        }
    }

    if (data.containsKey("wifi_status"))
    {
        // erweiterung Status ETH/WIFI vorhanden
        auto status = static_cast<wifi_status>(data["wifi_status"].as<uint8_t>());

        if (status == wifi_status::AP_ACTIVE)
        {
            // Logik für Access Point
            showMessage("", "Access Point", "is Active"); // show logo
            modal = true;
            return;
        }
        else if (status == wifi_status::AP_CONNECTED)
        {
            // Logik für Access Point
            showMessage("", "Access Point", "Client connected"); // show logo
            modal = true;
            return;
        }
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
    int x = (_screenWidth - w) / 2;
    int y = (_screenHeight - h) / 2;

    _u8g2->clearBuffer();
    _u8g2->drawXBMP(x, y, w, h, bitmap);
    _u8g2->sendBuffer();
}