/**************************************************************************************
 * OpenRemise Display ESP32-C3 Mini
 **************************************************************************************/

#include <Arduino.h>
#include "config.h"
#include "DisplayManager.h"
#include "logo.h"
#include <U8g2lib.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "esp_bt.h"
#include "esp_wifi.h"

DisplayManager display;

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

  display.begin();

  // Einfacher Aufruf dank deiner Klasse:
  display.drawImage(openeremise_logo_48_48, 48, 48);
}

void loop()
{
  if (Serial.available() > 0)
  {
    JsonDocument doc;

    // On-the-fly Deserialisierung direkt aus dem Serial-Stream
    DeserializationError error = deserializeJson(doc, Serial);

    if (error == DeserializationError::Ok)
    {
      // Daten extrahieren
      const char *mdns = doc["mdns"] | "N/A";
      int voltage = doc["voltage"];
      int rssi = doc["rssi"];
      const char *state = doc["state"] | "unknown";

      // Display-Update
/*       u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_6x12_tf);
      u8g2.setCursor(0, 12);
      u8g2.printf("Name: %s", mdns);
      u8g2.setCursor(0, 28);
      u8g2.printf("Volt: %d mV", voltage);
      u8g2.setCursor(0, 44);
      u8g2.printf("RSSI: %d dBm", rssi);
      u8g2.setCursor(0, 60);
      u8g2.printf("State: %s", state);
      u8g2.sendBuffer();
       */
    }
    else if (error == DeserializationError::InvalidInput)
    {
      // Müll im Puffer (z.B. halbe JSONs nach dem Start) einfach weglesen
      Serial.read();
    }
  }
}
