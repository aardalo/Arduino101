/*
** Rotary Encoder Example
** Use the Sparkfun Rotary Encoder to vary brightness of LED
**
** Sample the encoder at 200Hz using the millis() function
*/

int brightness = 0;    // how bright the LED is, start at half brightness
int fadeAmount = 5;    // how many points to fade the LED by
unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 4;  // pin 4
const int pin_B = 5;  // pin 5
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;

void setup()  {
  pinMode(11, OUTPUT);    // declare pin 11 to be an output:
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  currentTime = millis();
  loopTime = currentTime; 
} 

void loop()  {
  currentTime = millis();                 // get the current elapsed time
  if(currentTime >= (loopTime + 5)){      // 5ms since last check of encoder = 200Hz  

    encoder_A = digitalRead(pin_A);         // Read encoder pins
    encoder_B = digitalRead(pin_B);   
    if((!encoder_A) && (encoder_A_prev)){   // A has gone from high to low 
      if(encoder_B) {                         // B is high so clockwise
        // increase the brightness, dont go over 255
        if(brightness + fadeAmount <= 255) brightness += fadeAmount;               
      }   
      else {                                  // B is low so counter-clockwise      
        // decrease the brightness, dont go below 0
        if(brightness - fadeAmount >= 0) brightness -= fadeAmount;               
      }   
    }   
    encoder_A_prev = encoder_A;             // Store value of A for next time    
    analogWrite(11, brightness);             // set the brightness of pin 11:
    loopTime = currentTime;                 // Updates loopTime
  }
}
