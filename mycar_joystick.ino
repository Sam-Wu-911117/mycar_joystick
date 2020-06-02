#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <Servo.h>

AF_DCMotor motor(3);

const int rxpin = A0; // 接收 pin
const int txpin = A1; // 發送 pin
SoftwareSerial bluetooth(rxpin, txpin);
Servo frontservo;
int angle = 0;
bool isFlashing = false;
  
void setup() {
  motor.setSpeed(200);
  motor.run(RELEASE);
  
  Serial.begin(9600);
  Serial.setTimeout(100);
  Serial.println("Serial ready");
  bluetooth.begin(9600);
  bluetooth.setTimeout(100);
  Serial.println("Bluetooth ready");

  frontservo.attach(10);
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);
  pinMode(A3,OUTPUT);
  digitalWrite(A3,LOW);
}
  
void loop() {
  int command = GetCommand();  
  ExecCommand(command);
}

int GetCommand() {
  int command = bluetooth.parseInt();
  return command;
}

void ExecCommand(int command){

  if(command==1) {
    motor.run(RELEASE);
    Serial.println("stop");
    analogWrite(A2,255);
    analogWrite(A3,255);
    isFlashing = false;
  }
  else if(command==2) {
    motor.run(FORWARD);
    Serial.println("forward");
    analogWrite(A2,0);
    analogWrite(A3,0);
    isFlashing = false;
  }
  else if(command==3) {
    motor.run(BACKWARD);
    isFlashing = true;
    Serial.println("backward");
  }
  else if (command==4){
    int angle = bluetooth.parseInt();
    Serial.println(angle); 
    frontservo.write(angle);
    isFlashing = false;
   }
  if(isFlashing) {
      flash(); 
  }
 
}

void flash(){

  static bool LedStatus = LOW;
  static unsigned long lasttime = 0;
  unsigned long thistime = millis(); 
    if(thistime-lasttime>500){
      LedStatus = !LedStatus;
      digitalWrite(A2, LedStatus);
      digitalWrite(A3, LedStatus); 
      lasttime = thistime;
    }
}         
