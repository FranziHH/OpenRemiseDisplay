#pragma once

#include <ArduinoJson.h>
#include "config.h"

class DataManager {
private:
    JsonDocument _doc;
    bool _newDataAvailable;

public:
    DataManager();

    // Verarbeitet eingehende Serial-Daten
    bool update();

    // Zugriff auf die geparsten Daten
    const JsonDocument& getData() const;

    // Prüft, ob seit dem letzten Aufruf neue Daten kamen
    bool hasNewData();
};