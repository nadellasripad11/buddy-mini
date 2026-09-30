# 02 — parts and what they do

*written in session 1, 2026-09-30*

## why i did this

my parts are a pile of stuff in a box. if i don't know what each piece does, wiring
is going to feel like copying magic symbols off the internet. once i know each part's
job, the wiring diagram makes obvious sense instead of being memorization.

## two words i needed first

**gpio pin** — the esp32 has a row of metal pins down both sides. most are "general
purpose input/output." each one can either *listen* (input — "is the button pressed?")
or *talk* (output — "turn this led on"). every pin has a number printed next to it.
in code i say "pin 4" and it means that specific metal leg.

**voltage / ground** — electricity needs a complete loop. power flows out of a 3.3v
pin, through the component, and back into a gnd pin. if the loop isn't closed,
nothing happens. apparently that's most beginner debugging.

---

## the parts

### esp32 dev board — the brain
a tiny computer. runs my code forever in a loop, reads the buttons, tells the screen
and buzzer what to do. it also supplies 3.3v and gnd to everything else on the
breadboard, drawn from the usb cable. it's the only smart part — everything else is
dumb and does what the esp32 says.

### ssd1306 oled 128x64 i²c — buddy's face
a small screen, 128 pixels across and 64 down, where i draw the eyes and mouth.
oled means each pixel makes its own light, so black is truly black. that's why the
face in my reference photo glows against pure black.

**i²c** is the language it speaks. instead of needing 64 wires, i²c uses two data
wires: sda (data) and scl (clock). plus power and ground = 4 wires total for a whole
screen. that's why this display was the right pick.

### breadboard — wiring with no soldering
a plastic block full of holes with metal clips inside. push a wire in, it grips.
the key fact: **holes are connected in groups.** the long rails along the edges
(marked + and −) run the full length and carry power and ground. the middle rows run
in short strips of 5 holes. two components in the same 5-hole strip are electrically
the same point.

understanding that one fact is basically the whole skill of breadboarding.

### jumper wires — the connections
male ends are pins, female ends are sockets. male-to-male goes breadboard-to-
breadboard. male-to-female goes from a module with pin headers (like the oled) to
the breadboard.

### tactile push buttons — buddy's touch sense
a tiny 4-leg switch. pressing it connects two of its legs, and the esp32 notices the
pin change. the legs come in pairs that are *already* connected internally, which
trips up beginners — have to watch for that when wiring.

### passive piezo buzzer — buddy's voice
a small disc that vibrates to make sound. **passive** matters: it doesn't know how to
make a tone on its own, so i feed it a rapidly switching signal and the *speed* of the
switching is the pitch. that's good news — it means different notes and melodies are
possible, not just one flat beep. an active buzzer only does one fixed tone.

### leds — the status light
a tiny light with a long leg (+, anode) and a short leg (−, cathode). direction
matters — backwards and it just won't light. this is the white glow under the face
in my reference photo.

### resistors — the safety parts
a resistor limits how much current flows. two jobs here:

- **220Ω** goes in series with the led. without it the led pulls too much current and
  burns out, and it can also stress the esp32 pin.
- **10kΩ** is a pull-up/pull-down for buttons. when a button *isn't* pressed its pin
  isn't connected to anything and the reading flickers randomly between on and off
  ("floating"). the resistor holds it at a known value so it reads a clean off.

the esp32 has pull-up resistors built in and they can be switched on in software,
so i may not physically need the 10kΩ ones. will try both ways.

### usb data cable — power + code upload
carries the program from my laptop to the esp32 and supplies 5v power.
**must be a data cable, not charge-only.** charge-only cables are apparently the #1
reason "my board won't connect."

---

## one-line summary

| part | job |
|---|---|
| esp32 | the brain — runs the code, powers everything |
| oled display | buddy's face |
| breadboard | holds parts and connects them without solder |
| jumper wires | the actual connections |
| 2x push button | buddy's sense of touch |
| passive buzzer | buddy's voice |
| led | status light — "i'm awake" |
| 220Ω resistor | protects the led from burning out |
| 10kΩ resistor | keeps button readings clean (may be optional) |
| usb data cable | power + uploading code |

---

## check questions i answered

**why does the oled only need 2 data wires instead of many?**
the oled uses i²c, so it only needs sda and scl for data.

**what happens if you connect an led without a resistor?**
too much current can flow through the led and damage it, or damage the esp32 pin.

**what makes a passive buzzer better here than an active one?**
a passive buzzer lets the esp32 control different tones, so buddy can make more sounds.
