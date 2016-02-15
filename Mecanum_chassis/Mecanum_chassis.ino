//  Please download the Makeblock library from: https://github.com/Makeblock-official/Makeblock-Libraries
//  Press the "Mode" key until the red LED and the green LED are both on.

#include <Wire.h>
#include <SoftwareSerial.h>
#include "MeOrion.h"

MeUSBHost joypad(PORT_3);
//  The code of joypad (when the red LED is on)
//  default£º128-127-128-127-15-0-0-128
//  left one£º128-127-128-127-15-1-0-128
//  right one£º128-127-128-127-15-2-0-128
//  left two£º128-127-128-127-15-4-0-128
//  right two£º128-127-128-127-15-8-0-128
//  triangle£º128-127-128-127-31-0-0-128 £¨0001 1111£©
//  square£º128-127-128-127-143-0-0-128 £¨1000 1111£©
//  cross£º128-127-128-127-79-0-0-128 £¨0100 1111£©
//  circle£º128-127-128-127-47-0-0-128 £¨0010 1111£©
//  left joystick press down: 128-127-128-127-15-64-0-128
//  right joystick press down: 128-127-128-127-15-128-0-128
//  up£º128-127-128-127-0-0-0-128 £¨0000 0000£©
//  down£º128-127-128-127-4-0-0-128 £¨0000 0100£©
//  left£º128-127-128-127-6-0-0-128 £¨0000 0110£©
//  right£º128-127-128-127-2-0-0-128 £¨0000 0010£©
//  left-up£º128-127-128-127-7-0-0-128 £¨0000 0111£©
//  left-down£º128-127-128-127-5-0-0-128 £¨0000 0101£©
//  right-up£º128-127-128-127-1-0-0-128 £¨0000 0001£©
//  right-down£º128-127-128-127-3-0-0-128 £¨0000 0011£©
//  select£º128-127-128-127-15-16-0-128
//  start£º128-127-128-127-15-32-0-128
//  joystick£ºrightX-rightY-leftX-leftY-15-0-0-128

//  map the pins to the wheels/motors
#define PIN_MOTOR_1	10
#define PIN_MOTOR_2	11
#define PIN_MOTOR_3	9
#define PIN_MOTOR_4	3

float linearSpeed = 50;
float angularSpeed = 50;
float maxLinearSpeed = 120;
float maxAngularSpeed = 120;
float minLinearSpeed = 15;
float minAngularSpeed = 15;

void setup()
{
	pinMode(PIN_MOTOR_1, OUTPUT);
	pinMode(PIN_MOTOR_2, OUTPUT);
	pinMode(PIN_MOTOR_3, OUTPUT);
	pinMode(PIN_MOTOR_4, OUTPUT);

	joypad.init(USB1_0);
}

void loop()
{
	if (!joypad.device_online)
	{
		joypad.probeDevice();
		delay(1000);
	}
	else
	{
		int len = joypad.host_recv();
		parseJoystick(joypad.RECV_BUFFER);
		delay(5);
	}
}


void setEachMotorSpeed(float speed1, float speed2, float speed3, float speed4)
{
	if ((fabs(speed1) < 3) && (fabs(speed2) < 3) && (fabs(speed3) < 3) && (fabs(speed4) < 3))
	{
		analogWrite(PIN_MOTOR_1, 0);
		analogWrite(PIN_MOTOR_2, 0);
		analogWrite(PIN_MOTOR_3, 0);
		analogWrite(PIN_MOTOR_4, 0);
	}
	else
	{
		analogWrite(PIN_MOTOR_1, 128 - speed1);
		analogWrite(PIN_MOTOR_2, 128 + speed2);
		analogWrite(PIN_MOTOR_3, 128 + speed3);
		analogWrite(PIN_MOTOR_4, 128 - speed4);
	}
}

void parseJoystick(unsigned char *buf)   //Analytic function, print 8 bytes from USB Host
{
    //  Test the position and direction of wheels
    //  Press the left one and two, and the right one and two.
    //  Then press one of the 1:triangle/2:square/3:cross/4:circle and the up/down to test the wheels
	if (15 == (buf[5]) & 0x0F)
	{
		char wheelNumber = 0;
		char dir = 0;

		switch (buf[4] | 0x0F)
		{
		case 31:
			wheelNumber = 1;
			break;
		case 143:
			wheelNumber = 2;
			break;
		case 79:
			wheelNumber = 3;
			break;
		case 47:
			wheelNumber = 4;
			break;
		default:
			wheelNumber = 0;
			break;
		}
		switch (buf[4] & 0x0F)
		{
		case 0:
		case 1:
		case 7:
			dir = 1;
			break;
		case 3:
		case 4:
		case 5:
			dir = -1;
			break;
		default:
			dir = 0;
			break;
		}

		analogWrite(PIN_MOTOR_1, 128 + (!!(1 == wheelNumber)) * 100 * dir);
		analogWrite(PIN_MOTOR_2, 128 + (!!(2 == wheelNumber)) * 100 * dir);
		analogWrite(PIN_MOTOR_3, 128 + (!!(3 == wheelNumber)) * 100 * dir);
		analogWrite(PIN_MOTOR_4, 128 + (!!(4 == wheelNumber)) * 100 * dir);
	}
	else    //  normal control mode
	{
		//  increase/decrease the speed
		switch (buf[5])
		{
		case 1:
			linearSpeed += 3;
			if (linearSpeed > maxLinearSpeed)
			{
				linearSpeed = maxLinearSpeed;
			}
			break;
		case 2:
			angularSpeed += 3;
			if (angularSpeed > maxAngularSpeed)
			{
				angularSpeed = maxAngularSpeed;
			}
			break;
		case 4:
			linearSpeed -= 3;
			if (linearSpeed < minLinearSpeed)
			{
				linearSpeed = minLinearSpeed;
			}
			break;
		case 8:
			angularSpeed -= 3;
			if (angularSpeed < minAngularSpeed)
			{
				angularSpeed = minAngularSpeed;
			}
			break;
		default:
			break;
		}

		if ((128 != buf[0]) || (127 != buf[1]) || (128 != buf[2]) || (127 != buf[3]))
		{
            //  the joystick
			float x = ((float)(buf[2]) - 127) / 128;
			float y = (127 - (float)(buf[3])) / 128;
			float a = (127 - (float)(buf[0])) / 128;
			mecanumRun(x * linearSpeed, y * linearSpeed, a * angularSpeed);
		}
		else
		{
            //  the direction keys
			switch (buf[4])
			{
			case 0:
				mecanumRun(0, linearSpeed, 0);
				break;
			case 4:
				mecanumRun(0, -linearSpeed, 0);
				break;
			case 6:
				mecanumRun(-linearSpeed, 0, 0);
				break;
			case 2:
				mecanumRun(linearSpeed, 0, 0);
				break;
			case 7:
				mecanumRun(-linearSpeed / 2, linearSpeed / 2, 0);
				break;
			case 5:
				mecanumRun(-linearSpeed / 2, -linearSpeed / 2, 0);
				break;
			case 1:
				mecanumRun(linearSpeed / 2, linearSpeed / 2, 0);
				break;
			case 3:
				mecanumRun(linearSpeed / 2, -linearSpeed / 2, 0);
				break;
			default:
				mecanumRun(0, 0, 0);
				break;
			}
		}
	}
}

//  motion control
void mecanumRun(float xSpeed, float ySpeed, float aSpeed)
{
	float speed1 = ySpeed - xSpeed + aSpeed;
	float speed2 = ySpeed + xSpeed - aSpeed;
	float speed3 = ySpeed - xSpeed - aSpeed;
	float speed4 = ySpeed + xSpeed + aSpeed;

	float max = speed1;
	if (max < speed2)   max = speed2;
	if (max < speed3)   max = speed3;
	if (max < speed4)   max = speed4;

	if (max > maxLinearSpeed)
	{
		speed1 = speed1 / max * maxLinearSpeed;
		speed2 = speed2 / max * maxLinearSpeed;
		speed3 = speed3 / max * maxLinearSpeed;
		speed4 = speed4 / max * maxLinearSpeed;
	}

	setEachMotorSpeed(speed1, speed2, speed3, speed4);
}
