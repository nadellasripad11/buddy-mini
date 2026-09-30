# 03 — block diagram

*session 1, 2026-09-30*

how the five parts hang off the esp32. pin numbers are tbd until step 3 — this
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
| oled | esp32 → screen | i2c (sda + scl) | 4 (sda, scl, 3v3, gnd) | tbd |
| button 1 | button → esp32 | digital in | 2 (signal, gnd) | tbd |
| button 2 | button → esp32 | digital in | 2 (signal, gnd) | tbd |
| buzzer | esp32 → buzzer | pwm out (tone) | 2 (signal, gnd) | tbd |
| status led | esp32 → led | digital out | 2 (signal + 220r, gnd) | tbd |

## things this diagram made obvious

- **it's a star, not a chain.** nothing connects to anything except the esp32.
  if one part is broken it can't break the others, which should make debugging
  a lot easier — i can unplug things one at a time.
- **two arrows point in, three point out.** the buttons are the only inputs.
  everything else is the esp32 talking.
- **every single part needs gnd.** five parts, five ground connections — that's
  what the long breadboard rails are for, instead of cramming five wires into
  the esp32's ground pins.
- **the oled is the only part that needs 3.3v power** on top of its data wires.
  the buzzer and led get their power *from* the signal pin itself.

## what i still have to decide (step 3)

- which actual gpio numbers each part gets
- whether the buttons use physical 10k resistors or the esp32's built-in pull-ups
- where on the breadboard each part physically sits
