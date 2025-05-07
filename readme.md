# Robot Butterfly

[Robot Butterfly](http://robotzwrrl.xyz/robot-butterfly/) invites you to experience nature's elegance alongside it, while interacting with the surrounding wilderness! [Available now as a kit](https://robotmissions.org/store/) to build yourself. 

For more information, check out: [robotzwrrl.xyz/robot-butterfly](http://robotzwrrl.xyz/robot-butterfly/). As well, see the [instructions](https://robotzwrrl.xyz/robot-butterfly/instructions/chapter01.php) to build your own!

Robot Butterfly's design files are released as open source hardware, provided under these unique terms: If you replicate it, please take a photo of it in nature and email it to me with the city and its nickname! ☺️

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

- https://github.com/RobotGrrl/boostlet
- https://github.com/RobotGrrl/Keystone


### What do the LEDs mean?

There are a few LEDs that are visible through the windows on the back of the enclosure.

On Chonky, there are two yellow 5 mm LEDs. On the left hand side, is D2. In the firmware, D2 is `LED_COMMS_PIN` (pin 5). On the right hand side, this is D1. In the firmware, D1 is `LED_HEARTBEAT_PIN` (pin 13).

LEDs from Keystone are also visible through the LED ports on the back side of the enclosure. As well, sometimes their brightness reflects in Chonky’s 5 mm LEDs. On the left hand side is power. The power LED is on all the time. On the right hand side are TX and RX. The TX and RX LEDs are activated whenever there is data on the serial port. This can be useful for a visual check, but it also just looks techno cool.

In the inaugural version, all the LEDs are yellow, which can sometimes make it tricky to visually differentiate between Chonky and Keystone's LEDs. This will be changed in a future production revision :)

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

**Firmware is in active development**


### Library Dependencies

In the Arduino IDE library manager, please install the following libraries:

- [Streaming](https://github.com/janelia-arduino/Streaming) - v5
- [movingAvg](https://github.com/JChristensen/movingAvg) - v2.3.2
- [MPU6050](https://github.com/ElectronicCats/mpu6050) - v1.4.1
- [Adafruit Neopixel](https://github.com/adafruit/Adafruit_NeoPixel) - v.1.12.4
- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) - v3.0.6 
- [ServoEasing](https://github.com/ArminJo/ServoEasing) - v3.4.0
- [DHT](https://github.com/adafruit/DHT-sensor-library) - v1.4.6
- [Adafruit Sensor](https://github.com/adafruit/Adafruit_Sensor) - v1.1.15 
- [HCSR04](https://github.com/d03n3rfr1tz3/HC-SR04) - v1.1.3 


### Upload Code

Here's how you to upload code to Robot Butterfly!

1. Download Arduino IDE

2. Tools -> Board -> Board Manager

3. Search for **esp32**

4. Install esp32 by Espressif Systems

5. Tools -> Board -> esp32 -> Set board to **ESP32 Dev Module**

6. Tools -> Set upload speed to **115200 baud** (it’s down at the bottom of the menu)

7. Tools -> Set CPU speed to **80 MHz** (it’s the first setting after a divider in the menu)

8. Tools -> Manage Libraries
There are library dependencies that will need to be downloaded, see list above

9. Open a Robot Butterfly example sketch

10. Plug in Robot Butterfly

11. Tools -> Port -> Select the port

12. Upload

13. Congrats! You have uploaded code to Robot Butterfly!


### Animations

For developers - here's how to make your own animation (servo and neopixel):

1. Add identifier to enum servoAnimName

`SERVO_ANIM_SOARING`

2. Add the run function prototype

`void runServoAnim_soaring(struct ServoAnimation *animation);`

3. Write the run function

`void runServoAnim_soaring(struct ServoAnimation *a) { ... }`

4. Write the init function

`void initServoAnim_soaring(struct ServoAnimation *a) { ... }`

5. Add the case to setServoAnim

`case SERVO_ANIM_SOARING: 
initServoAnim_soaring(a); 
break;`

6. Call the animation to run it!

`setServoAnim(&servo_animation_alert, SERVO_ANIM_SOARING, SERVO_ANIM_ALERT); 
startServoAnim(&servo_animation_alert);`


---

## Debugging

Please see [this page for debugging FAQ](https://robotzwrrl.xyz/robot-butterfly/instructions/debugging.php).


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
