/*

Thermostat potentiometer - control out pin with a potentiometer.

 __________                       ` __________
|          |                      `|          |
|          |______________________`|          |_____  ...
                                  `
<---------><--------------------->`
onDuration
<-------------------------------->
       globalPeriod

See https://www.arduino.cc/en/Tutorial/AnalogReadSerial for wiring
*/

/*
TODO:
  - bouton boost :
    - 1 appui => led clignote 1 fois (1/2s on 1,5 off) => on pendant 30s
    - 2 appuis => led clignote 2 fois => on pendant 60s

*/

#define arduino  // Comment for Attiny

#ifdef arduino
  // Arduino
  #define dbg  // Serial out
  byte ledPin = 13;  // LED_BUILTIN
  byte outPin = 12;
  byte potPin = A0;
  unsigned int globalPeriod = 10000;  // Max 65535
#else
  // Attiny 85
  byte ledPin = 0;  // high when out low
  byte outPin = 1;
  byte potPin = 2;
  unsigned int globalPeriod = 20000;  // Max 65535
#endif

unsigned int sleepTime = 500;     // Sleep duration between actions
unsigned int minPotValue = 140;   // if potValue < minPotValue => off
unsigned int maxPotValue = 1016;  // if potValue > maxPotValue => on
bool on;
bool invert_pot;

unsigned long currentPeriodBegin = 0;
unsigned int potValue;
unsigned int onDuration;
unsigned int lastPotValue = 0;


void setup() {
  pinMode(potPin, INPUT);
  pinMode(outPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  on = true;
  invert_pot = true;
  digitalWrite(outPin, HIGH);
  digitalWrite(ledPin, HIGH);
  #ifdef dbg
  Serial.begin(1200);
  #endif
}

void debug(String message) {
  #ifdef dbg
    Serial.println(message);
  #endif
}

void blink_led(byte nb_times, byte d=200) {
  digitalWrite(ledPin, LOW);
  delay(d);
  for(byte i = 0; i < nb_times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(d);
    digitalWrite(ledPin, LOW);
    delay(d);
  }
}

void loop() {
  // pot read
  potValue = analogRead(potPin);
  if (invert_pot) potValue = 1024 - potValue;
  // debug(String(on));
  if (abs((int) potValue - (int) lastPotValue) > 7) {
    Serial.println("\npot: " + String(lastPotValue) + " -> " + String(potValue));
    lastPotValue = potValue;
    if (potValue < minPotValue) {
      debug("< min => OFF");
      onDuration = 0;
      blink_led(3);
      digitalWrite(ledPin, LOW);
    } else if (potValue >= maxPotValue) {
      debug(">= max => ON");
      onDuration = globalPeriod;
      blink_led(6);
      digitalWrite(ledPin, HIGH);
    } else {
      // onDuration = (unsigned int)(potValue * potToTimeRatio);
      onDuration = map(potValue, minPotValue, maxPotValue, 0, globalPeriod);
      #ifdef dbg
        Serial.println("onDuration: " + String(onDuration) + " = " + String(onDuration*(100.0/(float)globalPeriod)) + '%');
      #endif
    }
  }
  // time to switch on ?
  if (!on && onDuration > 100 && (millis() - currentPeriodBegin > globalPeriod)) {
    // Switch on
    currentPeriodBegin = millis();
    digitalWrite(outPin, HIGH);
    digitalWrite(ledPin, HIGH);
    on = true;
  }
  // time to switch off ?
  if (on && onDuration < globalPeriod && (millis() - currentPeriodBegin > onDuration)) {
    // Serial.print("millis() - currentPeriodBegin: "); Serial.println(millis() - currentPeriodBegin);
    // Switch off
    digitalWrite(outPin, LOW);
    digitalWrite(ledPin, LOW);
    on = false;
  }
  delay(sleepTime);
}
