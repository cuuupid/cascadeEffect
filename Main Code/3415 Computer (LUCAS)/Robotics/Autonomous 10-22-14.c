#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S3,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     Accel,          sensorI2CHiTechnicAccel)
#pragma config(Motor,  mtr_S1_C1_1,     backright,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     backleft,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     emptymotor,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     collector,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     frontright,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     frontleft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     liftRight,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     liftLeft,      tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    scoreB,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    scoreA,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    rollingBack,          tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    rollingSide,          tServoStandard)      !!*//

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

void initializeRobot()
{
	return;
}

void motorTrigger(tMotor motorCall,int x,int y,int z)
{
	motor[motorCall]= (y+x+z);
}

void move(int angle, int power, int time)
{

	int x = power*128/100*cosDegrees(angle);
	int y = power*128/100*sinDegrees(angle);
	int z = 0;

	int accelValue = SensorValue(Accel); //set accelerometer value to accelValue
	int gyroValue = SensorValue(Gyro); //set gyroscope value to gyroValue

	nxtDisplayCenteredBigTextLine(5,"%d",gyroValue); //display gyroValue to NXT
	nxtDisplayCenteredBigTextLine(5,"%d",accelValue); //display accelValue to NXT

	int trueX = (cosDegrees(currHeading)*x)-(sinDegrees(currHeading)*y); //sets trueX to rotated x value
	int trueY = (sinDegrees(currHeading)*x)+(cosDegrees(currHeading)*y); //sets trueY to rotated y value

	x = trueX;
	y = trueY;

	motorTrigger(frontright,x,-y,-z);
	motorTrigger(backright,-x,-y,-z);
	motorTrigger(frontleft,x,y,-z);
	motorTrigger(backleft,-x,y,-z);

	wait1Msec(time);
}

void rotate(int relativeAngle, int direction, int power)
{
	int finalHeading = currHeading + direction*relativeAngle;

	if (finalHeading > 360) finalHeading -= 360;
	else if(finalHeading < 0) finalHeading += 360;

	while(abs(finalHeading - currHeading) > 3)
	{
		int x = 0;
		int y = 0;
		int z = power*direction*128/100;

		motorTrigger(frontright,x,-y,-z);
		motorTrigger(backright,-x,-y,-z);
		motorTrigger(frontleft,x,y,-z);
		motorTrigger(backleft,-x,y,-z);
	}
}

void rollingGoal(TServoIndex chosenServo, bool release)
{
	if (release)servo[chosenServo] = 0; //#brianleeoneliner
	else servo[chosenServo] = 50; //#brianleeoneliner
}

task getHeading()
{
	HTGYROstartCal(Gyro);
	PlaySound(soundBeepBeep);
	while (true) {
		time1[T1] = 0;
		curRate = HTGYROreadRot(Gyro);
		if (abs(curRate) > 3) //sets deadzones for gyroscope
		{
			prevHeading = currHeading;
			currHeading = prevHeading + curRate * delTime; //changes current heading based on the rate of change and time elapsed
			if (currHeading > 360) currHeading -= 360; //keeps current heading between 0 and 360
			else if (currHeading < 0) currHeading += 360; // keeps curent heading between 0 and 360
		}
		wait1Msec(5);
		delTime = ((float)time1[T1]) / 1000;
	}
}

task   getXY()
{  //365.76

}//18in = 45.72

task display()
{
	while (true){
		eraseDisplay();
		nxtDisplayCenteredTextLine(0, "Heading: %d", currHeading);
		nxtDisplayCenteredTextLine(1, "joyAngle: %d", joyAngle);
		nxtDisplayCenteredTextLine(2, "Sonar: %d", sonarValue);
		wait1Msec(20);
	}
}

task getAccel()
{
	int X_Accel;
	int Y_Accel;

	//X_Accel = SensorValue[xAxis]/2000;
	//Y_Accel = SensorValue[yAxis];

}

task calibrateDrive()//needs work, doing some wack stuff
{
	while(true)
	{
		if(joy1Btn(1)&&joy1Btn(3))
		{
			PlaySound(soundUpwardTones);
			wait1Msec(500);
			/*

			initializeRobot();
			HTGYROstartCal(Gyro);
			delTime = 0;//calibration
			prevHeading = 0;
			curRate = 0;
			wait1Msec(500);
			*/
		}
	}
}

task main()
{
	initializeRobot();
	waitForStart();

	motorTrigger(frontright,0,-30,-0);
	motorTrigger(backright,-0,-30,-0);
	motorTrigger(frontleft,0,30,-0);
	motorTrigger(backleft,-0,30,-0);
	//move off ramp
	//move(0, 80, 5); //drive backwards angle, power, time
	//get rolling goal
	//rollingGoal(rollingback, true); //clamp onto 60 rolling goal
	//score into rolling goal
	//motor[transportA]=80;
	//motor[transportB]=80;//move linear slides in position to score in 60
	wait1Msec(2000);
	/*
	motorTrigger(frontright,0,-0,-0);
	motorTrigger(backright,-0,-0,-0);
	motorTrigger(frontleft,0,0,-0);
	motorTrigger(backleft,-0,0,-0);
	*/
	motor[backleft] = 0;
	motor[frontright] = 0;
	motor[backright] = 0;
	motor[frontleft] = 0;

	//motor[transportA]=0;
	//motor[transportB]=0; //stop linear slides
	//drive into parking zone


	while(true)
	{
		//wait1Msec(1000);
	}
}
