#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  HTServo,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     frontRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     backRight,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     lift1,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     lift2,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     frontLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     backLeft,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     collector,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     collector2,        tmotorTetrix, openLoop)
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
/*																																			*/

#include "hitechnic-gyro.h"
#include "Collector.c"
#include "RollingGoal.c"
#include "Dump.c"
//#include "TurnRotate.c"
//#include "Lift.c"
//#include "Testing.c"



float initial; 	//initial gyro reading
int lastTime = 0;	//last time header was updated, used to find average change in rotation
float currHeading;//USED
float calibrate;
float delTime = 0;//calibration
float prevHeading = 0; //USED
float curRate = 0;
int deadZone = 15;
float slideBtn = 0;
float clampPress = 0;
float globalZ;
float trueZ;
float encodedValue;
float sonarValue;
float encoderTest;


void motorTrigger(tMotor motorCall,int x,int y,int z)
{
	motor[motorCall]= (y+x+z);
}

void moveEncodedMotor(int target)
{
	nMotorEncoder[lift2] = 0;
//	nMotorEncoder[lift2] = 0; has no encoder

	nMotorEncoderTarget[lift2] = target;  //times spun 2.5 rotations = 900
//	nMotorEncoderTarget[lift2] = target; //times spun

	motor[lift1] = 50;
	motor[lift2] = 50;

  wait1Msec(8000);
}

void move(int angle, int power, int time)
{

	int x = power*128/100*cosDegrees(angle);
	int y = power*128/100*sinDegrees(angle);
	int z = 0;

//	int accelValue = SensorValue(Accel); //set accelerometer value to accelValue
	int gyroValue = SensorValue(Gyro); //set gyroscope value to gyroValue

	nxtDisplayCenteredBigTextLine(5,"%d",gyroValue); //display gyroValue to NXT
	//nxtDisplayCenteredBigTextLine(5,"%d",accelValue); //display accelValue to NXT

	int trueX = (cosDegrees(currHeading+calibrate)*x)-(sinDegrees(currHeading+calibrate)*y); //sets trueX to rotated x value
	int trueY = (sinDegrees(currHeading+calibrate)*x)+(cosDegrees(currHeading+calibrate)*y); //sets trueY to rotated y value

	x = trueX;
	y = trueY;

	motorTrigger(frontRight,x,-y,-z);
	motorTrigger(backRight,-x,-y,-z);
	motorTrigger(frontLeft,x,y,-z);
	motorTrigger(backLeft,-x,y,-z);

	wait1Msec(time);

	motorTrigger(frontRight,0,0,0);
	motorTrigger(backRight,0,0,0);
	motorTrigger(frontLeft,0,0,0);
	motorTrigger(backLeft,0,0,0);

	wait1Msec(100);

}

void rotate(int relativeAngle, int direction, int power)
{
	int finalHeading = currHeading + direction*relativeAngle;

	if (finalHeading > 360) finalHeading -= 360;
	else if(finalHeading < 0) finalHeading += 360;

	while(abs(finalHeading - currHeading) != 0)
	{
		int x = 0;
		int y = 0;
		int z = power*direction*128/100;

		motorTrigger(frontRight,x,-y,-z);
		motorTrigger(backRight,-x,-y,-z);
		motorTrigger(frontLeft,x,y,-z);
		motorTrigger(backLeft,-x,y,-z);
	}
	motorTrigger(frontRight,0,0,0);
	motorTrigger(backRight,0,0,0);
	motorTrigger(frontLeft,0,0,0);
	motorTrigger(backLeft,0,0,0);

	wait1Msec(100);
}

task calibrateDrive()//needs work, doing some wack stuff
{
			calibrate = 360 - currHeading;
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

task display()
{
	while (true){
		eraseDisplay();
		nxtDisplayCenteredTextLine(0, "Heading: %d", currHeading);
		nxtDisplayCenteredTextLine(2, "Sonar: %d", sonarValue);
		wait1Msec(20);
	}
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

void encodedMove(float dist){
	encodedValue = (dist/4*PI)*1440;
	nMotorEncoderTarget(frontLeft) = nMotorEncoder(encodedValue);
	nMotorEncoderTarget(frontRight) = encodedValue;
	nMotorEncoderTarget(backRight) = -encodedValue;
	nMotorEncoderTarget(backLeft) = -encodedValue;
}

task main()
{
	waitForStart();

motor[lift2]=-90;
motor[lift1]=-90;
nMotorEncoder[lift2] = 0;
wait1Msec(50);
while(abs(nMotorEncoder[lift2])<(1440*2.5))//2.5 rotations
{}
motor[lift2]=0;
motor[lift1]=0;
//Use defensive tactics to create obstacles for other automated machines
//Reset position/Recalibrate
//Find Infrared Sensor
//Score balls
//Knock down kickstand






//Neccessary do not touch
while(true)
	{
	wait1Msec(10000);
	}
}

/*

Move out of parking zone/ramp to point A (in front of ramp) facing forward still
Then scan for IR
Move to location, mantaining no strafe
release kickstand

*/
