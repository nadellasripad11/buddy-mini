# buddy mini

a small desk buddy that reacts to me with animated eyes, sounds, and buttons.

i want it to feel like a little character sitting on my desk instead of just being
another circuit. this is my first step toward a bigger desk robot that can
eventually move around, listen to me, and talk back.

built for hack club half life. i'm a beginner with hardware — this repo is the real
record of me learning it, including the parts i got wrong.

---

## what i'm aiming at

![design target](media/reference/design-target.webp)

that's the end goal, not v1. v1 is the same brain and the same face, running on a
bare breadboard with wires everywhere. the 3d printed body comes much later.

---

## how it connects

![block diagram](media/block-diagram.png)

it's a star, not a chain. nothing talks to anything except the esp32, so a broken
part can't take the others down with it, and i can unplug things one at a time when
something misbehaves.

## the pins

![pin plan](media/pin-plan.png)

## where i'm at

| version | what it is | state |
|---|---|---|
| v1 (warm-up) | breadboard: oled face, 2 buttons, buzzer, status led | firmware written and compiling, waiting on parts |
| v2 | tidier wiring, more faces, a sleep mode | not started |
| v3 | 3d printed body | not started |
| v4 (someday) | movement, mic, talking back | out of scope for half life |

---

## what buddy mini v1 does

1. shows an animated face on a small oled screen
2. changes expression when i press its buttons
3. makes a different sound for each button
4. has a bit of personality through those faces and reactions
5. blinks on its own every few seconds, even when nobody's touching it
6. has a status led so you can tell it's awake

number 5 matters most. without it buddy is frozen until you poke it, and a frozen
face reads as a screen, not a character.

## what it doesn't do yet

- move around
- listen to me or talk back
- use a microphone
- use ai or voice recognition
- run on a battery — it's usb powered
- need any soldering — breadboard only
- have a case

this list is as important as the first one. it's what stops me wandering off into
motors and voice control before i've got a single pixel on a screen.

## how i'll know v1 is done

- the oled shows its face
- the face changes between expressions
- the buttons trigger different reactions
- the buzzer makes different sounds
- it all works together on the esp32
- i can film a short video of the finished thing working

the video is the real test. if i can't film it working in one take, it isn't done.

---

## the firmware

[`firmware/buddy-mini/buddy-mini.ino`](firmware/buddy-mini/buddy-mini.ino) is all of
v1: the three faces, the idle blink, both buttons with debouncing, both sounds, and
the status led.

build it with:

```
arduino-cli compile --fqbn esp32:esp32:esp32doit-devkit-v1 firmware/buddy-mini
```

compiles clean against esp32 core 3.3.11 — 24% of program storage, 7% of ram.
**compiled, not run.** the kit hasn't arrived, so none of the drawing or timing is
proven on a real panel yet.

---

## the parts

all from the lafvin basic starter kit for esp32.

| part | qty |
|---|---|
| esp32 dev board | 1 |
| 0.96" ssd1306 oled, 128x64, i²c | 1 |
| breadboard | 1 |
| jumper wires | 1 kit |
| tactile push buttons | 2 |
| passive piezo buzzer | 1 |
| leds | a few |
| 220Ω resistors | 1–2 |
| 10kΩ resistors | 0–2 (probably not needed — see doc 02) |
| usb **data** cable | 1 |

---

## the notes

- [01 — scope](docs/01-scope.md) — what v1 is, and what it deliberately isn't
- [02 — parts](docs/02-parts.md) — every component, and what it actually does
- [03 — block diagram](docs/03-block-diagram.md) — how the five parts hang off the esp32
- [04 — pin plan](docs/04-pin-plan.md) — which pins each part gets, and which ones are off-limits
- [05 — interactions and sounds](docs/05-interactions.md) — what each button does and what it sounds like
- [06 — build plan](docs/06-build-plan.md) — one goal per session, toolchain through demo video
- [07 — evidence](docs/07-evidence.md) — what to photograph and save, and when
- [devlog](journal/README.md) — session by session, including what broke

## a note on the two journals

`JOURNAL.md` and `BOM.md` in the root are mirrored from half life and get
overwritten every time it syncs. don't hand-edit them — anything typed there
disappears. my own writing lives in [`journal/`](journal/README.md).

## repo layout

```
buddy-mini/
├── docs/            planning notes, one file per step
├── firmware/        the esp32 sketch
├── journal/         devlog, one file per session
├── media/           diagrams, reference images, build photos
└── README.md
```

## time

hours go through hackatime under project `buddy-mini`. only time actually spent on
this counts.
