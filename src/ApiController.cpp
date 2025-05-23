#include "ApiController.h"

ApiController::ApiController(ESP8266WebServer &server)
  : server(server), cache(10000) {
}

void ApiController::init(){
  cache.setReader(readSensor);

  server.on("/", [this]() { handleRoot(); });
  server.on("/status", [this]() { handleApiStatus(); });
  server.on("/moisture", [this]() { handleMoisture(); });
  server.onNotFound([this]() { handleNotFound(); });

  server.begin();
  Serial.println("\n✅ API server started");
}

void ApiController::handleRoot() {
  printLocalTime();
  Serial.println(" | Request: /");

  server.send(200, "text/plain", "Hi from ESP8266");
}

void ApiController::handleApiStatus() {
  printLocalTime();
  Serial.println(" | Request: /status");

  server.send(200, "application/json", "{\"status\":\"UP\"}");
}

void ApiController::handleMoisture() {
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

void ApiController::handleNotFound() {
  printLocalTime();
  Serial.print(" | Unknown request: ");
  Serial.println(server.uri());

  server.send(404, "text/plain", "404: Not Found");
}

void ApiController::printLog(int moisture){
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