#include <Arduino.h>
#include <WiFi.h>

#define WIFI_SSID "Orange_Swiatlowod_D5A0"
#define WIFI_PASSWORD "FYPWSCW59ZAY"

void setup() {
  Serial.begin(921600);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connected");
}

bool isConnected = false;

void loop() {

  if(WiFi.status() == WL_CONNECTED && isConnected){
    Serial.println("connected");
    digitalWrite(BUILTIN_LED, HIGH);
    isConnected = true;
  }

  if(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    digitalWrite(BUILTIN_LED, !digitalRead(LED_BUILTIN));
    delay(1000);
    isConnected = false;
  }

}
