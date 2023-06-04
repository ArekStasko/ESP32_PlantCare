#include <ArduinoJson.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

#define WIFI_SSID "Orange_Swiatlowod_D5A0"
#define WIFI_PASSWORD "FYPWSCW59ZAY"
#define WS_HOST ""
#define WS_PORT 443
#define WS_URL ""

#define JSON_DOC_SIZE 2048
#define MSG_SIZE 128

WebSocketsClient wsClient;

void setup() {
  Serial.begin(921600);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connected");
  wsClient.beginSSL(WS_HOST, WS_PORT, WS_URL, "", "wss");
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

  wsClient.loop();
}
