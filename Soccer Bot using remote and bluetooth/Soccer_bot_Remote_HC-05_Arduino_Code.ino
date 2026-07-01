/*
"Hello everyone! 
PROJECT TITLE: Hybrid Remote & Bluetooth Soccer Bot 
 CONTROLLERS : FlySky i6X (i-BUS) & HC-05 Bluetooth
 FEATURES    : Auto-Switching, Zero-Lag, Sharp Diagonal Turns (0.1 Multiplier)
This code is specifically made for the FlySky i6X remote
 and a Bluetooth app to drive a dual-control soccer bot."
"It lets you drive smoothly with both a remote and Bluetooth,
automatically switching between them without any lag."
"Warning: Always unplug the RX pin from the Arduino before uploading the code
 or it will fail!"
 Bluetooth configuration D3 = RX, D2 = TX
 */
#include <IBusBM.h>
#include <SoftwareSerial.h>

IBusBM IBus; 

// Bluetooth configuration (D2 = RX, D3 = TX)
SoftwareSerial Bluetooth(3, 2); 

// Left motor pins
const int LPWM_L = 6;
const int RPWM_L = 5;
// Right motor pins
const int LPWM_R = 10;
const int RPWM_R = 9;

const int DEADZONE = 40;     
char btCommand;             
int btSpeed = 190;          

void setup() {
  IBus.begin(Serial);
  Bluetooth.begin(9600); 

  pinMode(LPWM_L, OUTPUT);
  pinMode(RPWM_L, OUTPUT);
  pinMode(LPWM_R, OUTPUT);
  pinMode(RPWM_R, OUTPUT);

  stopMotors();
}

void loop() {
  int xVal = IBus.readChannel(0); 
  int yVal = IBus.readChannel(1); 
  int throttleVal = IBus.readChannel(2); 

  
  if (xVal >= 1000 && yVal >= 1000 && throttleVal >= 1000) {
    float maxSpeedFactor = map(throttleVal, 1000, 2000, 0, 100) / 100.0;
    
    int moveX = xVal - 1500;
    int moveY = yVal - 1500;

    if (abs(moveX) < DEADZONE) moveX = 0;
    if (abs(moveY) < DEADZONE) moveY = 0;

    int leftSpeed = moveY + moveX;
    int rightSpeed = moveY - moveX;

    leftSpeed = map(leftSpeed, -500, 500, -255, 255) * maxSpeedFactor;
    rightSpeed = map(rightSpeed, -500, 500, -255, 255) * maxSpeedFactor;

    controlLeftMotor(constrain(leftSpeed, -255, 255));
    controlRightMotor(constrain(rightSpeed, -255, 255));
  } 
  // 
  else {
    if (Bluetooth.available() > 0) {
      btCommand = Bluetooth.read(); 
      
     // Speed Control Logic via App Slider
      if (btCommand == '0') btSpeed = 0;
      else if (btCommand == '1') btSpeed = 25;
      else if (btCommand == '2') btSpeed = 51;
      else if (btCommand == '3') btSpeed = 76;
      else if (btCommand == '4') btSpeed = 102;
      else if (btCommand == '5') btSpeed = 127;
      else if (btCommand == '6') btSpeed = 153;
      else if (btCommand == '7') btSpeed = 178;
      else if (btCommand == '8') btSpeed = 204;
      else if (btCommand == '9') btSpeed = 230;
      else if (btCommand == 'q') btSpeed = 255;
      
      // Movement Commands
      if (btCommand == 'F') {        
        controlLeftMotor(btSpeed);  controlRightMotor(btSpeed);
      } 
      else if (btCommand == 'B') {  
        controlLeftMotor(-btSpeed); controlRightMotor(-btSpeed);
      } 
      else if (btCommand == 'L') {  
        controlLeftMotor(-btSpeed); controlRightMotor(btSpeed);
      } 
      else if (btCommand == 'R') {  
        controlLeftMotor(btSpeed);  controlRightMotor(-btSpeed);
      } 
      
      else if (btCommand == 'G') {  
        controlLeftMotor(btSpeed * 0.1);  controlRightMotor(btSpeed);
      } 
      else if (btCommand == 'I') {  
        controlLeftMotor(btSpeed);        controlRightMotor(btSpeed * 0.1);
      } 
      else if (btCommand == 'H') {  
        controlLeftMotor(-btSpeed * 0.1); controlRightMotor(-btSpeed);
      } 
      else if (btCommand == 'J') {  
        controlLeftMotor(-btSpeed);       controlRightMotor(-btSpeed * 0.1);
      } 
      else if (btCommand == 'S' || btCommand == 'D') { 
        stopMotors();
      }
    }
  }
}

// Left Motor Control Function
void controlLeftMotor(int speed) {
  if (speed > 0) { 
    analogWrite(RPWM_L, speed);  
    analogWrite(LPWM_L, 0);      
  } else if (speed < 0) { 
    analogWrite(RPWM_L, 0); 
    analogWrite(LPWM_L, abs(speed)); 
  } else { 
    analogWrite(RPWM_L, 0); 
    analogWrite(LPWM_L, 0); 
  }
}

// Right Motor Control Function
void controlRightMotor(int speed) {
  if (speed > 0) { 
    analogWrite(RPWM_R, speed);  
    analogWrite(LPWM_R, 0);      
  } else if (speed < 0) { 
    analogWrite(RPWM_R, 0); 
    analogWrite(LPWM_R, abs(speed)); 
  } else { 
    analogWrite(RPWM_R, 0); 
    analogWrite(LPWM_R, 0); 
  }
}
// Stop All Motors
void stopMotors() {
  analogWrite(RPWM_L, 0); analogWrite(LPWM_L, 0);
  analogWrite(RPWM_R, 0); analogWrite(LPWM_R, 0);
}

/*
  HOW TO INSTALL THE LIBRARIES (FOR BEGINNERS):
 *  IBusBM Library (Required for FlySky i6X):
 *  Open Arduino IDE.
 * Click on: Sketch -> Include Library -> Manage Libraries...
 *  In the search bar, type: "IBusBM"
 *  Find the library by "BM" and click "Install".
 *  SoftwareSerial Library (Required for Bluetooth):
 *  No need to install anything! 
 *  It is already built-in inside the Arduino software.
 */