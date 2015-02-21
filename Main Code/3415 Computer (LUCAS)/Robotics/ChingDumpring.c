#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     Gyro,           sensorI2CMuxController)
#pragma config(Sensor, S3,     Sonar,          sensorSONAR)
#pragma config(Sensor, S4,     Accel,          sensorI2CHiTechnicAccel)
#pragma config(Motor,  mtr_S1_C1_1,     backright,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     backleft,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     emptymotor,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     collector,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     frontright,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     frontleft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     liftRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     liftLeft,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    scoreA,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    scoreB,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    rollingBack,          tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    rollingSide,          tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)


#include "JoystickDriver.c"
#include "hitechnic-gyro.h"

float initial; 	//initial gyro reading
float heading; 	//robots current yaw
float radheading;   //heading in radians
int lastTime = 0;	//last time header was updated, used to find average change in rotation
float FLset; //front-left set, refers to Front-Left and Back-Right
float FRset; //front-right set, refers to Front-Right and Back-Left motor
float joyAngle; // angle of the first joystick
float currHeading;
float sonarValue;
float delTime = 0;//calibration
float prevHeading = 0;
float curRate = 0;
int deadZone = 15;

void motorTrigger(tMotor motorCall,int x,int y,int z)
{
	motor[motorCall]= (y+x+z);
}

void initializeRobot()
{
	servo[scoreA]=0;
	servo[scoreB]=0;
	servo[rollingBack] = 180;
	servo[rollingSide] = 180; //rolling goal clamps are open

	return;
}


task main()
{
	initializeRobot();

	waitForStart();   // wait for start of tele-op phase
	getJoystickSettings(joystick);

	while (true)
	{
		if (joy2Btn(1))//ready position
		{
			servo[scoreA] = 75;
			servo[scoreB] = 255-75;
		}
		if (joy2Btn(2))//score position
		{
			servo[scoreA] = 240;
			servo[scoreB] = 10;
		}
		if (joy2Btn(3))//resting position
		{
			servo[scoreA] = 10;
			servo[scoreB] = 240;
		}
		if (joy1Btn(5))
		{
			servo[rollingBack] = 50; //clamp onto rolling goal in back of robot
		}
		if (joy1Btn(7))
		{
			servo[rollingBack] = 0; //release rolling goal
		}
		if (joy1Btn(6))
		{
			servo[rollingSide] = 50; //clamp onto rolling goal on side of robot
		}
		if (joy1Btn(8))
		{
			//PlaySound(soundUpwardTones);
			servo[rollingSide] = 0; //release rolling goal
		}
	}
}
