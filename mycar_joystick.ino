#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <Servo.h>
AF_DCMotor motor(3);

  const int rxpin = A0; // 接收 pin
  const int txpin = A1; // 發送 pin
  SoftwareSerial bluetooth (rxpin, txpin);
  Servo frontservo;
  int angle = 0;
  bool isFlashing = false;
  int Speed =0;
  
void setup(){
  motor.setSpeed(200);
  motor.run(RELEASE);
  
  Serial.begin(9600);
  Serial.setTimeout(100);
  Serial.println("Serial ready");
  bluetooth.begin(9600);
  bluetooth.setTimeout(100);
  Serial.println("Bluetooth ready");

  frontservo.attach(9);
  frontservo.write(90);
  delay(500);
  frontservo.detach();
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);
  pinMode(A3,OUTPUT);
  digitalWrite(A3,LOW);
}
  
void loop() {
  int command = GetCommand();  
  if(command>0) {
    ExecCommand(command);
  }
  frontservo.attach(9);
  
  int cangle = frontservo.read();
  if(cangle!=angle) {
    frontservo.write(angle);
    delay(30);
  }
  frontservo.detach();
}

int GetCommand() {
  int command = 0;
  if(bluetooth.available()) {
    command = bluetooth.parseInt();
    if(command>0) {
      Serial.print("cmd:");
      Serial.println(command);
    }
    //int command = Serial.parseInt();
  }
  return command;
}

void ExecCommand(int command){
 if(command==1) {
    motor.run(RELEASE);
    Serial.println("stop");
    analogWrite(A2,0);
    analogWrite(A3,0);
    isFlashing = false;
  }
  else if(command==2) {
    int Speed = bluetooth.parseInt();
    //int Speed = Serial.parseInt();
    Serial.println(Speed);
    //motor.setSpeed(200);  
    Serial.println("forward");
    motor.run(FORWARD);
    analogWrite(A2,255);
    analogWrite(A3,255);
    isFlashing = false;
  }
  else if(command==3) {
    motor.run(BACKWARD);
    int Speed = bluetooth.parseInt();
    Serial.println(Speed);
    motor.setSpeed(Speed);
    isFlashing = true;
    Serial.println("backward");
    
  }
  else if (command==4){
    angle = bluetooth.parseInt();
    Serial.println(angle); 
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
