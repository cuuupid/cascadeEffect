#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  HTServo,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     frontRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     backRight,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorLift1,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorLift2,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     frontLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     backLeft,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     collector,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorK,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    dump,                 tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
#pragma config(Servo,  srvo_S2_C2_1,    rollingGoalLeft,      tServoStandard)
#pragma config(Servo,  srvo_S2_C2_2,    rollingGoalRight,     tServoStandard)
#pragma config(Servo,  srvo_S2_C2_3,    servo9,               tServoNone)
#pragma config(Servo,  srvo_S2_C2_4,    servo10,              tServoNone)
#pragma config(Servo,  srvo_S2_C2_5,    servo11,              tServoNone)
#pragma config(Servo,  srvo_S2_C2_6,    servo12,              tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "hitechnic-gyro.h"
#include "Collector.c"
#include "RollingGoal.c"
#include "Dump.c"
//#include "Lift.c"
//#include "Testing.c"

float initial; 	//initial gyro reading
int lastTime = 0;	//last time header was updated, used to find average change in rotation
float currHeading;//USED

float delTime = 0;//calibration
float prevHeading = 0; //USED
float curRate = 0;
int deadZone = 15;
float calibrate =0;
float slideBtn = 0;
float clampPress = 0;
float globalZ;

void motorTrigger(tMotor motorCall,int x,int y,int z)
{
	motor[motorCall]= (y+x+z);
}

void initializeRobot()
{
	initial = 0;
	for(int i = 0; i < 100; i++){//Sensor
		initial += SensorValue[S4];
		wait10Msec(1);
	}
	initial = initial / 100;//Sensor
	return;


}


task FPSDrive()
{
	while(true)
	{
	getJoystickSettings(joystick);

	int x = joystick.joy1_x1; //sideways
	int y = joystick.joy1_y1; //forward and backward
	int z = joystick.joy1_x2; //rotation
	//set x y and z to the joystick values
	//x is horizontal, y is vertical, 1 and 2 are different buttons

	int trueX = (cosDegrees(currHeading)*x)-(sinDegrees(currHeading)*y); //sets trueX to rotated x value
	int trueY = (sinDegrees(currHeading)*x)+(cosDegrees(currHeading)*y);

	if ( (abs(joystick.joy1_x1) < 15)) //drive only if joystick out of dead zones
	{
		x = 0;
	}
	if ( (abs(joystick.joy1_y1) < 15)) //drive only if joystick out of dead zones
	{
		y = 0;
	}

	x = trueX;
	y = trueY;

	//int left, right;
	motor[frontLeft]=(y+x-z)*.75;
	motor[backLeft]= (y-x-z)*.75; // -y-x||x+y
	motor[frontRight]=(-y+x-z)*.75;//-y+x||-x+y
	motor[backRight]=(-y-x-z)*.75;


	}
}

task getHeading() {
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

task Lift()
{
	while(true)
	{
		getJoystickSettings(joystick);
		int y= joystick.joy2_y1;
		if (abs(joystick.joy2_y1) <30)
			{
			 y=0;
		  }
		if(y<0)
			{
				y*.25;
			}

		motor[motorLift1] = -y;
		motor[motorLift2] = -y;
			}
}
task main()
{
	initializeRobot();
	StartTask(getHeading);
	StartTask(FPSDrive);
	StartTask(runCollector);
	StartTask(runRollingGoal);
	StartTask(runDump);
	StartTask(Lift);

	waitForStart();

	while(true)
	{
		getJoystickSettings(joystick);
	}
	while(true)
	{
		wait1Msec(1000);
	}
}
