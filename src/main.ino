// EnableInterrupt Simple example sketch. Demonstrates operation on a single pin of your choice.
// See https://github.com/GreyGnome/EnableInterrupt and the README.md for more information.
#include <EnableInterrupt.h>

// Modify this at your leisure. Refer to https://github.com/GreyGnome/EnableInterrupt/wiki/Usage#Summary
#define ARDUINOPIN 10

volatile uint16_t interruptCount=0; // The count will go back to 0 after hitting 65535.

void interruptFunction() {
  interruptCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(ARDUINOPIN, INPUT_PULLUP);  // See http://arduino.cc/en/Tutorial/DigitalPins
  enableInterrupt(ARDUINOPIN, interruptFunction, CHANGE);
}

// In the loop we just display interruptCount. The value is updated by the interrupt routine.
void loop() {
  Serial.println("---------------------------------------");
  delay(1000);
  Serial.print("Pin was interrupted: ");
  Serial.print(interruptCount, DEC);
  Serial.println(" times so far.");
}
