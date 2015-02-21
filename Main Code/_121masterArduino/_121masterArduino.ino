int stepCounter;
int plan = 1;

int northSonar; //Sonar Value
int eastSonar;
int southSonar;
int westSonar;

int nextStepPin = 6;
int previousStepPin = 7;
int switchPlanPin = 8;

void setup() {
  pinMode(nextStepPin, OUTPUT);
  pinMode(previousStepPin, OUTPUT);
  pinMode(switchPlanPin, OUTPUT);
}

void flashPin (int pin) {
  digitalWrite(pin, HIGH);
  delay(10);
  digitalWrite(pin, LOW);
}

void nextStep () {
  stepCounter++;
  flashPin(nextStepPin);
}

void previousStep () {
  stepCounter++;
  flashPin(previousStepPin);
}

void switchPlan () {
  plan = 2;
  flashPin(switchPlanPin);
}

void moveFromSideBarrier() {
  if (southSonar > 50) {//placeholder
    nextStep();
  }
}

void loop() {
  switch (stepCounter) {
    case 1: moveFromSideBarrier(); break;
    case 2: break;
  }
  delay(1);
}
