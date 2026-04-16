#include "DataManager.h"

DataManager::DataManager() : _newDataAvailable(false) {
    // Falls nötig, hier Vorbelegung des Dokuments
}

bool DataManager::update() {
    if (Serial.available() > 0) {
        // Wir versuchen das JSON direkt aus dem Stream zu lesen
        DeserializationError error = deserializeJson(_doc, Serial);
        
        if (error == DeserializationError::Ok) {
            _newDataAvailable = true;
            return true;
        } 
        else if (error == DeserializationError::InvalidInput) {
            // Falls der Puffer nicht mit '{' beginnt, weglesen bis zum nächsten Paketstart
            while (Serial.available() > 0 && Serial.peek() != '{') {
                Serial.read();
            }
        }
    }
    return false;
}

const JsonDocument& DataManager::getData() const {
    return _doc;
}

bool DataManager::hasNewData() {
    bool temp = _newDataAvailable;
    _newDataAvailable = false;
    return temp;
}