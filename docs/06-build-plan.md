# 06 — build plan

*session 1, 2026-09-30*

one hour per session, one goal per session. each session ends with something that
visibly works, so a failure is always isolated to the thing i just added.

## session 2 — toolchain — done (2026-09-30)

done before the kit arrived. everything was already installed:

| thing | version |
|---|---|
| arduino-cli | 1.5.1 |
| esp32 core | 3.3.11 |
| `Adafruit SSD1306` | 2.5.17 |
| `Adafruit GFX` | 1.12.6 |

wrote the full v1 firmware and compiled it against `esp32:esp32:esp32doit-devkit-v1`:

```
Sketch uses 317714 bytes (24%) of program storage space.
Global variables use 24004 bytes (7%) of dynamic memory.
```

**`tone()` compiles fine on core 3.3.11**, so the buzzer does not need the ledc
fallback. that was the open question and it is now closed.

original plan for this session, kept for the record:

- install the arduino ide
- add esp32 board support (boards manager → "esp32" by espressif)
- install libraries: `Adafruit SSD1306` and `Adafruit GFX`
- paste in the generated buzzer sketch and hit **verify** (compile only, no board)

the point is to find out now whether `tone()` compiles on my core version. if it
errors with `tone was not declared in this scope`, the core is older than 3.x and
the buzzer has to be driven through LEDC instead. better to know that today than
at 1am with the hardware in front of me.

## session 3 — first blink

the kit has arrived. **do not wire anything yet.**

- unbox, lay parts out, photograph them
- plug in the esp32, pick the right port
- upload the stock `Blink` example, change it to blink the onboard led on pin 2

nothing else happens until this works. a blinking onboard led proves the board,
the cable, the drivers, the port and the upload process all work at once. trying
to debug a face on an unproven board would be miserable.

## session 4 — the screen wakes up

- wire the oled: sda→21, scl→22, vcc→3v3, gnd→gnd
- run an **i²c scanner** sketch first — it prints the address of whatever it finds.
  most ssd1306 boards are at `0x3C`, some are `0x3D`. knowing the real address
  before drawing anything saves a lot of guessing.
- draw a rectangle, then some text
- then draw the **normal face**: two filled circles and a mouth

## session 5 — buddy comes alive

- add the idle blink on a `millis()` timer: normal face, blink face for ~150ms,
  back to normal, every 3–5 seconds
- this is the session that decides whether buddy reads as a character. tune the
  timing until it feels right rather than looking mechanical

## session 6 — touch

- wire both buttons: one leg to the gpio, other leg to the ground rail
- `pinMode(BTN, INPUT_PULLUP)` — a press reads low
- add debouncing
- button 1 → happy face for 2s. button 2 → double blink

## session 7 — voice and light

- wire the buzzer to 25, the led to 26 through the 220Ω resistor
- drop in the two sound functions
- connect them to the button handlers

## session 8 — finish and film

- tidy the wiring so it photographs well
- run through every feature once
- **record the demo video in one take**

if it can't be filmed working in one take, v1 isn't done.

## known things to fix in v2

- sounds use `delay()`, which freezes the eyes while playing. proper fix is a
  non-blocking `millis()` state machine.
- a speaker module (dfplayer mini + small 8Ω speaker, ~$5) would let buddy play
  real audio instead of buzzer tones. deliberately **not** in v1 — adding a second
  audio path before the first one works means not knowing which half is broken.
