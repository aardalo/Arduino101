/*////////////////////////////////////////////////////////
/* Finite State Machine for Arduino
/*  - timed and interrupt driven state change
/*
/* v0.1  Øyvind Aardal    Bare bones
 */////////////////////////////////////////////////////////
#include <FiniteStateMachine.h>
#define DEBUG

boolean button1press = LOW;  // HIGH when button-press is detected
unsigned long cDisplay;      // count of calls to Display state
unsigned long cButtons;      // ...
unsigned long cIncoming;
unsigned long cLong;

#ifdef DEBUG
unsigned long cDisplayLast;  // Used to calculate frequenzy if DEBUG is #defined
unsigned long cButtonsLast;  //..
unsigned long cIncomingLast;
unsigned long cLongLast;
#endif

// Initialize states
const byte NUMBER_OF_STATES = 4;

State Display = State(runUpdateDisplay);    // State Display calls function runUpdateDisplay
State Buttons = State(runPollButtons);      //..
State Incoming = State(runPollIncoming);
State LongInterval = State(runLongInterval);

FSM fsmDemo = FSM(Display);                 // Start FSM in runUpdateDisplay state

unsigned long now = 0;                      // Used to hold time of entry
unsigned long lastlong = 0;                 // last time run for the long cycle state
unsigned long lastshort = 0;                // last time run for the short cycle state

void setup() {
  Serial.begin(115200);  // Don't waste time waiting for slow communications
  pinMode(2,INPUT);      // button connected to pin 2 with pull-down
  pinMode(11,OUTPUT);    // LED connected to pin 11 with current limiting resistor
  now = micros();        // initialize variables used to keep track of time
  lastlong = now;        // ..
  lastshort = now;
}

unsigned long freq = 0;  // counter to distribute non-timed states

void loop(){                                // the loop - time and sequence controlled state table
  freq++;                                   // sequencer
  now = micros();                           // timer

  if ((now-lastlong) >= 1000000){
    fsmDemo.transitionTo(LongInterval);      // do the long interval
    lastlong = micros();                     // store the time before executing state - that way we don't loose time
  } 
  else if ((now-lastshort) >= 100000) {      
    fsmDemo.transitionTo(Display);           // do the short interval
    lastshort = micros();                    // store the time before executing state - that way we don't loose time
  } 
  else if (freq % 4 == 0) {                  // empty the UART max every 1 in 4 iterations less long/short states
    fsmDemo.transitionTo(Incoming);
  } 
  else {
    fsmDemo.transitionTo(Buttons);           // Spend the most time (3 in 4 iterations) looking for button presses
  };

  fsmDemo.update();                          // call the FSM executer
}

boolean ledState = LOW;                      // To keep track of LED state without having to read the pin

void runUpdateDisplay(){                     // Update display
  cDisplay++;                                // count number of calls
  if (button1press == HIGH) {                // ...if button press was detected
    ledState = !ledState;                    // swap state
    digitalWrite(11,ledState);               // and write the new state to the led
    button1press = LOW;                      // done processing this button press - prepare for the next
  };
}

boolean button1in = LOW;                     // To keep trak of when a button is in the pressed state

void runPollButtons(){                       // Polls the buttons
  cButtons++;                                // counting the number of calls

  if (button1in == HIGH) {                   // If the button tracking is in the pressed state
    if (digitalRead(2) == LOW){              // and the pin is low - it has been released - PS: Just lucky debounce due to the release is detected in the next FSM cycle
      button1in = LOW;                       // it's not pressed anymore
      button1press = HIGH;                   // but a press has been detected
    };
  } 
  else {                                     // button tracking is in depressed state
    if (digitalRead(2) == HIGH){             // but the pin is HIGH, soo....
      button1in = HIGH;                      // set the button tracking to pressed state
    };
  };
};

void runPollIncoming(){                      // booooring
  cIncoming++;                               // counting the number of calls
};

void runLongInterval(){                      // used to write to the serial line if debugging - 
  cLong++;                                   // counting the number of calls

#ifdef DEBUG                                 // #define DEBUG to print statistics to serial
  Serial.print(now);                         // show the timer variables
  Serial.print("-");
  Serial.print(lastlong);
  Serial.print("=");
  Serial.print(now-lastlong);
  Serial.println();

  Serial.print("Calls to Display: ");        // show the number of calls to the different states
  Serial.print(cDisplay);
  Serial.print(", Buttons: ");
  Serial.print(cButtons);
  Serial.print(", Incoming: ");
  Serial.print(cIncoming);
  Serial.print(", Long: ");
  Serial.print(cLong);

  Serial.print(". Frequency/s: ");          // show the count of calls since last long interval
  Serial.print((cDisplay-cDisplayLast));
  Serial.print(", ");
  Serial.print((cButtons-cButtonsLast));
  Serial.print(", ");
  Serial.print((cIncoming-cIncomingLast));
  Serial.print(", ");
  Serial.println((cLong-cLongLast));

  cDisplayLast = cDisplay;                  // remember the last number
  cButtonsLast = cButtons;
  cIncomingLast = cIncoming;
  cLongLast = cLong;
#endif

};







