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
