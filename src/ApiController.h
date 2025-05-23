#ifndef API_CONTROLLER_H
#define API_CONTROLLER_H

#include <ESP8266WebServer.h>
#include "SensorReader.h"
#include "CacheManager.h"
#include "TimeManager.h"

class ApiController {
public:
  ApiController(ESP8266WebServer &server);
  void init();

private:
  ESP8266WebServer &server;
  CacheManager cache;
  void handleRoot();
  void handleApiStatus();
  void handleMoisture();
  void handleNotFound();
  void printLog(int moisture);
};

#endif