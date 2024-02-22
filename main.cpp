#include <Arduino.h>

void setup(){
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT); // G
  pinMode(A6,OUTPUT);
  pinMode(A7,OUTPUT);
  pinMode(A8,OUTPUT);
  pinMode(A9,OUTPUT);
  pinMode(A10,OUTPUT); // B
  pinMode(A11,OUTPUT);
  pinMode(A12,OUTPUT);
  pinMode(A13,OUTPUT);
  pinMode(A14,OUTPUT);
  pinMode(A15,OUTPUT);
}

void loop(){
  analogWrite(A0,240); //
  analogWrite(A5,148);
  analogWrite(A10,36);
  analogWrite(A1,0); //
  analogWrite(A6,128);
  analogWrite(A11,0);
  analogWrite(A2,128);   //
  analogWrite(A7,128);
  analogWrite(A12,0);
  analogWrite(A3,128); //
  analogWrite(A8,0);
  analogWrite(A13,128);
  analogWrite(A4,0); //
  analogWrite(A9,128);
  analogWrite(A14,128);
  delay(500);
  analogWrite(A0,0); //
  analogWrite(A5,0);
  analogWrite(A10,0);
  analogWrite(A1,0); //
  analogWrite(A6,0);
  analogWrite(A11,0);
  analogWrite(A2,0);   //
  analogWrite(A7,0);
  analogWrite(A12,0);
  analogWrite(A3,0); //
  analogWrite(A8,0);
  analogWrite(A13,0);
  analogWrite(A4,0); //
  analogWrite(A9,0);
  analogWrite(A14,0);
  delay(500);
}