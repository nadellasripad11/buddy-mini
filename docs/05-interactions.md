# 05 — button interactions and sounds

*session 1, 2026-09-30*

## what the buzzer can and can't do

a passive piezo is one vibrating disc, not a speaker. it plays **one tone at a
time**. beeps, melodies and sliding sweeps are fine. voices, samples, songs with
layers, or any actual audio clip are not possible.

so the "6-7" sound below is an *impression* of the cadence, not the real audio.
that's the limit of the part, not something i can code around.

## the two sounds

### button 1 — the cool one

a rising sweep, 200hz climbing to 1200hz. sci-fi power-up, `wheeeooop`.
piezos are good at this because sliding a frequency is cheap.

```cpp
void powerUp() {
  for (int f = 200; f <= 1200; f += 25) {
    tone(BUZZER, f, 12);
    delay(6);
  }
  noTone(BUZZER);
}
```

### button 2 — the funny one

two notes doing the 6-7 cadence: a short high blip for "six", then a pitch that
**slides down** for "seh-vennn". the drop is what makes it read as the meme.

```cpp
void sixSeven() {
  tone(BUZZER, 880, 110);                 // "six"
  delay(150);
  for (int f = 784; f >= 400; f -= 12) {  // "seh-vennn"
    tone(BUZZER, f, 10);
    delay(5);
  }
  noTone(BUZZER);
}
```

**both of these are easy to change.** the `f` numbers are pitch, the `delay`
numbers are speed. nothing in the wiring cares what comes out — swapping the
sound is editing two lines.

## the two interactions

| | button 1 (pin 32) | button 2 (pin 33) |
|---|---|---|
| sound | rising sweep | 6-7 |
| face | happy `^ ^` for 2 seconds | double blink, fast |
| after | back to normal | back to normal |

button 2 deliberately **reuses the blink face** instead of needing a fourth
expression. the art already exists, and a fast double-blink paired with the
6-7 slide gives it an attitude.

## debouncing — the thing that will go wrong

the metal contacts inside a tactile button physically chatter for a few
milliseconds when pressed. the esp32 reads that as several presses, so the
sound fires multiple times off one tap.

fix: ignore any press that happens within ~200ms of the last one.

```cpp
const int BTN1 = 32, BTN2 = 33;
unsigned long lastPress1 = 0;
const unsigned long DEBOUNCE = 200;

void loop() {
  if (digitalRead(BTN1) == LOW && millis() - lastPress1 > DEBOUNCE) {
    lastPress1 = millis();
    showHappy();
    powerUp();
  }
  // ... same shape for BTN2
}
```

note `== LOW`. that's the internal pull-up from [04](04-pin-plan.md) showing up
in real code — the pin sits high until the button connects it to ground.

## known limitation to fix later

both sound functions use `delay()`, which **blocks** — the eyes freeze while a
sound plays. the sounds are under 400ms so it's tolerable for v1, but the idle
blink will stutter if a sound is playing.

the proper fix is a non-blocking state machine driven by `millis()` instead of
`delay()`. that's a v2 problem. writing it down now so it doesn't look like i
didn't notice.

## if `tone()` doesn't compile

`tone()` exists in the esp32 arduino core 3.x. on older cores it doesn't, and
you have to drive the buzzer through the LEDC peripheral instead
(`ledcAttach` / `ledcWriteTone`). if the compiler says `tone was not declared
in this scope`, that's which version i'm on.
