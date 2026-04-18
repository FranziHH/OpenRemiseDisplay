#pragma once

#include <Arduino.h>

// --- DISPLAY TYP WÄHLEN ---
// Kommentiere aus, was du NICHT verwendest
//#define USE_SSD1306
//#define USE_SH1106
#define USE_SSD1309

// Display Dimensionen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C Pins für den ESP32-C3
// GPIO 8 und 9 sind Standard, aber hier kannst du sie jederzeit ändern
#define OLED_SDA 8
#define OLED_SCL 9

// Display Reset Pin (falls dein 2.4" einen hat, sonst auf -1 lassen)
#define OLED_RST -1

// Serielle Schnittstelle
#define MONITOR_SPEED 115200

// JSON Puffergröße (ArduinoJson 7)
// Ein Dokument von ca. 512-1024 Bytes reicht für die meisten Steuerungen dicke
#define JSON_CAPACITY 1024

// Zeitüberschreitung für Daten (in Millisekunden)
// Wenn nach 5 Sekunden nichts kommt, zeigen wir "Offline"
#define DATA_TIMEOUT 15000

// nach Error / Restart / Connection Lost
#define MODAL_TIMEOUT 10000

// Buttons gegen GND geschaltet
#define BTN_NEXT_VIEW 4  // GPIO 4
#define BTN_ACTION    5  // GPIO 5
#define BTN_UP        6  // GPIO 6
#define BTN_DOWN      7  // GPIO 7

#define VIEW_INTERVAL 10 // Next View after Seconds, 0 = Disable
