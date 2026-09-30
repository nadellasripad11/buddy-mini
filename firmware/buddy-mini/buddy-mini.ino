// buddy mini — v1
//
// a small desk buddy: animated oled face, two buttons, a buzzer and a status led.
// runs on an esp32 devkit v1, usb powered, everything on a breadboard.
//
// pins (see docs/04-pin-plan.md):
//   oled sda -> 21      oled scl -> 22      oled vcc -> 3v3
//   button 1 -> 32      button 2 -> 33      (internal pull-ups, a press reads LOW)
//   buzzer + -> 25      status led -> 26 through a 220 ohm resistor

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- pins ----------
const int PIN_SDA    = 21;
const int PIN_SCL    = 22;
const int PIN_BTN1   = 32;
const int PIN_BTN2   = 33;
const int PIN_BUZZER = 25;
const int PIN_LED    = 26;

// ---------- screen ----------
const int  SCREEN_W    = 128;
const int  SCREEN_H    = 64;
const byte OLED_ADDR   = 0x3C;   // run an i2c scanner first — some boards are 0x3D
Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

// ---------- face geometry ----------
// all three faces share these positions. only the eye shape changes, which is
// what makes a blink cheap — no full redraw of a different layout.
const int EYE_L   = 42;
const int EYE_R   = 86;
const int EYE_Y   = 24;
const int EYE_R_PX = 9;
const int MOUTH_Y = 48;

enum Face { FACE_NORMAL, FACE_HAPPY, FACE_BLINK };

// ---------- timing ----------
const unsigned long DEBOUNCE      = 200;   // ignore repeat presses inside this
const unsigned long BLINK_SHUT_MS = 140;   // how long the eyes stay closed
const unsigned long HAPPY_HOLD_MS = 2000;  // how long button 1 holds the happy face

unsigned long lastPress1 = 0;
unsigned long lastPress2 = 0;
unsigned long happyUntil = 0;
unsigned long blinkAt    = 0;              // when the next idle blink starts
unsigned long blinkEnds  = 0;              // when the current blink ends, 0 if not blinking
Face          shown      = FACE_NORMAL;    // what is currently on the screen

// ---------- drawing ----------

// a smile: a shallow parabola, lowest in the middle. drawn two pixels thick so it
// reads clearly on a 128x64 panel.
void drawMouth(bool big) {
  const int half = big ? 24 : 18;
  const int k    = big ? 34 : 30;
  for (int x = -half; x <= half; x++) {
    int y = MOUTH_Y - (x * x) / k;
    display.drawPixel(64 + x, y, SSD1306_WHITE);
    display.drawPixel(64 + x, y - 1, SSD1306_WHITE);
  }
}

// an upward arc, peak in the middle — the happy "^" eye.
void drawArcEye(int cx) {
  for (int x = -13; x <= 13; x++) {
    int y = EYE_Y - 7 + (x * x) / 11;
    display.drawPixel(cx + x, y, SSD1306_WHITE);
    display.drawPixel(cx + x, y + 1, SSD1306_WHITE);
  }
}

void drawEyes(Face f) {
  switch (f) {
    case FACE_NORMAL:
      display.fillCircle(EYE_L, EYE_Y, EYE_R_PX, SSD1306_WHITE);
      display.fillCircle(EYE_R, EYE_Y, EYE_R_PX, SSD1306_WHITE);
      break;
    case FACE_HAPPY:
      drawArcEye(EYE_L);
      drawArcEye(EYE_R);
      break;
    case FACE_BLINK:
      display.fillRect(EYE_L - 12, EYE_Y - 2, 24, 4, SSD1306_WHITE);
      display.fillRect(EYE_R - 12, EYE_Y - 2, 24, 4, SSD1306_WHITE);
      break;
  }
}

void showFace(Face f) {
  display.clearDisplay();
  drawEyes(f);
  drawMouth(f == FACE_HAPPY);
  display.display();
  shown = f;
}

// ---------- sounds ----------
// tuned in the browser bench before the hardware arrived. the numbers are pitch
// in hz and duration in ms — change them freely, nothing else depends on them.

void powerUp() {                              // button 1 — rising sci-fi sweep
  for (int f = 200; f <= 1200; f += 25) {
    tone(PIN_BUZZER, f, 12);
    delay(6);
  }
  noTone(PIN_BUZZER);
}

void sixSeven() {                             // button 2 — the 6-7 cadence
  tone(PIN_BUZZER, 880, 110);                 // "six"
  delay(110);
  noTone(PIN_BUZZER);
  delay(40);
  for (int f = 784; f >= 400; f -= 12) {      // "seh-vennn", sliding down
    tone(PIN_BUZZER, f, 10);
    delay(5);
  }
  noTone(PIN_BUZZER);
}

// ---------- idle blink ----------
// runs on millis() so the eyes keep blinking without anything triggering them.
// this is what makes buddy read as alive rather than as a screen showing a picture.

void scheduleBlink() {
  blinkAt = millis() + random(2500, 5200);
}

void idleBlink() {
  unsigned long now = millis();

  if (blinkEnds != 0) {                       // currently mid-blink
    if (now >= blinkEnds) {
      showFace(FACE_NORMAL);
      blinkEnds = 0;
      scheduleBlink();
    }
    return;
  }

  if (now >= blinkAt) {
    showFace(FACE_BLINK);
    blinkEnds = now + BLINK_SHUT_MS;
  } else if (shown != FACE_NORMAL) {
    showFace(FACE_NORMAL);
  }
}

// a quick double blink, used as button 2's reaction.
void doubleBlink() {
  for (int i = 0; i < 2; i++) {
    showFace(FACE_BLINK);
    delay(90);
    showFace(FACE_NORMAL);
    delay(90);
  }
}

// ---------- setup / loop ----------

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  Wire.begin(PIN_SDA, PIN_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    // if this fires, the screen is at a different i2c address or isn't wired right.
    // the status led blinks fast forever so the failure is visible without a laptop.
    Serial.println(F("no ssd1306 at 0x3C — run an i2c scanner"));
    while (true) {
      digitalWrite(PIN_LED, HIGH);
      delay(120);
      digitalWrite(PIN_LED, LOW);
      delay(120);
    }
  }

  digitalWrite(PIN_LED, HIGH);                // awake
  randomSeed(esp_random());

  showFace(FACE_NORMAL);
  scheduleBlink();
}

void loop() {
  unsigned long now = millis();

  // button 1 — happy face plus the rising sweep
  if (digitalRead(PIN_BTN1) == LOW && now - lastPress1 > DEBOUNCE) {
    lastPress1 = now;
    showFace(FACE_HAPPY);
    powerUp();
    happyUntil = millis() + HAPPY_HOLD_MS;
    blinkEnds  = 0;
  }

  // button 2 — double blink plus the 6-7 sound
  if (digitalRead(PIN_BTN2) == LOW && now - lastPress2 > DEBOUNCE) {
    lastPress2 = now;
    doubleBlink();
    sixSeven();
    happyUntil = 0;
    blinkEnds  = 0;
    scheduleBlink();
  }

  // holding an expression takes priority over the idle blink
  if (millis() < happyUntil) {
    if (shown != FACE_HAPPY) showFace(FACE_HAPPY);
    return;
  }

  idleBlink();
}
