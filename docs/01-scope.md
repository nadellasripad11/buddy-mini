# 01 — scope

*written in session 1, 2026-09-30*

## why i did this first

beginners usually fail at hardware projects for one reason: the project never had a
clear finish line. "make a robot" is not a finish line. "when i press button A, the
face blinks and the buzzer beeps twice" is a finish line. i can test it, i can show
it, and i know when i'm done.

## what buddy mini v1 does

1. shows a simple animated face on a small OLED display
2. reacts when i press its buttons by changing its expression
3. makes different sounds when i interact with it
4. has a simple personality through different faces and reactions
5. blinks on its own every few seconds, even when i'm not touching it
6. has a status LED that shows it's awake

items 5 and 6 got added after the first draft. the idle blink is the single biggest
thing that makes it feel like a character instead of just a screen — without it,
buddy is frozen until you touch it.

## what it does NOT do yet

- move around
- listen to or talk with me
- use a microphone
- use AI or voice recognition
- run on a battery — USB power only
- require soldering — breadboard only
- have a 3D printed case

this list matters as much as the first one. it's what stops me from rabbit-holing
on something that isn't v1.

## decisions made

| decision | choice | why |
|---|---|---|
| number of buttons | 2 | enough for two distinct reactions; breadboard leaves room for a 3rd later |
| power | USB only | no battery management to get wrong |
| assembly | breadboard, no solder | mistakes are free and reversible |
| enclosure | none in v1 | the case is a separate problem from the electronics |

## definition of done

buddy mini v1 is finished when:

- the OLED displays its face
- the face can change between multiple expressions
- the buttons trigger different reactions
- the buzzer makes different sounds
- everything works together on the ESP32
- i can record a short video showing the finished prototype working

the video is the real test. if i can't film it working in one take, it isn't done.
