/*
 * Arduino-based force sensor brake pedal for simracing
 * karsten@rohrbach.de - 20210411
 * 
 * Developed and tested on Atmega32U4 / Sparkfun Micro Pro
 */

#include <Arduino.h>
#include <Joystick.h>

// Pinout
const uint8_t CLUTCH_PIN = A1;
const uint8_t BRAKE_PIN = A2;
const uint8_t GAS_PIN = A0;

// Constants
const uint8_t HYSTERESIS = 10;      // measurement >0 that gets filtered away
const uint8_t INIT_INTERPOL = 40;   // interpolation while iniitalizing
const uint8_t RUN_INTERPOL = 3;     // interpolation while running
const uint16_t INTERPOL_DELAY = 1;  // delay between measurements
const int16_t BRAKE_MAX = 511;

// Globals
int64_t clutch_tare = 0;
int64_t brake_tare = 0;
int64_t gas_tare = 0;
Joystick_ Joystick(
    JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
    0, 0,                   // buttons, hats
    true, true, true,      // X, Y, Z axis
    false, false, false,    // Rx, Ry, Rz axis
    false, false,           // rudder, throttle
    false, false, false);    // accellerator, brake, steering

int64_t measure(uint8_t pin, uint8_t interpol, int64_t tare);


// Initialize on boot
void setup() {
    // Init USB joystick
    Joystick.begin(false);
    Joystick.setBrakeRange(0, BRAKE_MAX);

    // Calibrate sensor zero
    clutch_tare = measure(CLUTCH_PIN, INIT_INTERPOL, 0) + HYSTERESIS;
    brake_tare = measure(BRAKE_PIN, INIT_INTERPOL, 0) + HYSTERESIS;
    gas_tare = measure(GAS_PIN, INIT_INTERPOL, 0) + HYSTERESIS;
}


// Run
void loop() {

    // Set axis
    Joystick.setXAxis(measure(CLUTCH_PIN, RUN_INTERPOL, clutch_tare));
    Joystick.setYAxis(measure(BRAKE_PIN, RUN_INTERPOL, brake_tare));
    Joystick.setZAxis(measure(GAS_PIN, RUN_INTERPOL, gas_tare));
    // Send HID report
    Joystick.sendState();
    // Don't run too hot
    delay(1);
}


// Measure analog pin and return tare-corrected value
int64_t measure(uint8_t pin, uint8_t interpol, int64_t tare) {
    int64_t val = 0;

    for (uint8_t i = 0; i < interpol; i++) {
        val += analogRead(pin);
        delay(INTERPOL_DELAY);
    }
    return (val / interpol) - tare;
}

//.
