// Eyedoro - robot eyes Pomodoro timer
// Board: Seeed XIAO RP2040, two 8x8 MAX7219 LED matrix modules (the eyes)
//
// Based on the RobotEyes example from the MD_MAX72XX library by MajicDesigns:
// https://github.com/MajicDesigns/MD_MAX72XX
//
// Work time: the eyes glance around. Break time: the eyes sleep.

#include <MD_MAX72xx.h>
#include <SPI.h>
#include "MD_RobotEyes.h"

// ----- Pins and hardware -----
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW   // confirm when the board is built
#define MAX_DEVICES 2                          // two eyes
#define CS_PIN D7
#define BUTTON_PIN D3
#define BUZZER_PIN D1

// ----- Timer settings -----
bool testMode = false;     // true = short times for testing, false = real times
unsigned long workTime;
unsigned long breakTime;

MD_MAX72XX M = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_RobotEyes E;

// ----- Things the program remembers -----
int state = 0;                  // 0 = ready, 1 = work, 2 = break
unsigned long startTime = 0;    // when the work or break started
unsigned long nextLook = 0;     // when the eyes glance next
int sessions = 0;               // finished work sessions
int lastButton = HIGH;
unsigned long lastPress = 0;

// ----- Small jobs -----
void startWork() {
  state = 1;
  startTime = millis();
  nextLook = millis() + random(3000, 8000);
}

void startBreak() {
  state = 2;
  startTime = millis();
  E.setAutoBlink(false);                               // stop random blinking
  E.setAnimation(MD_RobotEyes::E_BLINK, false);        // eyes close and stay closed
}

void goReady() {
  if (state == 2) {
    E.setAnimation(MD_RobotEyes::E_BLINK, false, true); // eyes open again
  }
  state = 0;
  E.setAutoBlink(true);
}

void lookAround() {
  int pick = random(0, 5);   // a number from 0 to 4
  if (pick == 0) E.setAnimation(MD_RobotEyes::E_LOOK_L, true);
  if (pick == 1) E.setAnimation(MD_RobotEyes::E_LOOK_R, true);
  if (pick == 2) E.setAnimation(MD_RobotEyes::E_LOOK_U, true);
  if (pick == 3) E.setAnimation(MD_RobotEyes::E_LOOK_D, true);
  if (pick == 4) E.setAnimation(MD_RobotEyes::E_SCAN_LR, true);
  nextLook = millis() + random(3000, 8000);
}

// ----- Runs once at the start -----
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  randomSeed(analogRead(A0));

  M.begin();
  E.begin(&M);
  M.control(MD_MAX72XX::INTENSITY, 2);   // low brightness for USB power
  E.setAutoBlink(true);

  if (testMode) {
    workTime = 10000;     // 10 seconds
    breakTime = 5000;     // 5 seconds
  } else {
    workTime = 25UL * 60UL * 1000UL;   // 25 minutes
    breakTime = 5UL * 60UL * 1000UL;   // 5 minutes
  }
}

// ----- Runs again and again -----
void loop() {
  E.runAnimation();   // keeps the eyes moving

  // Button: LOW means pressed. 200 ms wait so one press counts once.
  int button = digitalRead(BUTTON_PIN);
  if (button == LOW && lastButton == HIGH && millis() - lastPress > 200) {
    lastPress = millis();
    if (state == 0) {
      startWork();
    } else {
      goReady();     // cancel
    }
  }
  lastButton = button;

  // While working: glance around now and then
  if (state == 1 && millis() > nextLook) {
    lookAround();
  }

  // Work time is over
  if (state == 1 && millis() - startTime >= workTime) {
    sessions = sessions + 1;
    tone(BUZZER_PIN, 880, 150);
    delay(200);
    tone(BUZZER_PIN, 1320, 250);
    delay(300);
    startBreak();
  }

  // Break time is over
  if (state == 2 && millis() - startTime >= breakTime) {
    tone(BUZZER_PIN, 440, 400);
    delay(450);
    goReady();
  }
}
