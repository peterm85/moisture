#include <ESP8266WebServer.h>

#include "NetworkManager.h"
#include "TimeManager.h"
#include "SensorReader.h"
#include "CacheManager.h"

ESP8266WebServer server(80);
CacheManager cache(10000);

void setup() {
  Serial.begin(9600); // Init serial communication
  //Serial.setDebugOutput(true); // DEBUG

  pinMode(SENSOR_POWER_PIN, OUTPUT);
  digitalWrite(SENSOR_POWER_PIN, LOW); // Keep turn-off since the beginning

  connectToWiFi();
  syncClock();

  server.on("/", handleRoot);
  server.on("/status", handleApiStatus);
  server.on("/moisture", handleMoisture);
  server.onNotFound(handleNotFound);

  server.begin();
  cache.setReader(readSensor);
  Serial.println("\n✅ API server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "Hola desde ESP8266");
}

void handleApiStatus() {
  server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleMoisture() {
  printLocalTime();

  bool force = false;
  if (server.hasArg("force")) {
    String val = server.arg("force");
    val.toLowerCase();
    force = (val == "true");
    Serial.print(" | Request: /moisture?force=" + String(val));
  }else{
    Serial.print(" | Request: /moisture");
  }

  int moisture = cache.get("MOISTURE", force);
  printLog(moisture);

  String response = "{\"moisture\": " + String(moisture) + "}";
  server.send(200, "application/json", response);
}

void handleNotFound() {
  printLocalTime();
  Serial.print(" | Unknown request: ");
  Serial.println(server.uri());

  server.send(404, "text/plain", "404: Not Found");
}

void printLog(int moisture){
  Serial.print(" | Moisture value: ");
  Serial.print(moisture);
  Serial.print(" - ");

  if (moisture < 50) {
    Serial.println("Very dry 🌵");
  } else if (moisture < 300) {
    Serial.println("Dry 🚿");
  } else if (moisture < 400) {
    Serial.println("Wet 💧");
  } else {
    Serial.println("Very humid 🌊");
  }
}
