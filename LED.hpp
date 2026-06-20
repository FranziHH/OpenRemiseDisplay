#pragma once
#include <Arduino.h>

class LED {
private:
    uint8_t _pin;
    bool _activeLow;
    TaskHandle_t _taskHandle = NULL;
    
    // Daten für den Task
    unsigned long _onTime;
    unsigned long _offTime;

    // Die Task-Funktion
    static void BlinkTask(void *pvParameters) {
        LED* instance = static_cast<LED*>(pvParameters);
        bool state = false;
        
        for (;;) {
            digitalWrite(instance->_pin, state ? (instance->_activeLow ? LOW : HIGH) : (instance->_activeLow ? HIGH : LOW));
            vTaskDelay((state ? instance->_onTime : instance->_offTime) / portTICK_PERIOD_MS);
            state = !state;
        }
    }

    void stopTask() {
        if (_taskHandle != NULL) {
            vTaskDelete(_taskHandle);
            _taskHandle = NULL;
        }
    }

public:
    LED(uint8_t pin, bool activeLow = true) : _pin(pin), _activeLow(activeLow) {
        pinMode(_pin, OUTPUT);
        off();
    }

    ~LED() { stopTask(); }

    void on() {
        stopTask();
        digitalWrite(_pin, _activeLow ? LOW : HIGH);
    }

    void off() {
        stopTask();
        digitalWrite(_pin, _activeLow ? HIGH : LOW);
    }

    void blink(unsigned long onTime, unsigned long offTime) {
        if (_taskHandle != NULL) return; // Läuft schon
        _onTime = onTime;
        _offTime = offTime;
        xTaskCreate(BlinkTask, "BlinkTask", 2048, this, 1, &_taskHandle);
    }
};