# 03 — block diagram

*session 1, 2026-09-30*

how the five parts hang off the ESP32. pin numbers are TBD until step 3 — this
diagram is about *what connects to what* and *which direction the signal flows*,
not which metal leg yet.

![block diagram](../media/block-diagram.png)

## simple view

```
esp32
 ├── oled          buddy's face
 ├── button 1      touch input
 ├── button 2      touch input
 ├── buzzer        buddy's voice
 └── status led    "i'm awake"
```

## the same thing as a table

| part | direction | signal type | wires | pin |
|---|---|---|---|---|
| OLED | ESP32 → screen | I2C (SDA + SCL) | 4 (SDA, SCL, 3V3, GND) | TBD |
| button 1 | button → ESP32 | digital in | 2 (signal, GND) | TBD |
| button 2 | button → ESP32 | digital in | 2 (signal, GND) | TBD |
| buzzer | ESP32 → buzzer | PWM out (tone) | 2 (signal, GND) | TBD |
| status LED | ESP32 → LED | digital out | 2 (signal + 220R, GND) | TBD |

## things this diagram made obvious

- **it's a star, not a chain.** nothing connects to anything except the ESP32.
  if one part is broken it can't break the others, which should make debugging
  a lot easier — i can unplug things one at a time.
- **two arrows point in, three point out.** the buttons are the only inputs.
  everything else is the ESP32 talking.
- **every single part needs GND.** five parts, five ground connections — that's
  what the long breadboard rails are for, instead of cramming five wires into
  the ESP32's ground pins.
- **the OLED is the only part that needs 3.3V power** on top of its data wires.
  the buzzer and LED get their power *from* the signal pin itself.

## what i still have to decide (step 3)

- which actual GPIO numbers each part gets
- whether the buttons use physical 10k resistors or the ESP32's built-in pull-ups
- where on the breadboard each part physically sits
