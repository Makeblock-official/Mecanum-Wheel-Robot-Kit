#include "MeOrion.h"
//#include <Arduino.h>
#include "SoftwareSerial.h"
#include <Wire.h>
/*************************************************************************
* File Name          : Mecanum_chassis_new.ino
* Author             : Zzipeng
* Updated            : Zzipeng
* Version            : V09.01.001
* Date               : 29/12/2016
* Description        : Firmware for Makeblock Mecanum_chassis.  
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 - 2016 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
* History:
* <Author>         <Time>         <Version>        <Descr>
* Zzipeng          2016/12/29     09.01.001        fix some bug
**************************************************************************/
MeUSBHost usbhost(PORT_3);

MeEncoderNew motor1(0x09, SLOT1); 
MeEncoderNew motor2(0x09, SLOT2); 
MeEncoderNew motor3(0x0a, SLOT1); 
MeEncoderNew motor4(0x0a, SLOT2); 

#define MAXSPEED 150
#define MINSPEED (-150)
#define DEFAULTSPEED 100
#define MID    128
#define FRONT  0
#define BACK   255
#define LEFT   0
#define RIGHT  255
typedef struct {
  signed int turn;
  signed int backfront;
  signed int leftright;
  unsigned char biaskey;
}RCDATA;
RCDATA Rcdata;

int moveSpeed = DEFAULTSPEED,motor1speed=0,motor2speed=0,motor3speed=0,motor4speed=0;
unsigned char stop_flag=0;
unsigned char Rc_Flag = 0;
signed char Martix[4]={-1,1,1,-1};//this array is set to motor's direction   changed the symbol to change the motor's direction

void parseJoystick(unsigned char * buf)
{
//  Rcdata.turn = buf[0] - MID;  //Remote control receive data
  Rcdata.leftright = buf[2];  //Remote control receive data
  Rcdata.backfront = buf[3];
  Rcdata.biaskey   = buf[4];
/*  Serial.print(Rcdata.turn);
  Serial.print('t');"*/
 /* Serial.print(Rcdata.leftright);
  Serial.print('t');
  Serial.print(Rcdata.backfront);
  Serial.print('t');
  Serial.println(Rcdata.biaskey);
  delay(20);*/
}

void usbdataprocess()
{
  if(!usbhost.device_online)
  {
    usbhost.probeDevice();//Device is not online ,initialization 
    Serial.println("failed");//when you see the serial led blink quickly,it means the mcu doesn't receive the rc data.so,reset the mcu board and uplug the port of the rcceiver
    Rc_Flag = 1;
    delay(100);
  }
  else
  {
    Rc_Flag = 0;
    int len = usbhost.host_recv();//Read data length
    parseJoystick(usbhost.RECV_BUFFER);
    delay(5);
  }
}

void motorprocess()
{
     if(Rcdata.biaskey == 31)
     {
        Speed_up();
     }
     else if(Rcdata.biaskey == 79)
     {
        Speed_down();
     }

    motor1speed = moveSpeed*Martix[0];
    motor2speed = moveSpeed*Martix[1];
    motor3speed = moveSpeed*Martix[2];
    motor4speed = moveSpeed*Martix[3];
    
   if(Rcdata.backfront == MID)
  {
    if(Rcdata.leftright == MID)
    {
        if(Rcdata.biaskey == 143)
       {
         TurnLeft_run();  //Spot Turn To Left
       }
       else if(Rcdata.biaskey==47) //Round keys(6 keys)
       { 
         TurnRight_run();//Spot Turn To Right
       }
       else
       {
          stop_flag++;
          if(stop_flag>2)
          {
            Stop_run();    //Release the keys/Stop motor 
            stop_flag=0;
          } 
       }  
     }
     else if(Rcdata.leftright == LEFT)
     {
        Left_run();
     }
     else if(Rcdata.leftright == RIGHT)
     {
        Right_run();
      }
   }
   else if(Rcdata.backfront == FRONT)
   {
      if(Rcdata.leftright == MID)
      {
         Forward_run();
      }
      else if(Rcdata.leftright == LEFT)
      {
        LeftUp_run();
       }
      else if(Rcdata.leftright == RIGHT)
      {
        RightUp_run();
       }
    }
    else if(Rcdata.backfront == BACK)
    {
       if(Rcdata.leftright == MID)
       {
         Backward_run();
       }
       else if(Rcdata.leftright == LEFT)
       {
         LeftDown_run();
       }
       else if(Rcdata.leftright == RIGHT)
       {
         RightDown_run();
       }     
     }
}

void Forward_run()
{
  motor1.runSpeed(motor1speed,0);
  motor2.runSpeed(motor2speed,0);
  motor3.runSpeed(motor3speed,0);
  motor4.runSpeed(motor4speed,0);
 }

 void Backward_run()
{
  motor1.runSpeed(-motor1speed,0);
  motor2.runSpeed(-motor2speed,0);
  motor3.runSpeed(-motor3speed,0);
  motor4.runSpeed(-motor4speed,0);
 }

 void Right_run()
{
  motor1.runSpeed(motor1speed,0);
  motor2.runSpeed(-motor2speed,0);
  motor3.runSpeed(motor3speed,0);
  motor4.runSpeed(-motor4speed,0);
 }

 void Left_run()
{
  motor1.runSpeed(-motor1speed,0);
  motor2.runSpeed(motor2speed,0);
  motor3.runSpeed(-motor3speed,0);
  motor4.runSpeed(motor4speed,0);
 }
 
void RightUp_run()
{
  motor1.runSpeed(motor1speed,0);
  motor2.runSpeed(0,0);
  motor3.runSpeed(motor3speed,0);
  motor4.runSpeed(0,0);
}

void RightDown_run()
{
  motor1.runSpeed(0,0);
  motor2.runSpeed(-motor2speed,0);
  motor3.runSpeed(0,0);
  motor4.runSpeed(-motor4speed,0);
 
}

void LeftUp_run()
{
  motor1.runSpeed(0,0);
  motor2.runSpeed(motor2speed,0);
  motor3.runSpeed(0,0);
  motor4.runSpeed(motor4speed,0);
}

void LeftDown_run()
{
  motor1.runSpeed(-motor1speed,0);
  motor2.runSpeed(0,0);
  motor3.runSpeed(-motor3speed,0);
  motor4.runSpeed(0,0);
}

void TurnRight_run()
{
  motor1.runSpeed(motor1speed,0);
  motor2.runSpeed(-motor2speed,0);
  motor3.runSpeed(-motor3speed,0);
  motor4.runSpeed(motor4speed,0);
}

void TurnLeft_run()
{
  motor1.runSpeed(-motor1speed,0);
  motor2.runSpeed(motor2speed,0);
  motor3.runSpeed(motor3speed,0);
  motor4.runSpeed(-motor4speed,0);
}

void Speed_up()
{
  moveSpeed +=2;
  if(moveSpeed>MAXSPEED)  moveSpeed=MAXSPEED;
}
void Speed_down()
{
   moveSpeed -= 2;
  if(moveSpeed<0)  moveSpeed=0;
}
void Stop_run()
{
  motor1.runSpeed(0,1);
  motor2.runSpeed(0,1);
  motor3.runSpeed(0,1);
  motor4.runSpeed(0,1);
}
 
void setup() {
  // put your setup code here, to run once:
  usbhost.init(USB1_0);   //USB Remote Control Handle Initialization
  delay(10);
  motor1.begin();
//  motor2.begin();
  motor3.begin();
//  motor4.begin();
  delay(10);
  motor1.setMode(1);  //0:I2C_MODE;1:PWM_MODE;2:PWM_I2C_PWM;
  motor2.setMode(1);
  motor3.setMode(1);  //0:I2C_MODE;1:PWM_MODE;2:PWM_I2C_PWM;
  motor4.setMode(1);
  motor1.runSpeed(0);
  motor2.runSpeed(0);
  motor3.runSpeed(0);
  motor4.runSpeed(0);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 usbdataprocess();//receive the data from usb host
 if(!Rc_Flag)
 {
    motorprocess();
 }
}
