#include <ESP8266WebServer.h>
#include "NetworkManager.h"
#include "TimeManager.h"
#include "ApiController.h"

ESP8266WebServer server(80);
ApiController apiController(server);

void setup() {
  Serial.begin(9600); // Init serial communication
  //Serial.setDebugOutput(true); // DEBUG

  pinMode(SENSOR_POWER_PIN, OUTPUT);
  digitalWrite(SENSOR_POWER_PIN, LOW); // Keep turn-off since the beginning

  connectToWiFi();
  syncClock();

  apiController.init();
}

void loop() {
  server.handleClient();
}