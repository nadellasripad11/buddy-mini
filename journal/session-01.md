# session 01 — planning and parts research

**date:** 2026-09-30
**time:** ~1 hour
**hardware in hand:** none yet — lafvin esp32 starter kit not delivered

---

## what i worked on

no parts yet, so this session was entirely planning and learning. i didn't want the
kit to show up and have me staring at a pile of components with no idea what to do.

**locked down the scope for v1.** wrote out exactly what buddy mini will and won't do
in the warm-up version, and a definition of done i can actually test. the "won't do"
list was the harder half — it's what keeps me from drifting into motors and voice
control before i've gotten a single pixel on a screen.

two things got added after my first draft:
- an **idle blink**, so the eyes blink on their own every few seconds. without it
  buddy is frozen until you touch it, which kills the character feeling.
- a **status led**, matching the little white light in my reference photo.

also decided on **2 buttons** for v1, with breadboard space left for a third later.

**learned what every component actually does.** this was the bulk of the hour. i went
part by part instead of just reading a wiring diagram, because i want the wiring to
make sense later instead of being symbols i copied.

things i didn't know before today:

- the oled uses **i²c**, which is why a whole 128x64 screen only needs two data
  wires (sda and scl) instead of dozens.
- a **passive** buzzer can't make a tone by itself — you feed it a fast switching
  signal and the switching *speed* is the pitch. that's actually better for me,
  because it means different notes, not one flat beep.
- an led without a **220Ω resistor** pulls too much current and can damage the led
  or the esp32 pin.
- a button pin with nothing connected **floats** and reads randomly. a 10kΩ pull-up
  holds it at a clean value — but the esp32 has pull-ups built in that can be
  switched on in software, so i may not need the physical resistors at all.
- on a breadboard, the **edge rails run the full length** (power and ground) while
  the middle rows are short 5-hole strips. two parts in the same strip are the same
  electrical point. this seems like the thing that causes most beginner wiring bugs.
- the usb cable has to be a **data** cable, not charge-only. apparently that's the
  number one reason a board "won't connect."

## what worked

going scope-first. by the time i got to the parts list i already knew why each
component was on it, instead of just owning a box of stuff.

## what didn't

nothing broke, because nothing is built yet. the honest limitation is that this was
all on paper — none of it is verified against real hardware, and i expect at least a
couple of these assumptions to be wrong once i'm actually wiring.

## what's next (session 2)

- block diagram of how everything connects
- pick specific esp32 gpio pins for each component
- design the 3 facial expressions on 128x64 grid paper
- plan the 2 button interactions and 2 sounds

first thing once the kit arrives: get the esp32 blinking its onboard led. nothing
else until that works, because it proves the board, the cable, the drivers, and the
upload process all work. debugging a face on an unproven board would be miserable.

## evidence from this session

- `docs/01-scope.md`
- `docs/02-parts.md`
- this entry

## block diagram

![block diagram](../media/block-diagram.png)

```
esp32
 ├── oled          buddy's face
 ├── button 1      touch input
 ├── button 2      touch input
 ├── buzzer        buddy's voice
 └── status led    "i'm awake"
```

full version with signal directions in [docs/03-block-diagram.md](../docs/03-block-diagram.md).

the useful thing i noticed drawing it: it's a **star, not a chain**. nothing
connects to anything except the esp32, so a broken part can't break the others
and i can unplug things one at a time when debugging. also every single part
needs ground — five parts, five ground wires — which is exactly what the long
breadboard rails are for.

## pin plan + the 3 faces

![pin plan](../media/pin-plan.png)

picked the actual gpio pins: **21 + 22** for the oled, **32 + 33** for the buttons,
**25** for the buzzer, **26** for the led.

the real lesson here was how many esp32 pins i'm *not* allowed to touch. 6–11 are
wired to the chip's internal flash — using one stops the board booting at all.
0, 2, 12 and 15 are "strapping pins," which the esp32 reads the instant it powers
on to decide how to boot, so hanging a button off one can stop it starting or
uploading. 1 and 3 are the usb serial lines. and 34–39 are input-only with no
internal pull-ups, so they'd need physical 10k resistors.

that last one is why i landed on 32 and 33 for the buttons — they have pull-ups
built into the chip, so i can switch them on in code with `INPUT_PULLUP` and skip the
10k resistors entirely. side effect: a pressed button reads low, not high. feels
backwards but that's how pull-ups work.

also sketched the three faces. they all share the same mouth and eye positions —
only the eye *shape* changes. normal is two filled circles, happy is two upward
arcs, blink is two flat lines. that's deliberate: if only the eye shape changes,
a blink is a cheap swap instead of redrawing a whole face.

one thing i'm making myself remember for when the kit arrives: pin *order along the
edge* is different between esp32 boards. i go by the number printed on the board,
not by where it sits in my drawing.

## the two button interactions

**button 1** — happy face for 2 seconds + a rising sweep, 200hz up to 1200hz.
sci-fi power-up noise. **button 2** — the 6-7 cadence: a short high blip for
"six", then a pitch sliding down for "seh-vennn", paired with a fast double
blink.

button 2 reuses the blink face on purpose instead of needing a fourth
expression. the art already exists and a double-blink with the slide gives it
some attitude.

learned the buzzer's actual limit here. a passive piezo is one vibrating disc,
so it plays **one tone at a time** — beeps, melodies, slides. it physically
cannot play a voice or an audio clip, so the 6-7 is an impression of the
cadence, not the real sound. that's the part, not the code.

also read up on **debouncing**. the metal contacts inside a tactile button
chatter for a few milliseconds when pressed, so one tap reads as several
presses and the sound fires multiple times. the fix is ignoring any press
within ~200ms of the last one, using `millis()`.

one thing i already know is wrong: both sound functions use delay(), which
blocks, so the eyes freeze while a sound plays. under 400ms so it's tolerable
for v1, but the idle blink will stutter. the real fix is a non-blocking
millis() state machine. writing it down now so it's clear i noticed rather
than missed it.

## wrote the firmware and got it compiling

turns out the toolchain was already on this machine — arduino-cli 1.5.1, esp32
core 3.3.11, adafruit ssd1306 2.5.17 and gfx 1.12.6. so instead of just testing
whether things installed, i wrote the whole of v1 and compiled it.

```
Sketch uses 317714 bytes (24%) of program storage space.
Global variables use 24004 bytes (7%) of dynamic memory.
```

**the open question is answered: `tone()` compiles fine on core 3.3.11**, so the
buzzer doesn't need the LEDC fallback i was worried about. good thing to know now
rather than with the hardware in front of me at 1am.

the sketch has all three faces, the idle blink on a `millis()` timer, both buttons
with debouncing, both sounds, and the status led. two details i'm happy with:

the mouth and the "^" happy eyes are drawn as parabolas computed pixel by pixel,
because adafruit gfx has no arc primitive i trusted to behave predictably. a short
loop with `y = (x*x)/k` gives an exact curve i can tune.

if the oled doesn't answer at 0x3c, setup() doesn't just silently fail — the
status led blinks fast forever. that way a wiring problem is visible without
plugging into a laptop. worth doing since some ssd1306 boards are at 0x3d.

this is **compiled, not run**. no hardware yet, so none of the drawing or timing
is proven. i fully expect the face positions to need nudging once i see them on
a real 128x64 panel.
