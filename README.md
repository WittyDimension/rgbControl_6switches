---
Title: RGB-LED_Control
Description: Control of RGB LED and battery percentage identification using 6 switches
Author: WittyDimension
Tags: electronics, rgb, led, rgb_led control
Created:  October 24, 2021
---

<!-- This content will not appear in the rendered Markdown -->

RGB-LED_Control
===============
## Control of RGB LED and battery percentage identification using 6 switches


### Introduction

An RGB LED is a combination of three different kinds of LED in one package, namely,
* **Red** LED,
* **Green** LED,
* **Blue** LED.

In contrast to a normal LED that has 2(two)-terminals. An RGB LED has 4(four) terminals, of which three terminals represents three different colours- `RED`, `GREEN` & `BLUE`. And, the 4<sup>th</sup>(fourth) one represents the `GND` terminal. By combining these three colours, we can produce around 16 million hues of light.

[![Generic RGB LED](https://github.com/WittyDimension/rgbControl_6switches/raw/main/images/rgb-led.png)](#features)
*`Fig:` Pic of an **RGB** LED (common Cathode)*

___________________

### Problem Statement

Imagine You have the following parts with you - 
1.  An Arduino Uno `3.3V`, 
2.  You have 6 tactile switches,
3.  One RGB LED (Analog),
4.  One battery management chip.  The Battery management chip provides you with an analog value indicating 
the percentage of the battery (`0V` for `0%` ~ `1.8V` for `100%`). 

Design schematic and an Arduino code so that the circuit can do the following function. Assume you have 
access to any passive component, LDO. If any information is missing (e.g., pinout of the battery management chip) 
make educated assumptions.
 
1.  Take power from a battery and read the `% age` of the battery also.
2.  One switch puts the entire system to `sleep`, turns off the LED.
3.  One Switch wakes up the system from `Sleep`.
4.  One Switch to select the LED Color (`Red`, `Blue`, `Green`).
5.  Two switch one to reduce and increase the intensity.
6.  One switch to show the `% age` of battery in any creative way possible you can think of through the LED.

The Schematic can be on `Paper` or on `E-CAD` software.

___________________

### Rules To Follow

You will be graded on:
1. Design of Circuit considering good practices.
2. Code clarity, commenting, formatting.

`Hint`: An example of good practices → A circuit which avoids floating pins, debounce methods.

__________________________

### Design Thinking Process

Generally, Arduino requires 7-12V to operate optimally on power.
For our case, the BMS chip was mentioned as `1.8V`. So, to achieve an operating voltage within that range we can do the mathematical calculation as,

                                                 1.8V * 4 = 7.2V 

That makes our battery power to be `7.2 V` which is well within the desired range :v:.

Here the Analog Pin `A0` is used to read the battery percentage value. The circuit diagram is drawn and tested on a breadboard using the [TinkedCAD](https://www.tinkercad.com/) platform.



[![My Designed Circuit in TinkerCAD](https://github.com/WittyDimension/rgbControl_6switches/raw/main/images/ckt_diagram.png)](#features)
The circuit diagram is generated using [TinkerCAD](https://www.tinkercad.com/).

___________________

### Installation

There is nothing to install there. Just download the file and upload it to your Arduino Uno. Connect the circuit according to the circuit diagram(*above*). And Voila! :v: It works.

**Enjoy Learning** `&` **don't forget to star the repository**.
