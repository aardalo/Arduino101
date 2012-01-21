#define ledPin 13                         // LED connected to pin 13
#define buttonPin 2                       // button connected to pin 2

void setup() {                
  pinMode(ledPin, OUTPUT);                // write to LED pin
  pinMode(buttonPin, INPUT);              // read from button pin
}

void ledSoftOn(byte pin) {                // Soft-on function for LED
  for (int i = 1 ; i < 10000; i+=100) {   // 10000 is trial an succed number, 100 sets speed from 0 to full
    digitalWrite(pin, LOW);               // start with LED off
    delayMicroseconds(10000 - i);         // off-time decreases with i
    digitalWrite(pin, HIGH);              // then LED on
    delayMicroseconds(i);                 // on-time increases with i - smooth transition
  }
}    

void ledSoftOff(byte pin) {              // same as On just the other way
  for (int i = 1 ; i < 10000; i+=100) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(10000 - i);
    digitalWrite(pin, LOW);
    delayMicroseconds(i);
  }
}

boolean lastState = LOW;                 // a place to remember the last state

void loop() {
  int state = digitalRead(buttonPin);    // read the current state of the button on buttonPin
  if (state == HIGH) {                   // if it's high, something's happening
    while (state == HIGH) {              // while it's still pressed
      state = digitalRead(2);            // check state
    }                                    // and there it was released...

    if (lastState == LOW) {              // if the LED was OFF
      ledSoftOn(ledPin);                 // turn it on
      lastState = HIGH;                  // and remember for next time
    } 
    else {                               
      ledSoftOff(ledPin);                // turn it off  
      lastState = LOW;                   // and remember
    }
  }                                      // done processing the click
}                                        // goto loop()


