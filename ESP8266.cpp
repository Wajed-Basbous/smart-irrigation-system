//ESP 8266

#define BLYNK_TEMPLATE_ID "TMPL6pwwvj11p"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation System"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <RTClib.h>

// WiFi and Blynk setup
const char* ssid = "The GOAT";
const char* pass = "12345678";
char auth[] = "j6Y1o3LviDzac4TPCSouut0FSpPszUlb";

// SoftwareSerial setup
#define RX D2  // GPIO 4
#define TX D1  // GPIO 5
SoftwareSerial Serial2(RX, TX);

//PUMP
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  Serial.println("V1 toggled: " + String(pinValue));
  // Send the relay control command to Arduino via SoftwareSerial
  if (pinValue == 1) {
    Serial2.println("RELAYN");  // Command Arduino to turn relay ON
  } else {
    Serial2.println("RELAYFF");  // Command Arduino to turn relay OFF
  }
}
//PUMP 2
BLYNK_WRITE(V10) {
  int pinValue = param.asInt();
  Serial.println("V10 toggled: " + String(pinValue));
  // Send the relay control command to Arduino via SoftwareSerial
  if (pinValue == 1) {
    Serial2.println("ON");  // Command Arduino to turn relay ON
  } else {
    Serial2.println("OFF");  // Command Arduino to turn relay OFF
  }
}

void setup() {
  Serial.begin(9600);             // Debug serial
  Serial2.begin(9600);            // Communication serial with Arduino
  Blynk.begin(auth, ssid, pass);  // Connect to Blynk
}

void loop() {
  Blynk.run();  // Run Blynk
  if (Serial2.available()) {
    String sensorData = Serial2.readStringUntil('\n');  // Read each line from Arduino

    if (sensorData.indexOf("Relay turned OFF.") >= 0) {
      Blynk.virtualWrite(V1, 0);  
    }
    if (sensorData.indexOf("Relay is gonna be OFF.") >= 0) {
      Blynk.virtualWrite(V10, 0);  
    }

    // Extract and send soil moisture data to Blynk
    if (sensorData.indexOf("Soil Moisture Level 1: ") >= 0) {
      int startMoisture = sensorData.indexOf("Soil Moisture Level 1: ") + 23;
      int endMoisture = sensorData.indexOf(" %", startMoisture);  // Assuming there's a comma after the value
      float soilMoisture = sensorData.substring(startMoisture, endMoisture).toFloat();
      Blynk.virtualWrite(V3, soilMoisture);
    }

    // Extract and send soil moisture data to Blynk
    if (sensorData.indexOf("Soil Moisture Level 2: ") >= 0) {
      int startMoisture2 = sensorData.indexOf("Soil Moisture Level 1: ") + 23;
      int endMoisture2 = sensorData.indexOf(" %", startMoisture2);  // Assuming there's a comma after the value
      float soilMoisture2 = sensorData.substring(startMoisture2, endMoisture2).toFloat();
      Blynk.virtualWrite(V8, soilMoisture2);
    }

    // Extract and send temperature data to Blynk
    if (sensorData.indexOf("Temp: ") >= 0) {
      int tempStart = sensorData.indexOf("Temp: ") + 6;
      int tempEnd = sensorData.indexOf(" *C", tempStart);
      float temperature = sensorData.substring(tempStart, tempEnd).toFloat();
      Blynk.virtualWrite(V7, temperature);
    }

    // Extract and send temperature data to Blynk
    if (sensorData.indexOf("Temp 2: ") >= 0) {
      int tempStart2 = sensorData.indexOf("Temp 2: ") + 8;
      int tempEnd2 = sensorData.indexOf(" *C", tempStart2);
      float temperature2 = sensorData.substring(tempStart2, tempEnd2).toFloat();
      Blynk.virtualWrite(V11, temperature2);
    }

    // Extract and send humidity data to Blynk
    if (sensorData.indexOf("Humidity: ") >= 0) {
      int humStart = sensorData.indexOf("Humidity: ") + 10;
      int humEnd = sensorData.indexOf(" *C", humStart);
      float humidity = sensorData.substring(humStart, humEnd).toFloat();
      Blynk.virtualWrite(V4, humidity);
    }

    if (sensorData.indexOf("Humidity 2: ") >= 0) {
      int humStart2 = sensorData.indexOf("Humidity 2: ") + 12;
      int humEnd2 = sensorData.indexOf(" *C", humStart2);
      float humidity2 = sensorData.substring(humStart2, humEnd2).toFloat();
      Blynk.virtualWrite(V12, humidity2);
    }


    //Light
    if (sensorData.indexOf("Light Status: ") >= 0) {
      int lightValue = sensorData.indexOf("Dark") > -1 ? 0 : 1;
      Blynk.virtualWrite(V2, lightValue);  // Send binary light status to Blynk

      // Log an event in Blynk if it's dark
      if (lightValue == 0) {
        Blynk.logEvent("its_dark", "It's dark!");
      }
    }


    //Water Level
    if (sensorData.indexOf("Water: ") >= 0) {
      int startWaterLevel = sensorData.indexOf("Water: ") + 7;
      int endWaterLevel = sensorData.indexOf(" %", startWaterLevel);  // Assuming there's a comma after the value
      float waterLevel = sensorData.substring(startWaterLevel, endWaterLevel).toFloat();
      Blynk.virtualWrite(V9, waterLevel);
    }
    //Rain
    if (sensorData.indexOf("Rain Status: ") >= 0) {
      int rainValue = sensorData.indexOf("It's raining") > -1 ? 1 : 0;
      Blynk.virtualWrite(V5, rainValue);  // Send binary rain status to Blynk

      // Log an event in Blynk if it's raining
      if (rainValue == 1) {
        Blynk.logEvent("rain_detected", "Rain detected!");
      }
    }

    //RTC
    if (sensorData.indexOf("Date and Time: ") >= 0) {
    int startTime = sensorData.indexOf("Date and Time: ") + 15;
    String time = sensorData.substring(startTime);  // No need to call toString()
    Blynk.virtualWrite(V6, time);
}


    if (sensorData.indexOf("Relay turned ON.") >= 0) {
      Blynk.virtualWrite(V1, 1);  
    }

    if (sensorData.indexOf("Relay is gonna be ON.") >= 0) {
      Blynk.virtualWrite(V10, 1);  
    }
    Serial.println(sensorData);  // Print the sensor data to the Serial Monitor
  }
}
