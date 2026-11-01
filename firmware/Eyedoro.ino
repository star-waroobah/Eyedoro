
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "MD_RobotEyes.h"

// 1. Hardware settings
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 2
#define CS_PIN D7
#define BUTTON_PIN D3
#define BUZZER_PIN D1

// Change to true for 10-second testing
const bool testMode = false;

const unsigned long WORK_TIME =
  testMode ? 10000UL : 25UL * 60UL * 1000UL;

const unsigned long BREAK_TIME =
  testMode ? 5000UL : 5UL * 60UL * 1000UL;

MD_MAX72XX M(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_RobotEyes E;

// 2. Pomodoro timer
enum TimerState { READY, WORK, BREAK };

TimerState state = READY;
bool paused = false;

unsigned long timeRemaining = WORK_TIME;
unsigned long lastTick = 0;
unsigned long nextLook = 0;
unsigned int sessions = 0;

// 3. Button debounce
const unsigned long DEBOUNCE_MS = 40;

int rawButtonLast = HIGH;
int stableButton = HIGH;
unsigned long buttonChangedAt = 0;

// 4. Start 25-minute work session
void startWork(unsigned long now) {
  state = WORK;
  paused = false;
  timeRemaining = WORK_TIME;
  lastTick = now;

  nextLook = now + random(3000, 8000);

  E.setAutoBlink(true);
  E.setAnimation(MD_RobotEyes::E_NEUTRAL, false);

  Serial.println("WORK STARTED");
}

// 5. Work complete: high beep and sleeping eyes
void startBreak(unsigned long now) {
  state = BREAK;
  paused = false;
  timeRemaining = BREAK_TIME;
  lastTick = now;

  E.setAutoBlink(false);
  E.setAnimation(MD_RobotEyes::E_BLINK, false);

  tone(BUZZER_PIN, 880, 140);
  delay(190);

  tone(BUZZER_PIN, 1320, 240);
  delay(270);
  noTone(BUZZER_PIN);

  lastTick = millis();

  Serial.println("WORK DONE - BREAK STARTED");
}

// 6. Break complete: low beep and wake up
void finishBreak() {
  tone(BUZZER_PIN, 440, 400);
  delay(420);
  noTone(BUZZER_PIN);

  state = READY;
  paused = false;
  timeRemaining = WORK_TIME;

  E.setAnimation(MD_RobotEyes::E_BLINK, false, true);
  E.setAutoBlink(true);

  Serial.println("BREAK DONE - READY");
}

// 7. Random robot eye movement
void lookAround(unsigned long now) {
  switch (random(0, 5)) {
    case 0:
      E.setAnimation(MD_RobotEyes::E_LOOK_L, true);
      break;
    case 1:
      E.setAnimation(MD_RobotEyes::E_LOOK_R, true);
      break;
    case 2:
      E.setAnimation(MD_RobotEyes::E_LOOK_U, true);
      break;
    case 3:
      E.setAnimation(MD_RobotEyes::E_LOOK_D, true);
      break;
    default:
      E.setAnimation(MD_RobotEyes::E_SCAN_LR, true);
      break;
  }

  nextLook = now + random(3000, 8000);
}

// 8. Button: Start / Pause / Resume
void handleButton(unsigned long now) {
  int raw = digitalRead(BUTTON_PIN);

  if (raw != rawButtonLast) {
    buttonChangedAt = now;
  }

  if (now - buttonChangedAt >= DEBOUNCE_MS &&
      raw != stableButton) {

    stableButton = raw;

    if (stableButton == LOW) {
      if (state == READY) {
        startWork(now);
      }
      else {
        paused = !paused;
        lastTick = now;

        Serial.println(paused ? "PAUSED" : "RESUMED");
      }
    }
  }

  rawButtonLast = raw;
}

// 9. Countdown logic
void updateTimer(unsigned long now) {
  if (state == READY || paused) return;

  unsigned long elapsed = now - lastTick;
  lastTick = now;

  if (elapsed >= timeRemaining) {
    timeRemaining = 0;

    if (state == WORK) {
      sessions++;
      startBreak(now);
    }
    else {
      finishBreak();
    }
  }
  else {
    timeRemaining -= elapsed;
  }
}

// 10. Arduino setup
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  randomSeed(analogRead(A0));

  M.begin();
  E.begin(&M);

  M.control(MD_MAX72XX::INTENSITY, 2);

  E.setAutoBlink(true);
  E.setAnimation(MD_RobotEyes::E_NEUTRAL, false);

  Serial.println("EYEDORO READY");
  Serial.println("PRESS BUTTON TO START");
}

// 11. Arduino main loop
void loop() {
  unsigned long now = millis();

  E.runAnimation();
  handleButton(now);
  updateTimer(now);

  if (state == WORK &&
      !paused &&
      (long)(now - nextLook) >= 0) {
    lookAround(now);
  }
}
