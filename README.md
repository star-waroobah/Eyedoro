## EyeDoro!

[![View PCB on KiCanvas](https://hack.club/pcb-badge)](https://kicanvas.org/?repo=https://github.com/<OWNER>/<REPOSITORY>/tree/main/pcb)
![3d front](assets/3d%20front.png)

Eyedoro is a desk buddy with two robot eyes that works as a Pomodoro study timer. Press the button to start a 25 minute work session. While you work, the eyes look around as if they are watching over you. When time is up, a beep plays and the eyes fall asleep for a 5 minute break, then a lower beep wakes them up.

I made Eyedoro because I wanted a study timer that felt more like a little desk companion instead of just another boring countdown. Sometimes it's hard to stay focused while studying, so I thought having a pair of animated robot eyes watching over my desk would make studying a little more fun.
For Hack Club Half Life Week 4, I wanted to experiment with LED displays. I also wanted to learn more about programming microcontrollers, working 
with buttons and buzzers, and making different components work together.

### How It Works
- **Study Mode (25 Minutes):** Press the button to start a study session. The robot's eyes randomly look left, right, up, and down while occasionally blinking.
- **Break Mode (5 Minutes):** When the study session ends, the buzzer plays a high-pitched alert and the eyes close to sleep.
- **Wake-Up Alert:** After the break ends, a lower-pitched beep plays and the robot wakes up, ready for another study session.
- **Pause/Resume:** Press the button during a session to pause the timer, and press it again to resume.

A Seeed XIAO RP2040 drives two 8x8 MAX7219 LED matrix modules (the eyes) over SPI. A 74AHCT125 level shifter turns the XIAO's 3.3 V signals into the 5 V signals the MAX7219 needs. A button starts and pauses the timer and a passive buzzer plays the alerts.

### Technical Details
- **Microcontroller:** Seeed Studio XIAO RP2040
- **Displays:** 2 × MAX7219 8×8 LED Matrix Modules
- **Communication:** SPI
- **Level Shifter:** 74AHCT125 (3.3V to 5V)
- **Input:** Push Button
- **Audio:** Passive Buzzer
- **Programming Language:** Arduino C++
- **Timer:** `millis()`-based countdown
- **Sound Generation:** `tone()` function

## PCB
![PCB layout](assets/pcb.png)

## Schematic
![Schematic](assets/schematic.png)

## 3D view
![3d front](assets/3d%20front.png)
![3d back](assets/3d%20back.png)

## Bill of materials
- 1x Seeed XIAO RP2040
- 2x single 8x8 MAX7219 module
- 1x 74AHCT125 (SOIC-14)
- 1x 10 uF capacitor (0805)
- 1x 100 nF capacitor (0805)
- 1x push button (6 mm)
- 1x passive buzzer
- 1x 5-pin female header (J1)

## Firmware

The firmware for **Eyedoro** is written in **Arduino C++** and designed to run on the **Seeed Studio XIAO RP2040**.
It uses the **MD_MAX72XX** library and RobotEyes animations to control two MAX7219 8×8 LED matrix displays.

### Firmware Files
| File | Description |
|------|-------------|
| `Eyedoro.ino` | Main Arduino sketch containing the Pomodoro timer, button controls, and buzzer alerts. |
| `MD_RobotEyes.cpp` | Handles the robot eye animations. |
| `MD_RobotEyes.h` | Contains the eye animation class definitions. |
| `MD_RobotEyes_Data.h` | Stores the eye graphics and animation sequences. |

The firmware has been written and reviewed, but it has not yet been tested on the physical hardware. Final compilation, uploading, and hardware testing are still pending.

## Credits
A big thanks to the following projects and communities that helped in making Eyedoro!

- **[Hack Club Half Life](https://halflife.hackclub.com/)** – For organizing the hardware challenge and inspiring me to build something creative using LED displays.
- **[MajicDesigns – MD_MAX72XX](https://github.com/MajicDesigns/MD_MAX72XX)** – For the LED matrix library and original RobotEyes animation example that I used as a starting point.


## Status
Designed in week 4 of Half Life. The build is planned for week 9.
