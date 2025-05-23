#include "SensorReader.h"
#include <Arduino.h>

int readSensor(const String& key) {
  if (key == "MOISTURE") {
    digitalWrite(SENSOR_POWER_PIN, HIGH);
    delay(500);
    int moisture = analogRead(SENSOR_SIGNAL_PIN);
    digitalWrite(SENSOR_POWER_PIN, LOW);
    return moisture;
  }
  return -1;
}