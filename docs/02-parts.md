# 02 — parts and what they do

*written in session 1, 2026-09-30*

## why i did this

my parts are a pile of stuff in a box. if i don't know what each piece does, wiring
is going to feel like copying magic symbols off the internet. once i know each part's
job, the wiring diagram makes obvious sense instead of being memorization.

## two words i needed first

**GPIO pin** — the ESP32 has a row of metal pins down both sides. most are "general
purpose input/output." each one can either *listen* (input — "is the button pressed?")
or *talk* (output — "turn this LED on"). every pin has a number printed next to it.
in code i say "pin 4" and it means that specific metal leg.

**voltage / ground** — electricity needs a complete loop. power flows out of a 3.3V
pin, through the component, and back into a GND pin. if the loop isn't closed,
nothing happens. apparently that's most beginner debugging.

---

## the parts

### ESP32 dev board — the brain
a tiny computer. runs my code forever in a loop, reads the buttons, tells the screen
and buzzer what to do. it also supplies 3.3V and GND to everything else on the
breadboard, drawn from the USB cable. it's the only smart part — everything else is
dumb and does what the ESP32 says.

### SSD1306 OLED 128x64 I²C — buddy's face
a small screen, 128 pixels across and 64 down, where i draw the eyes and mouth.
OLED means each pixel makes its own light, so black is truly black. that's why the
face in my reference photo glows against pure black.

**I²C** is the language it speaks. instead of needing 64 wires, I²C uses two data
wires: SDA (data) and SCL (clock). plus power and ground = 4 wires total for a whole
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
breadboard. male-to-female goes from a module with pin headers (like the OLED) to
the breadboard.

### tactile push buttons — buddy's touch sense
a tiny 4-leg switch. pressing it connects two of its legs, and the ESP32 notices the
pin change. the legs come in pairs that are *already* connected internally, which
trips up beginners — have to watch for that when wiring.

### passive piezo buzzer — buddy's voice
a small disc that vibrates to make sound. **passive** matters: it doesn't know how to
make a tone on its own, so i feed it a rapidly switching signal and the *speed* of the
switching is the pitch. that's good news — it means different notes and melodies are
possible, not just one flat beep. an active buzzer only does one fixed tone.

### LEDs — the status light
a tiny light with a long leg (+, anode) and a short leg (−, cathode). direction
matters — backwards and it just won't light. this is the white glow under the face
in my reference photo.

### resistors — the safety parts
a resistor limits how much current flows. two jobs here:

- **220Ω** goes in series with the LED. without it the LED pulls too much current and
  burns out, and it can also stress the ESP32 pin.
- **10kΩ** is a pull-up/pull-down for buttons. when a button *isn't* pressed its pin
  isn't connected to anything and the reading flickers randomly between on and off
  ("floating"). the resistor holds it at a known value so it reads a clean OFF.

the ESP32 has pull-up resistors built in and they can be switched on in software,
so i may not physically need the 10kΩ ones. will try both ways.

### USB data cable — power + code upload
carries the program from my laptop to the ESP32 and supplies 5V power.
**must be a data cable, not charge-only.** charge-only cables are apparently the #1
reason "my board won't connect."

---

## one-line summary

| part | job |
|---|---|
| ESP32 | the brain — runs the code, powers everything |
| OLED display | buddy's face |
| breadboard | holds parts and connects them without solder |
| jumper wires | the actual connections |
| 2x push button | buddy's sense of touch |
| passive buzzer | buddy's voice |
| LED | status light — "i'm awake" |
| 220Ω resistor | protects the LED from burning out |
| 10kΩ resistor | keeps button readings clean (may be optional) |
| USB data cable | power + uploading code |

---

## check questions i answered

**why does the OLED only need 2 data wires instead of many?**
the OLED uses I²C, so it only needs SDA and SCL for data.

**what happens if you connect an LED without a resistor?**
too much current can flow through the LED and damage it, or damage the ESP32 pin.

**what makes a passive buzzer better here than an active one?**
a passive buzzer lets the ESP32 control different tones, so buddy can make more sounds.
