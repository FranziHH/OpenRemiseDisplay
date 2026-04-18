#pragma once

#include <Arduino.h>

class Button {
private:
    uint8_t _pin;
    bool _lastState;
    unsigned long _lastDebounceTime;
    unsigned long _debounceDelay;

public:
    Button(uint8_t pin);
    void begin();
    
    // Liefert true, wenn die Taste gerade erst gedrückt wurde (Flankenerkennung)
    bool isPressed();
};