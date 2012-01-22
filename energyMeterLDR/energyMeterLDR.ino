int threshold = 250;
int val = 0;
int maxA = 0, minA = 32767;
unsigned long impC = 0;
unsigned long aboveC = 0;
unsigned long belowC = 0;
unsigned long durL = 0, durB = 0, durE = 0;
unsigned long aboveT = 0;
boolean detectedBlink = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Calibrating...");
  while (millis() < 10000) {
    val = analogRead(0);
    if ( val > maxA) maxA = val;
    if ( val < minA) minA = val;
    delay(1);
  };
  
  threshold = (maxA-minA)/2+minA;
  Serial.print("Max ");
  Serial.print(maxA);
  Serial.print(" Min ");
  Serial.print(minA);
  Serial.print(" Threshold ");
  Serial.println(threshold);
}

void loop() {
  val = analogRead(0);

  if (val > threshold) {
    durB = micros();
    aboveC++;
    aboveT += val;
    durL = durB - durE;
  }
  else belowC++;

  if ( val > maxA) maxA = val;
  if ( val < minA) minA = val;

  while (val > threshold) {
    val = analogRead(0);
    durE = micros();
    if (val > threshold) {
      aboveT += val;
      aboveC++; 
    }
    else {
      belowC++;
      impC++;
      detectedBlink = true;
    };

    if ( val > maxA) maxA = val;
    if ( val < minA) minA = val;    

    if ((micros() - durB) > 500000) {    // fail safe - get out of here if it lasts more than 500ms
      Serial.println("Exception!");
      break;
    };
  }

  if (detectedBlink || (millis() % 5000 < 2)) { // Just luck when the % hits
    Serial.print("Duration H: ");
    Serial.print(durE - durB);
    Serial.print(" L: ");
    Serial.print(durL);
    Serial.print(". ");
    Serial.print(aboveC);
    Serial.print(" above, and ");
    Serial.print(belowC);
    Serial.print(" below. ");
    Serial.print(aboveT);
    Serial.print(" above, average ");
    Serial.print(aboveT/aboveC);
    Serial.print(". Min: ");
    Serial.print(minA);
    Serial.print(" Max: ");
    Serial.print(maxA);
    Serial.print(" Imp: ");
    Serial.print(impC);
    Serial.print(" kW/h: ");
    Serial.print((((durE - durB) + durL)*10)/169520);
    Serial.println(".");
    detectedBlink = false;
    minA = 32767; 
    maxA = 0;   // Reset - what's the stats for next blink
    delayMicroseconds(durL / 4); // Sleep a bit and allow the LDR to rest 
  }
}





