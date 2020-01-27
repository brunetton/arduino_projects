/*
 * DHT thermostat - inspired by MakerduDimanche project available here: https://gist.github.com/MakerduDimanche/cb51ab456589a577b802cc33670b567a
 * Licence CC-by-nc-sa.
 */

#include <DHT.h>;

int TARGET_TEMP = 30;
int TEMP_MARGIN = 1;   // will try to let temp in [TARGET_TEMP - TEMP_MARGIN, TARGET_TEMP + TEMP_MARGIN]

// Pins definitions
#define DHT_PIN 11            // DTH sensor
#define RELAIS_PIN 8          // Relay
// Led pin
#define LED_PIN LED_BUILTIN

// DTH init
#define DHT_TYPE DHT22        // DHT 22. See
DHT dht(DHT_PIN, DHT_TYPE);   // Initialize DHT sensor for normal 16mhz Arduino

char max_temp;
char min_temp;

void setup()
{
   // Init serial logger
   Serial.begin(9600);
   Serial.println("Init");
   dht.begin();
   pinMode(LED_PIN, OUTPUT);
   pinMode(RELAIS_PIN, OUTPUT);
   // Open relay to stop heating by default
   digitalWrite(RELAIS_PIN, HIGH);

   // Compute min and max temperatures
   max_temp = TARGET_TEMP + TEMP_MARGIN;
   min_temp = TARGET_TEMP - TEMP_MARGIN;

   // Blink led to visually spot it
   for (int i = 0; i < 8; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
   }
}

void loop()
{
   // Read temp
   float t = dht.readTemperature();

   // If temp read failed, try again
   if (isnan(t)) {
      Serial.println("Erreur DHT");
      delay(500);
      return;
   }

   Serial.print("Température: ");
   Serial.print(t);
   Serial.print("°C ");

   // See if it's time to heat, or stop heating
   if (dht.readTemperature() < min_temp) {
      // Time to heat
      Serial.println(" --> START");
      // Close relay to start heating
      digitalWrite(RELAIS_PIN, LOW);
      // Switch on led
      digitalWrite(LED_PIN, HIGH);
   } else if (dht.readTemperature() > max_temp) {
      // Time to stop heating
      Serial.println(" --> STOP");
      // Open relay to stop heating
      digitalWrite(RELAIS_PIN, HIGH);
      // Switch off led
      digitalWrite(LED_PIN, LOW);
   } else {
      Serial.println("");
   }

   delay(1000);
}
