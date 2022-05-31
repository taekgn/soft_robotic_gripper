// Pin definition
const int IN1_A = 5;
const int IN2_A = 6;

const int IN1_B = 7;
const int IN2_B = 8;

const int VALVE = 9;

const byte inflbtn = 2;     // the number of the pushbutton pin for interrupt
const byte deflbtn = 3;     // the number of the pushbutton pin for interrupt

const int red = 51;
const int green = 53;
const int yel = 41;

volatile byte vst = 0;
void setup()
{
  // Used to display information
  Serial.begin(9600);

  pinMode(inflbtn, INPUT_PULLUP);
  pinMode(deflbtn, INPUT_PULLUP);

  pinMode(IN1_A, OUTPUT);
  pinMode(IN2_A, OUTPUT);
  pinMode(IN1_B, OUTPUT);
  pinMode(IN2_B, OUTPUT);
  pinMode(VALVE, OUTPUT);

  pinMode(red, OUTPUT); //Red
  pinMode(yel, OUTPUT); //Red
  pinMode(green, OUTPUT); //Green

  attachInterrupt(digitalPinToInterrupt(inflbtn), inflate, RISING);
  attachInterrupt(digitalPinToInterrupt(deflbtn), deflate, RISING);
  // Wait for Serial Monitor to be opened
  while (!Serial)
  {
    //do nothing
  }
}

void inflate() {
  //Function is called by left button and executes left side motor.
  Serial.println("VALVE HIGH");
  digitalWrite(red, HIGH);
  digitalWrite(IN1_A, HIGH); digitalWrite(IN2_A, LOW);
  digitalWrite(IN1_B, LOW); digitalWrite(IN2_B, LOW);
  //digitalWrite(IN1_B, LOW); digitalWrite(IN2_B, HIGH);
}

void deflate() {
  //Function is invoked by right button and executes right side motor.
  Serial.println("VALVE LOW ");
  digitalWrite(green, HIGH); // Red on
  //digitalWrite(IN1_B, LOW); digitalWrite(IN2_B, HIGH);
  //digitalWrite(IN1_A, LOW); digitalWrite(IN2_A, LOW);
  digitalWrite(IN1_A, LOW); digitalWrite(IN2_A, LOW);
  digitalWrite(IN1_B, HIGH); digitalWrite(IN2_B, LOW);
}

void dormant() {
  //Turns off all motors and leds except led for valve.
  digitalWrite(IN1_A, LOW); digitalWrite(IN2_A, LOW);
  digitalWrite(IN1_B, LOW); digitalWrite(IN2_B, LOW);
  digitalWrite(red, LOW); digitalWrite(green, LOW);
}

/**
 * Due to structure of Solenoid valve, it was programmed in Active-Low synchronization.
 * When valve is opened, air can't pass through it, on the otherhand air can pass through if valve is closed.
 * Solenoid valve is like a voltage relay.
 */

void valveon() {
  vst = 1;
  digitalWrite(VALVE, vst);       //Switch Solenoid ON
  digitalWrite(yel, LOW);
}

void valveoff() {
  vst = 0;
  digitalWrite(VALVE, vst);       //Switch Solenoid OFF
  digitalWrite(yel, HIGH);
}

/**
 * Push buttons interrupt in Active-Low way.
 * Due to different power supply, Active-High way had some trouble; 14V for motors and 5V for other components but they are common grounded
 * So, Active-Low signals were generated when push button was tackled, it shorts the circuit from 5V to GND.
 * 
 * Gripper respones in dynamic(real-time) way, because static way(time-locked) was suitable for same size objects.
 * Thus, response should be in dynmaic, In order to grasp objects in various weight, volume and gripping should be inspected by seeing.
 */

void loop()
{
  int  inflst = digitalRead(inflbtn);
  int  deflst = digitalRead(deflbtn);
  valveon();
  if (inflst == LOW) {
    Serial.println("Infl Switched ON");
    valveoff();
    inflate();
    delay(50);
  }
  else if (inflst == HIGH && deflst == HIGH) {
    dormant();
    valveon();
  }
  if ( deflst == LOW) {
    Serial.println("Defl Switched ON");
    valveoff();
    deflate();
    delay(50);
  }
  delay(5);
}
