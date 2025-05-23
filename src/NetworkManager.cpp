#include "NetworkManager.h"
#include "config.h"

void connectToWiFi() {

  IPAddress local_IP(LOCAL_IP);
  IPAddress gateway(GATEWAY);
  IPAddress subnet(SUBNET);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("❌ Failed static IP configuration");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\n✅ Connected to WiFi | IP: ");
  Serial.println(WiFi.localIP());
}