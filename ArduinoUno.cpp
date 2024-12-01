//Arduino Uno

#include <SimpleDHT.h>
#include <DS3231.h>

// Define the Analog pin numbers on the Arduino for the sensors
int Moisture_signal1 = A0;    // Soil moisture sensor signal 1
int Moisture_signal2 = A1;    // Soil moisture sensor signal 2
int Rain_signal = 8;          // Rain sensor signal, digital pin
const int ldr_pin = 7;        // LDR sensor signal, digital pin
const int Lvl = A3;           // Water level sensor signal
const int relayPin = 5;       // Pin for relay control
const int relayPin2 = 10;       // Pin for relay control
bool relayState1 = false;      // Current state of the relay
bool lastRelayState1 = false;  // Last state of the relay to detect changes
bool relayState2 = false;      // Current state of the relay
bool lastRelayState2 = false;  // Last state of the relay to detect changes
bool manualOverride1 = false;  // Flag to check if relay is manually controlled
bool manualOverride2 = false;
int pinDHT11 = 2;             // DHT11 setup
int pinDHT11_2= 3;             // DHT11 setup

SimpleDHT11 dht11(pinDHT11);
SimpleDHT11 dht11_2(pinDHT11_2);
DS3231 rtc(SDA, SCL);  // RTC setup

void setup() {
  pinMode(ldr_pin, INPUT);
  pinMode(Rain_signal, INPUT);
  
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Initially turn the relay OFF (High is OFF here)
  digitalWrite(relayPin2, HIGH);  // Initially turn the relay OFF (High is OFF here)


  Serial.begin(9600);  // Debugging via USB Serial
  rtc.begin();
  //rtc.setDOW(MONDAY);     // Set Day-of-Week
  //rtc.setTime(4, 41, 9); // Set time to 15:48:00 (3:48 PM) format is (hour, minute, second)
  //rtc.setDate(29, 4, 2024); // Set date to March 20, 2024 format is (day, month, year)
}

void loop() {
  int Moisture1 = analogRead(Moisture_signal1);
  Moisture1 = map(Moisture1, 1024, 200, 0, 100);  // Map the reading to a percentage
  Serial.print("Soil Moisture Level 1: ");
  Serial.print(Moisture1);
  Serial.println("%");

  int Moisture2 = analogRead(Moisture_signal2);
  Moisture2 = map(Moisture2, 1024, 200, 0, 100);  // Map the reading to a percentage
  Serial.print("Soil Moisture Level 2: ");
  Serial.print(Moisture2);
  Serial.println("%");

  bool isRaining = digitalRead(Rain_signal) == LOW;
  Serial.print("Rain Status: ");
  Serial.println(isRaining ? "It's raining" : "It's not raining");

  bool isDark = digitalRead(ldr_pin) == HIGH;
  Serial.print("Light Status: ");
  Serial.println(isDark ? "Dark" : "Light");
//dht11 1
  byte temperature, humidity;
  if (dht11.read(&temperature, &humidity, NULL) == SimpleDHTErrSuccess) {
    Serial.print("Temp: ");
    Serial.print((int)temperature);
    Serial.println(" *C");
      Serial.println("");

    delay(500);
    Serial.print("Humidity: ");
    Serial.print((int)humidity);
    Serial.println("%");
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }
  delay(500);

  //dht11 2
byte temperature2, humidity2;
  if (dht11_2.read(&temperature2, &humidity2, NULL) == SimpleDHTErrSuccess) {
    Serial.print("Temp 2: ");
    Serial.print((int)temperature2);
    Serial.println(" *C");
    Serial.println("");
    Serial.print("Humidity 2: ");
    Serial.print((int)humidity2);
    Serial.println("%");
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }


  //Water Level
  int waterLevel = analogRead(Lvl);
  waterLevel = map(waterLevel, 0, 370, 0, 100);
  Serial.print("Water: ");
  Serial.print(waterLevel);
  Serial.println("%");
  delay(500);

  //RTC
  Serial.println("");
  Serial.print("Date and Time: ");
  String dateTime = String(rtc.getDOWStr()) + "  " + String(rtc.getDateStr()) + " -- " + String(rtc.getTimeStr());
  Serial.println(dateTime);

  if (!manualOverride1) {
    if ((Moisture1 < 40  && !isRaining && !isDark && waterLevel >= 50 && temperature >= 1 && temperature <= 30) && !relayState1) {
      relayState1 = true;
      digitalWrite(relayPin, LOW);  // Turn the relay ON
      if (relayState1 != lastRelayState1) {
        Serial.println("Relay turned ON.");
        lastRelayState1 = relayState1;
      }
    } else if ((Moisture1 > 70  || isRaining || isDark || waterLevel < 50 || temperature <= 0 || temperature > 30) && relayState1) {
      relayState1 = false;
      digitalWrite(relayPin, HIGH);  // Turn the relay OFF
      if (relayState1 != lastRelayState1) {
        Serial.println("Relay turned OFF.");
        lastRelayState1 = relayState1;
      }
    }
  }
  if (!manualOverride2) {
    if ((Moisture2 < 40  && !isRaining && !isDark && waterLevel >= 50 && temperature2 >= 1 && temperature2 <= 30) && !relayState2) {
      relayState2 = true;
      digitalWrite(relayPin2, LOW);  // Turn the relay ON
      if (relayState2 != lastRelayState2) {
        Serial.println("Relay is gonna be ON.");
        lastRelayState2 = relayState2;
      }
    } else if ((Moisture2 > 70  || isRaining || isDark || waterLevel < 50 || temperature2 <= 0 || temperature2 > 30) && relayState2) {
      relayState2 = false;
      digitalWrite(relayPin2, HIGH);  // Turn the relay OFF
      if (relayState2 != lastRelayState2) {
        Serial.println("Relay is gonna be OFF.");
        lastRelayState2 = relayState2;
      }
    }
  }

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Read the incoming command
    if (command.indexOf("RELAYN") >= 0) {
      digitalWrite(relayPin, LOW);  // Turn relay ON
      relayState1 = true;
      manualOverride1 = true;                          // Assume any command is a manual override

    } else if (command.indexOf("RELAYFF") >= 0) {
      digitalWrite(relayPin, HIGH);  // Turn relay OFF
      relayState1 = false;
      manualOverride1 = false;
    }
    
    if (command.indexOf("ON") >= 0) {
      digitalWrite(relayPin2, LOW);  // Turn relay ON
      relayState2 = true;
      manualOverride2 = true;                          // Assume any command is a manual override

    } else if (command.indexOf("OFF") >= 0) {
      digitalWrite(relayPin2, HIGH);  // Turn relay OFF
      relayState2 = false;
      manualOverride2 = false;
    }
    lastRelayState1 = relayState1;  // Update lastRelayState to match the manually set state
    lastRelayState2 = relayState2;  // Update lastRelayState to match the manually set state

  }

  delay(500);  // Short delay for stability
}