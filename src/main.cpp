#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

#define WIFI_SSID "Orange_Swiatlowod_D5A0"
#define WIFI_PASSWORD "FYPWSCW59ZAY"
#define WS_HOST ""
#define WS_PORT 443
#define WS_URL ""

WebSocketsClient wsClient;

void handleMessage(uint8_t * payload){
  
}

void onWSEvent(WStype_t type, uint8_t * payload, size_t length){
  switch(type){
    case WStype_CONNECTED: {
      Serial.println("WS Connected");
      break;
    }
    case WStype_DISCONNECTED : {
      Serial.println("WS Disconnected");
    }
    case WStype_TEXT: {
      Serial.printf("WS Message: %s\n", payload);
      handleMessage(payload);
      break;
    }
  }
}

void setup() {
  Serial.begin(921600);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connected");
  wsClient.beginSSL(WS_HOST, WS_PORT, WS_URL, "", "wss");
  wsClient.onEvent(onWSEvent);
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
