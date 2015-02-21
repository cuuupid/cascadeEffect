
 void turn degrees(tMotor leftMotor, tMotor rightMotor, int degrees wanted)
{
	//take initial reading
	int initial = SensorValue[gyro]; //the initial sensor value of the gyro
	int gyroReading = SensorValue[gyro]- initial; //final-intial
	int degreesTurned = gyroReading*.01;
	/*d=rt, unit of gyroReading (r) is degrees/time * time,
	eliminates time, so degreesTurned (d) is in degrees*/

	//start turning
	motor[rightMotor] = 15;
	motor[leftMotor] = -15;

	//check if turned enough
	while(degreesTurned<degreesWanted)
	{
		wait1Msec(10); //continue tuning for .01 sec, then read again
	}

	//stop turning
	motor[leftMotor] = 0;//when degreesTurned=degreesWanted, stop
	motor[rightMotor] = 0;



}
