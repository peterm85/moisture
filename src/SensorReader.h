#ifndef SENSOR_READER_H
#define SENSOR_READER_H

#define SENSOR_POWER_PIN D1
#define SENSOR_SIGNAL_PIN A0

#include <Arduino.h>

int readSensor(const String& key);

#endif