#include "MeOrion.h"
//#include <Arduino.h>
#include "SoftwareSerial.h"
#include <Wire.h>

MeUSBHost usbhost(PORT_3);

MeEncoderNew motor1(0x09, SLOT1); 
MeEncoderNew motor2(0x09, SLOT2); 

#define PORT1_MOTOR1  11    //PWM Control Pin
#define PORT1_MOTOR2  10
#define PORT2_MOTOR1  3
#define PORT2_MOTOR2  9

int moveSpeed = 125;
uint8_t moveSpeed_FW =0;
uint8_t moveSpeed_BW =0;
unsigned char stop_flag=0;

void setup()
{

  usbhost.init(USB1_0);   //USB Remote Control Handle Initialization
  Serial.begin(9600);
  motor1.begin();
  motor2.begin();
  motor1.setMode(1);  //0:I2C_MODE;1:PWM_MODE;2:PWM_I2C_PWM;
  motor2.setMode(1);
  pinMode(PORT1_MOTOR1, OUTPUT);
  pinMode(PORT1_MOTOR2, OUTPUT);
  pinMode(PORT2_MOTOR1, OUTPUT);
  pinMode(PORT2_MOTOR2, OUTPUT);  

}

void loop()
{
 control();
 moveSpeed_FW=moveSpeed/2+125;    //Speed conversion
 moveSpeed_BW=125-moveSpeed/2;
}


void parseJoystick(unsigned char * buf)
{
  unsigned char temp2=buf[2];  //Remote control receive data
  unsigned char temp3=buf[3];
  unsigned char temp4=buf[4];
 
  if(temp2==128)
  {
    if(temp3==128)  
    {
        stop_flag++;
        if(stop_flag>2)
       {
          Stop_run();    //Release the keys/Stop motor 
          stop_flag=3;
       }    
    }
    if(temp3==0) Forward_run();    //Forward keys (1 keys) 
    if(temp3==255) Backward_rum(); //Backward keys (3 keys)
  }
  if(temp2==255)
  {    
    if(temp3==0)  RightUp_run();     //Forward and Right keys (1 and 2 keys) 
    if(temp3==128)  Right_run();     //Right keys(2 keys)  
    if(temp3==255)  RightDown_run(); //Backward and Right keys(2 and 3 keys)
  }
  if(temp2==0)
  { 
    if(temp3==0)  LeftUp_run();      //Forward and lift keys(1 and 4 keys)   
    if(temp3==128)  Left_run();      //Life keys(4 keys) 
    if(temp3==255)  LeftDown_run();  //Backward and Lift keys(3 and 4 keys)
  }
  
   if(temp4==79)      //Fork keys(8 keys)
  {
     Speed_down();   //Speed Decrease
  }
  else if(temp4==31) //Triangle keys(7 keys)
  {
     Speed_up();     //Speed up
  }
  else if(temp4==143) //Square keys(5 keys)
  {
     TurnLeft_run();  //Spot Turn To Left
     stop_flag=0;
  }
  else if(temp4==47) //Round keys(6 keys)
  { 
     TurnRight_run();//Spot Turn To Right
     stop_flag=0;
  }  
}

void Forward_run() 
{  
  analogWrite(PORT1_MOTOR1, moveSpeed_BW);
  analogWrite(PORT1_MOTOR2, moveSpeed_FW);
  analogWrite(PORT2_MOTOR1, moveSpeed_FW);
  analogWrite(PORT2_MOTOR2, moveSpeed_BW);
}
void Backward_rum() 
{
  analogWrite(PORT1_MOTOR1, moveSpeed_FW);
  analogWrite(PORT1_MOTOR2, moveSpeed_BW);
  analogWrite(PORT2_MOTOR1, moveSpeed_BW);
  analogWrite(PORT2_MOTOR2, moveSpeed_FW);
}
void Right_run()  
{
  analogWrite(PORT1_MOTOR1, moveSpeed_BW);
  analogWrite(PORT1_MOTOR2, moveSpeed_BW);
  analogWrite(PORT2_MOTOR1, moveSpeed_FW);
  analogWrite(PORT2_MOTOR2, moveSpeed_FW);
}
void Left_run()  
{
  analogWrite(PORT1_MOTOR1, moveSpeed_FW);
  analogWrite(PORT1_MOTOR2, moveSpeed_FW);
  analogWrite(PORT2_MOTOR1, moveSpeed_BW);
  analogWrite(PORT2_MOTOR2, moveSpeed_BW);
}
void RightUp_run()
{
  analogWrite(PORT1_MOTOR1, moveSpeed_BW);
  analogWrite(PORT1_MOTOR2, 127);
  analogWrite(PORT2_MOTOR1, moveSpeed_FW);
  analogWrite(PORT2_MOTOR2, 127);
}
void RightDown_run()
{
  analogWrite(PORT1_MOTOR1, 127);
  analogWrite(PORT1_MOTOR2, moveSpeed_BW);
  analogWrite(PORT2_MOTOR1, 127);
  analogWrite(PORT2_MOTOR2, moveSpeed_FW);
}
void LeftUp_run()
{
  analogWrite(PORT1_MOTOR1, 127);
  analogWrite(PORT1_MOTOR2, moveSpeed_FW);
  analogWrite(PORT2_MOTOR1, 127);
  analogWrite(PORT2_MOTOR2, moveSpeed_BW);
}
void LeftDown_run()
{
  analogWrite(PORT1_MOTOR1, moveSpeed_FW);
  analogWrite(PORT1_MOTOR2, 127);
  analogWrite(PORT2_MOTOR1, moveSpeed_BW);
  analogWrite(PORT2_MOTOR2, 127);
}
void TurnRight_run()
{
  analogWrite(PORT1_MOTOR1, moveSpeed_BW);
  analogWrite(PORT1_MOTOR2, moveSpeed_BW);
  analogWrite(PORT2_MOTOR1, moveSpeed_BW);
  analogWrite(PORT2_MOTOR2, moveSpeed_BW);
}
void TurnLeft_run()
{
  analogWrite(PORT1_MOTOR1, moveSpeed_FW);
  analogWrite(PORT1_MOTOR2, moveSpeed_FW);
  analogWrite(PORT2_MOTOR1, moveSpeed_FW);
  analogWrite(PORT2_MOTOR2, moveSpeed_FW);
}

void Speed_up()
{
  moveSpeed=moveSpeed+3;
  if(moveSpeed>250)  moveSpeed=250;
}
void Speed_down()
{
   moveSpeed=moveSpeed-3;
  if(moveSpeed<0)  moveSpeed=0;
}
void Stop_run()
{
  analogWrite(PORT1_MOTOR1, 127);
  analogWrite(PORT1_MOTOR2, 127);
  analogWrite(PORT2_MOTOR1, 127);
  analogWrite(PORT2_MOTOR2, 127);
}

void control()
{
  if(!usbhost.device_online)
  {
   usbhost.probeDevice();//Device is not online ,initialization   
    delay(100);
  }
  else
  {
    int len = usbhost.host_recv();//Read data length
    parseJoystick(usbhost.RECV_BUFFER);
    delay(5);
  }
}


void parseJoystick1(unsigned char *buf)   //Analytic function, print 8 bytes from USB Host
{
  int i = 0;
  for(i = 0; i < 7; i++)
  {
    Serial.print(buf[i]);  //It won't work if you connect to the Makeblock Orion.
    Serial.print('-');
  }
  Serial.println(buf[7]);
  delay(10);
}

