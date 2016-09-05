
#include <EnableInterrupt.h>    // See https://github.com/GreyGnome/EnableInterrupt and the README.md for more information.
#include <TimerOne.h>           // http://playground.arduino.cc/Code/Timer1

#define ARDUINOPIN 10

volatile uint16_t interruptCount=0;     // The count will go back to 0 after hitting 65535.

unsigned int time[] = {0};

void rising() {
    Timer1.restart();           // set our stopwatch to 0
    Timer1.start();             // and start it up
    time[0] = Timer1.read();   // read the time since timer1 was restarted
    interruptCount++;
    enableInterrupt(ARDUINOPIN, falling, FALLING);
}

void falling() {
    time[0] = Timer1.read();   // read the time since timer1 was restarted
    Timer1.stop();
}

void setup() {
    Serial.begin(115200);

    Timer1.initialize(2200);
    Timer1.stop();
    Timer1.restart();

    pinMode(ARDUINOPIN, INPUT_PULLUP);
    enableInterrupt(ARDUINOPIN, rising, RISING);
}

// In the loop we just display interruptCount. The value is updated by the interrupt routine.
void loop() {
  Serial.println("---------------------------------------");
  delay(1000);
  Serial.print("Pin was interrupted: ");
  Serial.print(interruptCount, DEC);
  Serial.println(" times so far.");
}
