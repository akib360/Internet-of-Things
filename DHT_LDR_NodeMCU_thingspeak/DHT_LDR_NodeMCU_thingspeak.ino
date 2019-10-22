/*
Author: Akib Hosen Khan
        B.Sc. in Information & Communication Technology.
        Islamic University, Bangladesh
Hardware: DHT, LDR and NodeMCU
Data is displayed at thingspeak
*/

#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>

#define LDR_PIN A0
#define DHTPIN D5
#define DHTTYPE  DHT11

const char* ssid        = "your wifi ssid";
const char* password    = "your password";
const char* host        = "api.thingspeak.com";
const char* writeAPIKey = "your api";

DHT dht(DHTPIN, DHTTYPE, 15);

int ldr_value = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  ldr_value = analogRead(LDR_PIN); // read input value and store it
  

  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

// make TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }

  String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(temperature);
  url+="&field2=";
  url+=String(humidity);
  url+="\r\n";
  url+="&field3=";
  url+=String(ldr_value);
  url+="\r\n";

  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

    
  Serial.print("Temperature Value is :");  
  Serial.print(temperature);  
  Serial.println("C");  
  Serial.print("Humidity Value is :");  
  Serial.print(humidity);  
  Serial.println("%");
  Serial.print("LDR value is :");
  Serial.println(ldr_value);  
    
    
    
  delay(5000);
}
