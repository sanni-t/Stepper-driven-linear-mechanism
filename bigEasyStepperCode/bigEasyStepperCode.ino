//bigEasyStepperCode.ino

#define TOTAL_DISTANCE	1650   	//mm
#define TOTAL_NUM_STEPS 230000	//(Total distance/lead screw travel distance per rev) * Steps/rev
#define RUN_INTERVAL    550
#define EN 		   2
#define MS1 	   3
#define MS2 	   4
#define MS3		   5
#define STEP 	  A1
#define DIR 	  A0
#define SWITCH   6

enum DIRECTION{
  ANTICLOCKWISE = 0,
  CLOCKWISE
};

const int led = 13;
unsigned long distance;
bool dir = (bool)ANTICLOCKWISE;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  resetBEDPins();
  Serial.println("BEDpins reset");
  resetPosition();
  Serial.println("Position initialized");
  distance = 0;
}

void loop() {
	if(distance >= TOTAL_DISTANCE || digitalRead(SWITCH) == HIGH)
	{
    Serial.print("Changing direction");
    stepDown();
		delay(5000);
		dir = !dir; //For reverse
		digitalWrite(DIR, dir); //Pull direction pin high/low to move in reverse/forward respectively
		distance = 0;
    stepUp();
	}

	oneRev();
	distance += 3;	//Each rev equals 3mm travel distance of leadscrew
}

void resetBEDPins()
{
  digitalWrite(STEP, LOW);
  digitalWrite(DIR, dir);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(EN, HIGH);
}

void resetPosition()
{
  digitalWrite(led, HIGH);
  
  if(digitalRead(SWITCH) != HIGH)
  {
    Serial.println("Starting motor");
    stepUp();
  }
  while(digitalRead(SWITCH) != HIGH)
  {
    oneRev();
  }
  Serial.println("End switch triggered");
  stepDown();
  dir = !dir; //For reverse
  digitalWrite(DIR, dir); //Pull direction pin high/low to move in reverse/forward respectively
  delay(1000);
  stepUp();
  
  digitalWrite(led, LOW);
}

void stepUp()
{
  digitalWrite(led, HIGH);

  digitalWrite(MS1, HIGH);  //Switch to half step
  digitalWrite(EN, LOW);
  for(int interval = 1500; interval >=RUN_INTERVAL; interval--)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(STEP,HIGH); //Trigger one step forward
    delayMicroseconds(RUN_INTERVAL);
    digitalWrite(STEP,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(interval);
  } 
  digitalWrite(MS1, LOW);   //Switch back to full step
  
  digitalWrite(led, LOW);
}

void stepDown()
{
  digitalWrite(led, HIGH);

  digitalWrite(MS1, HIGH);  //Switch to half step
  digitalWrite(EN, LOW);
  for(int interval = RUN_INTERVAL; interval <=1500; interval+=3)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(STEP,HIGH); //Trigger one step forward
    delayMicroseconds(RUN_INTERVAL);
    digitalWrite(STEP,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(interval);
  } 
  digitalWrite(MS1, LOW);   //Switch back to full step

  digitalWrite(led, LOW);
}
//Default microstep mode function
void oneRev()
{
	digitalWrite(EN, LOW);
  for(int x= 1; x<400; x++)  //Loop the forward stepping for one revolution
  {
    digitalWrite(STEP,HIGH); //Trigger one step forward
    delayMicroseconds(RUN_INTERVAL);
    digitalWrite(STEP,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(RUN_INTERVAL);
  }
}

