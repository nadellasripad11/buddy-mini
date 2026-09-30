# 04 — pin plan

*session 1, 2026-09-30*

![pin plan](../media/pin-plan.png)

## the plan

| part | connection | esp32 pin |
|---|---|---|
| oled | sda (data) | **21** |
| oled | scl (clock) | **22** |
| oled | vcc | 3v3 |
| oled | gnd | gnd |
| button 1 | one leg | **32** |
| button 1 | other leg | gnd |
| button 2 | one leg | **33** |
| button 2 | other leg | gnd |
| buzzer | + | **25** |
| buzzer | − | gnd |
| status led | anode (long leg) | **26** → 220Ω → led |
| status led | cathode (short leg) | gnd |

so: **21, 22, 32, 33, 25, 26**. six pins, plus 3v3 and a bunch of grounds.

## pins i am not allowed to use

this was the actual lesson of this step. the esp32 has ~30 pins but a lot of them
are already spoken for:

| pins | why they're off-limits |
|---|---|
| 6, 7, 8, 9, 10, 11 | wired to the chip's internal flash memory. using them stops the board booting. never touch. |
| 0, 2, 12, 15 | "strapping pins" — the esp32 reads their voltage *at the instant it powers on* to decide how to boot. put a button or led on one and the board may refuse to start or fail to upload. |
| 1, 3 | the usb serial lines (tx/rx). using them fights with uploading code. |
| 34, 35, 36, 39 | input-only, and they have **no internal pull-up resistors**. they'd work for buttons but only with physical 10kΩ resistors soldered in. avoidable hassle. |

what's left and safe: **4, 5, 13, 14, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33**.
everything i picked came from that list.

## why these specific ones

- **21 and 22 for the oled** — these are the esp32's default i²c pins. the wire
  library assumes them unless told otherwise, so using them means less to configure
  and fewer things to get wrong.
- **32 and 33 for the buttons** — safe, side by side on the board, and they have
  internal pull-up resistors. that means **no 10kΩ resistors needed** — i switch the
  pull-ups on in code with `INPUT_PULLUP` and the pin reads a clean high when the
  button isn't pressed, low when it is. pressed = low feels backwards but that's how
  pull-ups work.
- **25 for the buzzer** — free, safe, and supports pwm, which is how a passive
  buzzer gets told what pitch to play.
- **26 for the led** — right next to 25, keeps the output stuff together on one side
  of the breadboard.

## wiring notes for when the kit arrives

- **the button's 4 legs are not 4 separate contacts.** they're two pairs, already
  connected internally. to be safe, use **diagonally opposite** legs — those are
  always the pair that the switch actually opens and closes.
- **the led's long leg is +.** long leg → resistor → pin 26. short leg → gnd.
  backwards and it just won't light (it won't break).
- **the 220Ω resistor goes on the led, not the buzzer.** the buzzer doesn't need one.
- **all the grounds go to the breadboard's − rail**, and one wire runs from that rail
  back to an esp32 gnd pin. five parts sharing one ground rail, not five wires
  crammed into the board.

## important caveat

pin *order along the edge of the board* differs between esp32 models. when wiring,
i go by **the number printed on the board itself**, not by where it sits in any
diagram. gpio 21 is gpio 21 regardless of which hole it's in.

## the 3 faces

drawn on the 128x64 screen. all three share the same mouth and eye positions — only
the eye *shape* changes, which is what makes a blink cheap to animate.

| face | eyes | when |
|---|---|---|
| **normal** | two filled circles | idle / resting state |
| **happy** | two upward arcs `^ ^` | button 1 pressed |
| **blink** | two flat lines `— —` | automatically, every few seconds |

the blink is the important one. it runs on a timer with nothing triggering it, and
it's the whole reason buddy reads as alive instead of as a screen showing a picture.
