#define ledPin 11                         // LED connected to pin 13
#define buttonPin 2                       // button connected to pin 2

void setup() {                
  pinMode(ledPin, OUTPUT);                // write to LED pin
  pinMode(buttonPin, INPUT);              // read from button pin
  Serial.begin(115200);
}

unsigned long countNops = 0;

void nop() {
  countNops++;                            // Mmm... free goo.
}

void ledSoftOn(byte pin) {                // Soft-on function for LED
  for (int i = 0 ; i <= 255; i++) {       // 0-255 is zero to full duty cycle
    analogWrite(pin,i);                   // Enable PWM with i/255 duty cycle
    unsigned long now = millis();         // Kaeu?
    while ((millis() - now) < 5) {        // Wait approx 5 ms before next iteration
      nop();                              // Totally useless but could have been ...
    }
  }
}    

void ledSoftOff(byte pin) {               // same as On just the other way
  for (int i = 0 ; i <= 255; i++) {
    analogWrite(pin,255 - i);
    unsigned long now = millis();         // Kaeu?
    while ((millis() - now) < 5) {        // Wait approx 5 ms before next iteration
      nop();                              // Totally useless but could have been ...
    }
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

    Serial.println(countNops);           // Brag about useful stuff done
  }                                      // done processing the click
}                                        // goto loop()




