/**************************************************************************************
 * OpenRemise Display ESP32-C3 Mini
 **************************************************************************************/

#include <Arduino.h>
#include "config.h"
#include "Button.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "Logo.h"
#include <U8g2lib.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "esp_bt.h"
#include "esp_wifi.h"

DisplayManager display;
DataManager dataHandler;

// Buttons instanziieren (Pins aus config.h)
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

  // Einfacher Aufruf dank deiner Klasse:
  // display.drawImage(openeremise_logo_48_48, 48, 48);
  display.showWelcome();
  
}

void loop()
{
  bool updateNeeded = false;

  if (btnNext.isPressed())
  {
    display.nextView();
    updateNeeded = true;
  }

  if (btnAction.isPressed())
  {
    // Tu etwas in der aktuellen Ansicht
  }

  if (dataHandler.update() || updateNeeded)
  {
    // Display nur aktualisieren, wenn neue Daten da sind
    display.draw(dataHandler.getData());
  }
}
