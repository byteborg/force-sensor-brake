/*
 * Arduino-based force sensor brake pedal for simracing
 * karsten@rohrbach.de - 20201127
 * 
 * Developed and tested on Atmega32U4 / Sparkfun Micro Pro
 */

#include <Arduino.h>
#include <Joystick.h>

// Pinout
const uint8_t SENSOR_PIN = A2;

// Constants
const uint8_t HYSTERESIS = 10;      // measurement >0 that gets filteres away
const uint8_t INIT_INTERPOL = 40;   // interpolation while iniitalizing
const uint8_t RUN_INTERPOL = 1;     // interpolation while running
const int16_t BRAKE_MAX = 511;

// Globals
int64_t tare = 0;
Joystick_ Joystick(
    JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK, 
    0, 0,                   // buttons, hats
    false, false, false,    // X, Y, Z axis
    false, false, false,    // Rx, Ry, Rz axis
    false, false,           // rudder, throttle
    false, true, false);    // accellerator, brake, steering

// Initialize on boot
void setup() {
    int64_t val = 0;

    // Init USB joystick
    Joystick.begin(false);
    Joystick.setBrakeRange(0, BRAKE_MAX);

    // Calibrate force sensor zero
    for (uint8_t i = 0; i < INIT_INTERPOL; i++)
    {
        val += analogRead(SENSOR_PIN);
    }
    tare = val / INIT_INTERPOL + HYSTERESIS;
}

// Run
void loop() {
    int64_t val = 0;

    // Measure
    for (uint8_t i = 0; i < RUN_INTERPOL; i++)
    {
        val += analogRead(SENSOR_PIN);
    }
    val = (val / RUN_INTERPOL) - tare;
    // Set axis
    Joystick.setBrake(val);
    // Send HID report
    Joystick.sendState();
    // Don't run too hot
    delay(1);
}

//.
