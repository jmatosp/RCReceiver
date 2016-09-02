#include <PinChangeInt.h>    // http://playground.arduino.cc/Main/PinChangeInt
#include <PinChangeIntConfig.h>
#include <TimerOne.h>        // http://playground.arduino.cc/Code/Timer1

#define NO_PORTB_PINCHANGES             // PinChangeInt setup
#define NO_PORTC_PINCHANGES             // only port D pinchanges (see: http://playground.arduino.cc/Learning/Pins)
#define PIN_COUNT 1                     // number of channels attached to the receiver
#define MAX_PIN_CHANGE_PINS PIN_COUNT

#define RC_THROTTLE 3         // Arduino pins attached to the receiver
byte pin[] = {RC_THROTTLE};   // for maximum efficiency these pins should be attached
unsigned int time[] = {0};    // to the receiver's channels in the order listed here

byte state = 0;
byte burp = 0;    // a counter to see how many times the int has executed
byte cmd = 0;     // a place to put our serial data
byte i = 0;       // global counter for tracking what pin we are on

void fall() {
    state = FALLING;
    time[i] = Timer1.read();   // read the time since timer1 was restarted
    Timer1.stop();
}

void rise() {
    Timer1.restart();           // set our stopwatch to 0
    Timer1.start();             // and start it up
    state = RISING;
    burp++;
}

void setup() {
    Serial.begin(115200);
    Serial.print("PinChangeInt ReciverReading test");
    Serial.println();            // warm up the serial port

    Timer1.initialize(2200);        // longest pulse in PPM is usually 2.1 milliseconds,
                                    // pick a period that gives you a little headroom.
    Timer1.stop();                  // stop the counter
    Timer1.restart();               // set the clock to zero

    for (byte i = 0; i < PIN_COUNT; i++) {
        pinMode(pin[i], INPUT);         // set the pin to input
        digitalWrite(pin[i], HIGH);     // use the internal pullup resistor
    }
    PCintPort::attachInterrupt(pin[i], rise, RISING); // attach a PinChange Interrupt to our first pin
}

void loop() {
    cmd = Serial.read();        // while you got some time gimme a systems report
    if (cmd == 'p') {
        Serial.print("time:\t");
        for (byte i = 0; i < PIN_COUNT; i++) {
            Serial.print(i,DEC);
            Serial.print(":");
            Serial.print(time[i], DEC);
            Serial.print("\t");
        }
        Serial.print(burp, DEC);
        Serial.println();
        Serial.print("\t");
        Serial.print(clockCyclesToMicroseconds(Timer1.pwmPeriod), DEC);
        Serial.print("\t");
        Serial.print(Timer1.clockSelectBits, BIN);
        Serial.print("\t");
        Serial.println(ICR1, DEC);
    }
    cmd = 0;
    switch (state) {
        case RISING: // we have just seen a rising edge
            PCintPort::detachInterrupt(pin[i]);
            PCintPort::attachInterrupt(pin[i], fall, FALLING); // attach the falling end
            state = 255;
            break;
        case FALLING: // we just saw a falling edge
            PCintPort::detachInterrupt(pin[i]);
            i++;                // move to the next pin
            i = i % PIN_COUNT;  // i ranges from 0 to PIN_COUNT
            PCintPort::attachInterrupt(pin[i], rise, RISING);
            state = 255;
            break;
    }
}
