//ultrasonic sensor
int trigPin = 6;    // Trigger
int echoPin = 7;    // Echo

//stepper
int stepPin = 4;
int dirPin = 3;
int enPin = 5;

long stepSpeed = 400 ;

//buttons
const int twoSwitch = 8;
const int fourSwitch = 9;
const int bottomSwitch = 10;
const int upSwitch = 11;
const int dwnSwitch = 12;
const int limitSwitch = 13;


//distance vars
long fixedPos = 100;
long twoIn = 22;
long fourIn = 73;

long twoInch = fixedPos + twoIn;
long fourInch = fixedPos + fourIn;

long stepsPermm = 200;

//random vars
int findSteps;
int reqSteps;
long demandPos;
long currentPos;
long sensorRead;
double dist1;
double Analog;
double Sum = 0;

//debounce
bool lastButton = false;
bool currentButton = false;

bool lastButton2 = false;
bool currentButton2 = false;

bool lastButton1 = false;
bool currentButton1 = false;

void setup() {

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);

  digitalWrite(enPin, HIGH);

  pinMode(twoSwitch, INPUT);
  pinMode(fourSwitch, INPUT);
  pinMode(bottomSwitch, INPUT);
  pinMode(upSwitch, INPUT);
  pinMode(dwnSwitch, INPUT);
  pinMode(limitSwitch, INPUT);

}


void loop() {

  //read manual control buttons
  bool up = digitalRead(upSwitch);
  bool dwn = digitalRead(dwnSwitch);

  //debounce
  currentButton = debounce(lastButton);
  currentButton2 = debounce2(lastButton2);
  currentButton1 = debounce1(lastButton1);

  //2 inch focuse button hit
  if (lastButton == LOW && currentButton == HIGH) {


    demandPos = twoInch;
    currentPos = distAvg(sensorRead);
    findSteps = (demandPos - currentPos) * stepsPermm;
    reqSteps = abs(findSteps);

    Serial.print("demand position: ");
    Serial.print(demandPos);
    Serial.println();
    Serial.print("current position: ");
    Serial.print(currentPos);
    Serial.println();
    Serial.print("steps required: ");
    Serial.print(reqSteps);
    Serial.println();
    Serial.println();

    if (demandPos > currentPos) {

      digitalWrite(dirPin, HIGH);
      while (reqSteps > 0) {
        moveStepper();
        reqSteps = reqSteps - 1;
      }

      if (reqSteps <= 0) {
        currentPos = demandPos;
      }

    }


    if (demandPos < currentPos) {

      digitalWrite(dirPin, LOW);
      while (reqSteps > 0) {
        moveStepper();
        reqSteps = reqSteps - 1;
      }
      if (reqSteps <= 0) {
        currentPos = demandPos;
      }
    }
    if (demandPos = currentPos) {

    }
    Serial.print("2in");
    Serial.println();

  }

  //four inch button hit
  if (lastButton2 == LOW && currentButton2 == HIGH) {

    demandPos = fourInch;
    currentPos = distAvg(sensorRead);
    findSteps = (demandPos - currentPos) * stepsPermm;
    reqSteps = abs(findSteps);

    Serial.print("demand position: ");
    Serial.print(demandPos);
    Serial.println();
    Serial.print("current position: ");
    Serial.print(currentPos);
    Serial.println();
    Serial.print("steps required: ");
    Serial.print(reqSteps);
    Serial.println();
    Serial.println();

    if (demandPos > currentPos) {

      digitalWrite(dirPin, HIGH);
      while (reqSteps > 0) {
        moveStepper();
        reqSteps = reqSteps - 1;
      }

      if (reqSteps <= 0) {
        currentPos = demandPos;
      }
    }


    if (demandPos < currentPos) {

      digitalWrite(dirPin, LOW);
      while (reqSteps > 0) {
        moveStepper();
        reqSteps = reqSteps - 1;
      }
      if (reqSteps <= 0) {
        currentPos = demandPos;
      }
    }
    if (demandPos = currentPos) {

    }
    Serial.print("4in");
    Serial.println();

  }

  //bed to bottom button hit
  if (lastButton1 == LOW && currentButton1 == HIGH) {

    bool limitPos = digitalRead(limitSwitch);

    digitalWrite(dirPin, LOW);
    while (limitPos == HIGH) {
      moveStepper();
      limitPos = digitalRead(limitSwitch);
    }
    Serial.print("bottom");
    Serial.println();

  }

  while (up == true) {
    digitalWrite(dirPin, HIGH);
    moveStepper();
    up = digitalRead(upSwitch);
  }

  while (dwn == true) {
    digitalWrite(dirPin, LOW);
    moveStepper();
    dwn = digitalRead(dwnSwitch);
  }


  //debounce
  lastButton = currentButton;
  lastButton2 = currentButton2;
  lastButton1 = currentButton1;

}

// run ultrasonic sensor & convert to mm
double senseDistance(double dist) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  double duration = pulseIn(echoPin, HIGH);

  dist = (duration / 2) * .343;

  return dist;

}

//take average of a lot of readings
long distAvg(double avg) {

  for (int i = 0; i < 1500; i++)
  {
    Analog = senseDistance(dist1);
    Sum = Sum + Analog;
  }
  avg = Sum / 1500;
  Sum = 0;
  return avg;
}

//move stepper one step
void moveStepper() {

  digitalWrite(stepPin, HIGH);
  delayMicroseconds(stepSpeed);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(stepSpeed);

}


//debounce buttons
boolean debounce (boolean last)
{
  boolean current = digitalRead(twoSwitch);
  if (last != current);
  {
    delay(5);
    digitalRead(twoSwitch);
  }
  return (current);
}

boolean debounce2 (boolean last2)
{
  boolean current2 = digitalRead(fourSwitch);
  if (last2 != current2);
  {
    delay(5);
    digitalRead(fourSwitch);
  }
  return (current2);
}

boolean debounce1 (boolean last1)
{
  boolean current1 = digitalRead(bottomSwitch);
  if (last1 != current1);
  {
    delay(5);
    digitalRead(bottomSwitch);
  }
  return (current1);
}
