# Robot Butterfly

[Robot Butterfly](http://robotzwrrl.xyz/robot-butterfly/) is a companion robot to appreciate the elegance of nature with, while observing all the details — no matter how tiny or inconspicuous!

For more information: [robotzwrrl.xyz/robot-butterfly](http://robotzwrrl.xyz/robot-butterfly/)

Robot Butterfly's design files are released as open source hardware, provided under these unique terms: If you replicate it, please take a photo of it in nature and email it to me with the city and its nickname!

![Robot Butterfly](https://github.com/RobotGrrl/RobotButterfly/blob/main/robot_butterfly.jpg?raw=true)


---

## Electronics

The Butterfly Chonky board connects with [Keystone](https://github.com/RobotGrrl/Keystone) and [Boostlet!](https://github.com/RobotGrrl/boostlet). Those two boards are also open source, and can be found in these repositories:

https://github.com/RobotGrrl/boostlet

https://github.com/RobotGrrl/Keystone

---

## Manufacturing

| Piece                     | Layer height    | Supports                   | Infill | Shells | Notes                                              |
| ------------------------- | --------------- | -------------------------- | ------ | ------ | -------------------------------------------------- |
| enclosure_v1.0.stl        | 0.25 mm DRAFT   | Everwhere, Organic         | 15%    | 2      | \-                                                 |
| neopixel_bracket_v1.0.stl | 0.25 mm DRAFT   | Everwhere, Organic         | 15%    | 3      | Prints standing up                                 |
| battery_bracket_v1.0.stl  | 0.25 mm DRAFT   | Supports to build platform | 15%    | 2      | Hiya!' faces up, Top fill pattern: Octogram Spiral |
| lid_v1.0.stl              | 0.25 mm DRAFT   | None                       | 15%    | 3      | \-                                                 |
| servo_bracket_v1.0.stl    | 0.25 mm DRAFT   | None                       | 15%    | 2      | Prints face down                                   |
| servo_horn_L_v1.0.stl     | 0.20 mm QUALITY | None                       | 20%    | 4      | \-                                                 |
| servo_horn_R_v1.0.stl     | 0.20 mm QUALITY | None                       | 20%    | 4      | \-                                                 |
| cover_v1.0.stl            | 0.25 mm DRAFT   | None                       | 15%    | 2      | Cool!' faces up                                    |
| spacer_back_v1.0.stl      | 0.25 mm DRAFT   | None                       | 15%    | 2      | Chamfer side down                                  |
| spacer_front_v1.0.stl     | 0.25 mm DRAFT   | None                       | 15%    | 2      | Chamfer side down                                  |
| wing_L_v1.0.stl           | 0.25 mm DRAFT   | None                       | 15%    | 3      | Top & bottom fill pattern: Archimedian Chords      |
| wing_R_v1.0.stl           | 0.25 mm DRAFT   | None                       | 15%    | 3      | Top & bottom fill pattern: Archimedian Chords      |

---

## Firmware

Library dependencies:

In the Arduino IDE library manager, search for the following and download the library by the following authours:
- ESP32Servo (by Kevin Harrington and John K. Bennett)
- Streaming (by Mikal Hart)
- Adafruit_NeoPixel (by Adafruit)
- Bounce2 (by Thomas O Fredericks)
- HCSR04 (by Dirk Sarodnick)
- movingAvg (by Jack Christensen)


Here’s how you can upload code to your Robot Butterfly!

1. Download Arduino IDE

2. Tools -> Board -> Board Manager

3. Search for ‘esp32’

4. Install esp32 by Espressif Systems

5. Tools -> Board -> esp32 -> Set board to ESP32 Dev Module

6. Tools -> Set upload speed to 115200 baud (it’s down at the bottom of the menu)

7. Tools -> Set CPU speed to 80 MHz (it’s the 1st setting after a divider in the menu)

8. Tools -> Manage Libraries
There are library dependencies that will need to be downloaded

Search for the following and download the library by the following authours:
ESP32Servo (by Kevin Harrington and John K. Bennett)
Streaming (by Mikal Hart)
Adafruit_NeoPixel (by Adafruit)
Bounce2 (by Thomas O Fredericks)
HCSR04 (by Dirk Sarodnick)
movingAvg (by Jack Christensen)

9. Download this sketch and open it:
https://github.com/RobotGrrl/RobotButterfly/tree/main/firmware/RobotButterflyBasic 

10. Plug in your Robot Butterfly

11. Tools -> Port -> Select the port

12. Upload

13. Congrats! You have uploaded code to your Robot Butterfly!

---

## License

Boostlet! (C) Copyright Erin RobotZwrrl, Robot Missions Inc.

12-30-23 v1.0

[RobotZwrrl.xyz/boostlet](http://RobotZwrrl.xyz/boostlet)

[RobotMissions.org](https://RobotMissions.org)


This hardware is released under:

CERN Open Hardware License Version 2.0 - Permissive (CERN-OHL-P)

[https://cern-ohl.web.cern.ch/](https://cern-ohl.web.cern.ch/)


Disclaimer: Distributed as is. No warranty is given. Not responsible or liable for any damage or harm that result from this product.
