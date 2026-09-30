# 07 — what to capture as evidence

*session 1, 2026-09-30*

half life wants proof of real work. this is what to actually save, and when.

## every session

- **a commit.** the git history is the strongest evidence there is — it's
  timestamped, it's incremental, and it can't be faked after the fact.
- **a journal entry** covering what worked, what didn't, and what i learned. the
  "what didn't" half is what makes it read as real.
- **hackatime** running. the hook logs against project `buddy-mini` automatically.

## photos — take these, not beauty shots

| when | what | why it matters |
|---|---|---|
| kit arrives | parts laid out on the desk, still in bags | timestamps when i actually got the hardware |
| session 3 | onboard led lit, usb plugged in | proves the board works |
| session 4 | first thing drawn on the oled, even if it's one rectangle | the moment the screen came alive |
| session 4 | the i²c scanner output in the serial monitor | shows real debugging, not a copied tutorial |
| session 5 | the face on screen, close up | this is the project's identity shot |
| any session | **the breadboard when something is wrong** | see below |
| session 8 | finished breadboard, wires tidy | the "done" photo |

## screenshots

- arduino ide **compiling successfully** the first time
- the **serial monitor** showing i²c scanner output
- any **error message** that cost real time — the red text in the ide

## the one people skip

**photograph the failures.** the wiring that didn't work, the blank screen, the
error message, the garbled face. then photograph it working after the fix.

a log that's all successes looks fabricated. a log with "the screen stayed black
for 40 minutes because my oled was at 0x3d not 0x3c" is obviously real, and it's
the more interesting story anyway.

## the video

one take, ~30 seconds, at the end:

1. buddy sitting there, eyes blinking on their own
2. press button 1 → happy face + sweep sound
3. press button 2 → double blink + the 6-7 sound
4. hold on the idle face for a few seconds

no cuts. cuts look like the parts between them didn't work.

## where it all lives

- photos and video → `media/`, named `sNN-description.jpg`
- notes → `journal/session-NN.md`
- the half life platform copy → `JOURNAL.md` (auto-synced, never edit by hand)
