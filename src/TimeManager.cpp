#include "TimeManager.h"
#include "config.h"

void syncClock() {

  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);

  struct tm timeinfo;
  int retry = 0;

  while (!getLocalTime(&timeinfo, 10000) && retry < MAX_RETRIES) {
    Serial.println("⌛ Waiting to synchronizing with NTP...");
    retry++;
    delay(1000);
  }

  if (retry == MAX_RETRIES) {
    Serial.println("❌ Cannot synch time");
  } else {
    Serial.print("✅ Time sucessfully synchronized | ");
    printLocalTime();
  }
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("❌ Cannot get the time");
    return;
  }
  char timeStr[64];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  Serial.print(timeStr);
}