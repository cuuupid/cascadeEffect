float stepCount;

task getPinOn(){

	//get pin on (use while true with several if statements
	while(true){
		if(pinOne = 1){
			stepCount++;
		}
		wait1MSec(3);
	}


}

task setStep () {
	while(true){

		switch(stepCount){

			case 1: doSomething; break;
			case 2: doSomething; break;
			case 3: doSomething; break;
			case 4: doSomething; break;
			case 5: doSomething; break;
			case 6: doSomething; break;
			case 7: doSomething; break;
			case 8: doSomething; break;
			case 9: doSomething; break;
			//continue with this logic trend

		}
}


task main()
{

StartTask(getPinOn);
StartTask(stepStep);

}
