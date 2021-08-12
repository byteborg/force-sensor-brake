# Arduino-based force sensor brake pedal for simracing

This is a firmware for 32U4 based Arduino boards that reads a force sensor
on an analog pin, transforming the readout into one axis of an USB 
joystick device.

It is conceived and tested on a Sparkfun Micro Pro, but should work on other
32U4 based boards as well.

Originally I wanted to build it with a load cell and HX711, but they take up much space and the HX711 allows for just up to 60 measurements/sec.

So I decided to use a [film-based force sensor](https://www.aliexpress.com/item/4000003496402.html?spm=a2g0s.9042311.0.0.27424c4dZNyJWF).

## Shopping list

* Sparkfun Micro Pro or similar Atmega32U4
* Resistive film force/pressure sensor, 5-50kg
* 10R 1/8W resistor

## Wiring

```
 _____________
| 32U4    Vcc |----------+
| board       |         / \
|             |  ____   \ / sensor
|         GND |-|____|---+
|             |   10R    |
|__________A2_|----------+
```

