# Robot Butterfly

[Robot Butterfly](http://robotzwrrl.xyz/robot-butterfly/) is a companion robot to appreciate the elegance of nature with, while observing all the details — no matter how tiny or inconspicuous!

For more information: [robotzwrrl.xyz/robot-butterfly](http://robotzwrrl.xyz/robot-butterfly/)

Robot Butterfly's design files are released as open source hardware, provided under these unique terms: If you replicate it, please take a photo of it in nature and email it to me with the city and its nickname!

![Robot Butterfly](https://github.com/RobotGrrl/RobotButterfly/blob/main/robot_butterfly.jpg?raw=true)

---

## Bug Fix 12-15-24

A 3D printable bugfix is available! The three pieces to print are:

- [Servo Horn Indent L](https://github.com/RobotGrrl/RobotButterfly/blob/main/3dprint/servo_horn_indent_L_v1.0.stl)
- [Servo Horn Indent R](https://github.com/RobotGrrl/RobotButterfly/blob/main/3dprint/servo_horn_indent_R_v1.0.stl)
- [Spacer Back](https://github.com/RobotGrrl/RobotButterfly/blob/main/3dprint/spacer_back_v1.1.stl)

The servo horn piece has been updated to work with the new horns that have an indent, rather than being flat in the previous version. The horn is placed on the side that has the arrow indicator. The fasteners go through holes #1 and #4. See photo below for reference.

![Bugfix 01](https://github.com/RobotGrrl/RobotButterfly/blob/main/documentation/kit_bugfix01.jpg?raw=true)

![Bugfix 02](https://github.com/RobotGrrl/RobotButterfly/blob/main/documentation/kit_bugfix02.jpg?raw=true)

![Bugfix 03](https://github.com/RobotGrrl/RobotButterfly/blob/main/documentation/kit_bugfix03.jpg?raw=true)

The spacer back piece has been updated to have clearance for the chamfer around the switch housing on the enclosure. This provides a cleaner fit and smoother lid installation process.

![Bugfix 04](https://github.com/RobotGrrl/RobotButterfly/blob/main/documentation/kit_bugfix04.jpg?raw=true)

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
| lid_v1.1.stl              | 0.25 mm DRAFT   | None                       | 15%    | 3      | Chamfer side down                                                 |
| servo_bracket_v1.0.stl    | 0.25 mm DRAFT   | None                       | 15%    | 2      | Prints face down                                   |
| servo_horn_indent_L_v1.0.stl     | 0.20 mm QUALITY | None                       | 20%    | 4      | \-                                                 |
| servo_horn_indent_R_v1.0.stl     | 0.20 mm QUALITY | None                       | 20%    | 4      | \-                                                 |
| cover_v1.0.stl            | 0.25 mm DRAFT   | None                       | 15%    | 2      | Cool!' faces up                                    |
| spacer_back_v1.1.stl      | 0.25 mm DRAFT   | None                       | 15%    | 2      | Chamfer side down                                  |
| spacer_front_v1.0.stl     | 0.25 mm DRAFT   | None                       | 15%    | 2      | Chamfer side down                                  |
| wing_L_v1.0.stl           | 0.25 mm DRAFT   | None                       | 15%    | 3      | Top & bottom fill pattern: Archimedian Chords. Print 1st layer at a very slow speed.      |
| wing_R_v1.0.stl           | 0.25 mm DRAFT   | None                       | 15%    | 3      | Top & bottom fill pattern: Archimedian Chords. Print 1st layer at a very slow speed.      |

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
- ESP32Servo (by Kevin Harrington and John K. Bennett)
- Streaming (by Mikal Hart)
- Adafruit_NeoPixel (by Adafruit)
- Bounce2 (by Thomas O Fredericks)
- HCSR04 (by Dirk Sarodnick)
- movingAvg (by Jack Christensen)

9. Download this sketch and open it:
https://github.com/RobotGrrl/RobotButterfly/tree/main/firmware/RobotButterflyBasic 

10. Plug in your Robot Butterfly

11. Tools -> Port -> Select the port

12. Upload

13. Congrats! You have uploaded code to your Robot Butterfly!

---

## License

Robot Butterfly (C) Copyright Erin RobotZwrrl, Robot Missions Inc.

05-30-24 v1.0

[RobotZwrrl.xyz/robot-butterfly](http://RobotZwrrl.xyz/robot-butterfly)

[RobotMissions.org](https://RobotMissions.org)


This hardware is released under:

CERN Open Hardware License Version 2.0 - Permissive (CERN-OHL-P)

[https://cern-ohl.web.cern.ch/](https://cern-ohl.web.cern.ch/)


Disclaimer: Distributed as is. No warranty is given. Not responsible or liable for any damage or harm that result from this product.
