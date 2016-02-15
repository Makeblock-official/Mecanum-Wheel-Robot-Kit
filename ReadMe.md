# Mecanum-Wheel-Robot-Kit

__Kit page:__ http://www.makeblock.cc/mecanum-wheel-robot-kit/

__Profile page:__ http://learn.makeblock.cc/mecanumbot/



### What is Mecanum Wheel Robot Kit?

Mecanum Wheel Robot Kit is a type of mobile chassis robot based on Makeblock open source hardware. The chassis of this kit consists of mecanum wheel, encoder motor driver, mechanical structure and other electronic modules (e.g. main controller). This robot kit is for projects that move in narrow space or projects that need to move in all directions. 

This mecanum wheel chassis is a two-in-one base which allows you to build into two different sizes.

![](http://i.imgur.com/k4uiB0y.png)

![](http://i.imgur.com/wI6nGds.png)

__Video:__ https://www.youtube.com/watch?v=FpP2oDS91r8&feature=youtu.be

![](http://i.imgur.com/E9mKWFc.gif)

![](http://i.imgur.com/mTwqBIg.gif)



### Specifications

1.	Frame: Anodized Aluminum

2.	Power Supply: DC 12V

3.	Main Controller: Makeblock Orion (Arduino UNO compatible) 

4.	Software: Arduino

5.	Connection: 2.4G Wireless Serial

6.	Color: Blue

7.	Speed: 1 m/s

8.	12V 36mm Encode DC Motor:

![](http://i.imgur.com/5NKBDXU.png)

9.	Package Contains:

``` 
*Aluminum Extrusion Parts 
*100mm Mecanum Wheel 
*Makeblock Orion (Controller)
*12V 36mm Encode DC Motor 
*Me High-Power Encoder Motor Driver 
*Other Hardware and Accessories
```

### FAQ

``` 
(1) Q: When I try to use handle to control this robot, why doesn’t the robot move?
```

A: Please check carefully whether the green LED on USB Host module is on or not. If the green LED is off, please reset the Me Orion board or re-insert the USB receiver. Then press the “Mode” button on the Handle till the green LED and the red LED are solid on. 

Note: Flashing green and red LED indicates unsuccessful pairing. Please try again.

``` 
(2) Q: During operation, I feel this robot is acting strange, how do I fix it?
```

A: Please check if your wiring order of motor is the same as the motor wiring order in the user manual or not. If not, please try to re-wire the motor.

``` 
(3)	Q: How to use other control boards to control this robot?
```

A: There are pin headers on the motor drive board, please read carefully about the meaning of every pin. PWM1 and PWM2 are speed control pins of the motor. When the PWM output is 50% mark-space ratio, the motor is not rotating. When the PWM output is 1% and 99% mark-space ratio, it means the motor is rotating in two different directions at its highest speed.

``` 
(4)	Q: Why my computer cannot install the Makeblock Orion drivers automatically?
```

A: If your computer can not install the Makeblock Orion driver automatically, please download drivers in the following link:

http://learn.makeblock.cc/driver_installation/

``` 
(5)	Q: If I misuse this Mecanum wheel robot, and the sticks in the Mecanum wheel get stuck, how do I fix this?
```

A: According to your own situation, please adjust the baffle bracket of the sticks into a movable status with vice or other tools.