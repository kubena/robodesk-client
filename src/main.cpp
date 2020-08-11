#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

ESP8266WiFiMulti wifiMulti;
const char* ssid     = "ESP8266-Access-Point";
const char* password = "123456789";
String mainURL ="http://192.168.4.1/";

void setup() {
  pinMode(2, INPUT);
  Serial.begin(9600);
  Serial.println('\n');

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(ssid, password);
  Serial.println("Connecting ...");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  Serial.println('\n');
}

// the loop function runs over and over again forever
void loop() {


  // detect pin2 status
  Serial.println("Looping ...");
  WiFiClient client;
  HTTPClient http;
  bool req = false;
  int pin2 = digitalRead(2);

  if (pin2 == 1){
    Serial.println("OFF");
    req = http.begin(client, mainURL + "move?direction=stop");
  }
  else
  {
    Serial.println("ON");
    req = http.begin(client, mainURL + "move?direction=up"); //"move?direction=up");
  }
  if (req) {  // HTTP
    Serial.print("[HTTP] GET...");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d", httpCode);
      // file found at server
      if (httpCode == 200 || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s", http.errorToString(httpCode).c_str());
    }

      http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect");
  }

  Serial.println('\n');
  
  delay(1000);
}