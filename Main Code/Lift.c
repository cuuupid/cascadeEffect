#include "JoystickDriver.c"

//HOW TO USE
//SETUP LIFT MOTORS, motorLift, motorLift2
//INCLUDE THIS FILE IN TELEOP
//RUN TASK encodedLift
//BUTTON 4 CONTROLS LIFT, PRESS ONCE FOR HIGH, TWICE FOR CENTER, THEN AGAIN TO RESET. NO NEED TO WAIT!

//to convert from distance to encoder ticks, it's ((dist in cm - 10cm off the ground + 2cm tolerance)/10.16* PI)*1440;
int BUTTON_SCORE  = 1;
int liftButton = 2;
int HIGH_GOAL_HEIGHT = ((90-8)/10.16*PI)*1440;
int CENTER_GOAL_HEIGHT = ((120-8)/10.16*PI)*1440;


int height = 1;
int scorePos = 1;
int pos = 0;

void lift(tMotor lifter1, tMotor lifter2, int pos){
	//syntax: nMotorEncoderTarget(lifter) == targetTicks - nMotorEncoder(lifter);
	switch(pos){
		case 0: nMotorEncoderTarget(lifter1) == -nMotorEncoder(lifter1); nMotorEncoderTarget(lifter2) == -nMotorEncoder(lifter2); break; //bottom
		case 1: nMotorEncoderTarget(lifter1) == (HIGH_GOAL_HEIGHT-nMotorEncoder(lifter1)); nMotorEncoderTarget(lifter2) == (HIGH_GOAL_HEIGHT-nMotorEncoder(lifter2)); break; //high goal
		case 2: nMotorEncoderTarget(lifter1) == (CENTER_GOAL_HEIGHT-nMotorEncoder(lifter1)); nMotorEncoderTarget(lifter2) == (CENTER_GOAL_HEIGHT-nMotorEncoder(lifter2)); break; //center goal
	}
}

task encodeLift()
{
	while(true)
	{
		if(joy2Btn(liftButton))
		{
				height++;
			switch(height % 3)
			{
				case 1: lift(motor[motorLift1], motor[motorLift2], 0); break; //0 is botto
				case 2: lift(motorLift1, motorLift2, 1); break; //1 is high goal
				case 0: lift(motorLift1, motorLift2, 2); break; //2 is center goal
			}

		}

	}

}
