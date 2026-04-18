#include "Button.hpp"

Button::Button(uint8_t pin) 
    : _pin(pin), _lastState(HIGH), _lastDebounceTime(0), _debounceDelay(50) {
}

void Button::begin() {
    pinMode(_pin, INPUT_PULLUP);
}

bool Button::isPressed() {
    bool reading = digitalRead(_pin);
    bool pressed = false;

    if (reading != _lastState) {
        if ((millis() - _lastDebounceTime) > _debounceDelay) {
            // Nur wenn der vorherige Status HIGH (nicht gedrückt) 
            // und der neue LOW (gedrückt) ist, feuern wir ein true
            if (_lastState == HIGH && reading == LOW) {
                pressed = true;
            }
            _lastState = reading;
        }
        _lastDebounceTime = millis();
    }
    return pressed;
}