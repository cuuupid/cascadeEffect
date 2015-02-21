//GLOBAL VARIABLES
float sonarValue; //this is the sensor facing the ramp
float sonarValue2; //this is the sensor facing the back
float goalWidth;
//

//put this in getHeading
sonarValue = SensorValue[Sonar];

//
task stopMotors() {
	motor[frontleft] = 0;
	motor[backleft] = 0;
	motor[backright] = 0;
	motor[frontright] = 0;
}

task waitSeconds(float x){
	wait1Msec(x*1000);
}

task movePlain(char a){
	if(a = 'r'){
		motor[frontleft] = -50;
		motor[frontright] = -50;
		motor[backleft] = 50;
		motor[backright] = 50;
	}
	if(a = 'l'){
		motor[frontleft] = 50;
		motor[frontright] = 50;
		motor[backleft] = -50;
		motor[backright] = -50;
	}
	if(a = 'f'){
		motor[frontleft] = 50;
		motor[frontright] = -50;
		motor[backleft] = 50;
		motor[backright] = -50;
	}
	if(a = 'b'){
		motor[frontleft] = -50;
		motor[frontright] = 50;
		motor[backleft] = -50;
		motor[backright] = 50;
	}
}

task moveToGoal(){
	movePlain('l');

	while(sonarValue < 10){
		wait1Msec(10);
	}

	movePlain('f');

	while(sonarValue2 < DISTANCE_IN_CM_FROM_BACKBOARD_TO_SENSOR2_WHEN_PAST_RAMP){
		wait1Msec(10);
	}

	movePlain('l');

	while(sonarValue < 10){
		wait1Msec(10);
	}

	//turn 90 degrees clockwise

	movePlain('r');

	while(sonarValue < goalWidth){
		wait1Msec(10);
	}

	stopMotors();

	//clamp_side

	//turn 90 degrees clockwise
	movePlain('b');

	while(sonarValue2 < goalWidth){
		wait1Msec(10);
	}

	stopMotors();

	//clamp_back




	//come back
}
