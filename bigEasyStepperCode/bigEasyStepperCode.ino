//bigEasyStepperCode.ino

#define TOTAL_DISTANCE	1727	//mm
#define TOTAL_NUM_STEPS 230000	//(Total distance/lead screw travel distance per rev) * Steps/rev
#define EN 		2
#define MS1 	3
#define MS2 	4
#define MS3		5
#define STEP 	A1
#define DIR 	A0

int led = 13;
unsigned long distance = 0;
bool dir;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);

  pinMode(EN, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  resetBEDPins();
}

void loop() {
	if(distance >= TOTAL_DISTANCE)
	{
		delay(5000);
		dir = !dir; //For reverse
		digitalWrite(DIR, dir); //Pull direction pin high/low to move in reverse/forward respectively
		distance = 0;
	}

	oneRev();
	distance += 3;	//Each rev equals 3mm travel distance of leadscrew
}

void resetBEDPins()
{
  digitalWrite(STEP, LOW);
  digitalWrite(DIR, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(EN, HIGH);
}

//Default microstep mode function
void oneRev()
{
	digitalWrite(EN, LOW);
  for(int x= 1; x<400; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(STEP,HIGH); //Trigger one step forward
    delayMicroseconds(600);
    digitalWrite(STEP,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(600);
  }
}

