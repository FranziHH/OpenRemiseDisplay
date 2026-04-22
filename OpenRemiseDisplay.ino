/**************************************************************************************
 * OpenRemise Display ESP32-C3 Mini
 **************************************************************************************/

#include <Arduino.h>
#include "config.hpp"
#include "Button.hpp"
#include "DisplayManager.hpp"
#include "DataManager.hpp"
#include "Logo.hpp"
#include <U8g2lib.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "esp_bt.h"
#include "esp_wifi.h"

DisplayManager display;
DataManager dataHandler;

// Buttons instanziieren (Pins aus config.hpp)
Button btnNext(BTN_NEXT_VIEW);
Button btnAction(BTN_ACTION);

void setup()
{
  // Bluetooth komplett abschalten und Speicher freigeben
  btStop();
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
  esp_bt_mem_release(ESP_BT_MODE_BTDM);

  // WiFi komplett abschalten
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  esp_wifi_stop();
  esp_wifi_deinit();

  Serial.begin(MONITOR_SPEED);

  btnNext.begin();
  btnAction.begin();

  display.begin();
  display.showMessage("", "Booting", "waiting for data ...");  // Show Logo
}

void loop()
{
  bool updateNeeded = false;
  static bool isJson = false;
  static unsigned long lastViewChange = 0;
  static unsigned long lastDataReceived = 0;

  if (!display.modal) {
    if (btnNext.isPressed())
    {
      
      display.nextView();
      updateNeeded = true;
      lastViewChange = millis();
    }

    if (btnAction.isPressed())
    {
      // Tu etwas in der aktuellen Ansicht
    }
  }

  if (dataHandler.update())
  {
    isJson = true;
    updateNeeded = true;
    lastDataReceived = millis(); // Zeitstempel aktualisieren
    display.dataTimeout = false;         // Timeout aufheben, falls aktiv
  }

  if (isJson && !display.dataTimeout && (millis() - lastDataReceived >= DATA_TIMEOUT))
  {
    display.dataTimeout = true;
    updateNeeded = true;
    isJson = false;
  }

  if (isJson && VIEW_INTERVAL != 0 && millis() - lastViewChange >= (VIEW_INTERVAL * 1000))
  {
    display.nextView();
    updateNeeded = true;
    lastViewChange = millis();
  }

  if (updateNeeded)
  {
      display.draw(dataHandler.getData()); 
  }
}
