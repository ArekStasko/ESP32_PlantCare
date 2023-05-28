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

void sendErrorMessage(const char *error){
  char msg[MSG_SIZE];

  sprintf(msg, "{\"action\":\"msg\",\"body\":{\"type\":\"error\",\"error\":\"%s\"}}", error);

  wsClient.sendTXT(msg);
}

uint8_t toMode(const char *val){
  if(strcmp(val, "output") == 0){
    return OUTPUT;
  }

  if(strcmp(val, "input_pullup") == 0){
    return OUTPUT;
  }

  return INPUT
}

void sendOkMessage(){
  char msg[MSG_SIZE];

  sprintf(msg, "{\"action\":\"msg\",\"body\":{\"type\":\"status\",\"body\":\"ok\"}}");

  wsClient.sendTXT(msg);
}

void handleMessage(uint8_t * payload){
   StaticJsonDocument<JSON_DOC_SIZE> doc;

    DeserializationError error = deserializeJson(doc, payload);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());

    sendErrorMessage(error.c_str());
    return;
  }

  if(doc["type"].is<const char *>()){
    sendErrorMessage("Invalid message type");
    return;
  }

  if(strcmp(doc["type"], "cmd") == 0){
    if(doc["body"].is<JsonObject>()){
      sendErrorMessage("Invalid command body");
      return;
    }
    
    if(strcmp(doc["body"]["type"], "pinMode") == 0){
      pinMode(doc["body"]["pin"], toMode(doc["body"]["mode"]));
      sendOkMessage()
      return;
    }

    if(strcmp(doc["body"]["type"], "digitalWrite") == 0){
      digitalWrite(doc["body"]["pin"], doc["body"]["value"]);
      sendOkMessage()
      return;
    }

    if(strcmp(doc["body"]["type"], "digitalRead") == 0){
      auto value = digitalRead(doc["body"]["pin"]);

      char msg[MSG_SIZE];
      sprintf(msg, "{\"action\":\"msg\",\"body\":{\"type\":\"output\",\"body\":%d}", value);
      wsClient.sendTXT(msg);
      
      return;
    }

    sendErrorMessage("unsupported command");
    return;
  }
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
