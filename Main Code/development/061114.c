#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     frontright,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     backright,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     backleft,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     frontleft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     sweep,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     transport,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    dump,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*#pragma config(Motor,  mtr_S1_C1_1,     frontright,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     backright,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     frontleft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     backleft,      tmotorTetrix, openLoop)*/

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
#include "JoystickDriver.c"
void initializeRobot()
{
	servo[dump]=50;
	// Place code here to sinitialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

	return;
}
task main()
{
	while(true)
	{
		getJoystickSettings(joystick);
		int y= joystick.joy1_y1;
		int x= joystick.joy1_x1;
		int z= -joystick.joy1_x2;

		motor[frontright]= (-y+x+z);
		motor[backright]= (-y-x+z);
		motor[frontleft]= (y+x+z);
		motor[backleft]= (y-x+z);

		/*if(abs(y)<15)
		{
			motor[frontright]=x;
			motor[backright]=-x;
			motor[frontleft]=x;
			motor[backleft]=-x;
		}
		*/

		if (joy1Btn(5))
		{
			motor[sweep]=-100;
			//button 5 sweeps the blocks into the collector
		}

		if (joy1Btn(7))
		{
			motor[sweep]=0;
			//button 7 stops the sweeper
		}

		motor[transport]=-joystick.joy2_y1;
		//driver 2 joystick 1 controls the linear slide

		if (joy1Btn(6))
		{
			servo[dump]=20;
			//button 6 turns collector to dumping value
		}
		if (joy1Btn(8))
		{
			servo[dump]=0;
		}
	}


}