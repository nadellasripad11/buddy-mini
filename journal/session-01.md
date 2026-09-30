# session 01 — planning and parts research

**date:** 2026-09-30
**time:** ~1 hour
**hardware in hand:** none yet — LAFVIN ESP32 starter kit not delivered

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
- a **status LED**, matching the little white light in my reference photo.

also decided on **2 buttons** for v1, with breadboard space left for a third later.

**learned what every component actually does.** this was the bulk of the hour. i went
part by part instead of just reading a wiring diagram, because i want the wiring to
make sense later instead of being symbols i copied.

things i didn't know before today:

- the OLED uses **I²C**, which is why a whole 128x64 screen only needs two data
  wires (SDA and SCL) instead of dozens.
- a **passive** buzzer can't make a tone by itself — you feed it a fast switching
  signal and the switching *speed* is the pitch. that's actually better for me,
  because it means different notes, not one flat beep.
- an LED without a **220Ω resistor** pulls too much current and can damage the LED
  or the ESP32 pin.
- a button pin with nothing connected **floats** and reads randomly. a 10kΩ pull-up
  holds it at a clean value — but the ESP32 has pull-ups built in that can be
  switched on in software, so i may not need the physical resistors at all.
- on a breadboard, the **edge rails run the full length** (power and ground) while
  the middle rows are short 5-hole strips. two parts in the same strip are the same
  electrical point. this seems like the thing that causes most beginner wiring bugs.
- the USB cable has to be a **data** cable, not charge-only. apparently that's the
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
- pick specific ESP32 GPIO pins for each component
- design the 3 facial expressions on 128x64 grid paper
- plan the 2 button interactions and 2 sounds

first thing once the kit arrives: get the ESP32 blinking its onboard LED. nothing
else until that works, because it proves the board, the cable, the drivers, and the
upload process all work. debugging a face on an unproven board would be miserable.

## evidence from this session

- `docs/01-scope.md`
- `docs/02-parts.md`
- this entry
