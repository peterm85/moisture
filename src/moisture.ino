#include <ESP8266WiFi.h>
#include <time.h>

const char* ssid     = "SSID";
const char* password = "PASSWORD";

//Fixed IP config
IPAddress local_IP(192, 168, 1, 152);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Time zone: ESP (UTC+1 / +2 on summer)
const char* ntpServer = "162.159.200.1"; //"pool.ntp.org";
const long  gmtOffset_sec = 3600;        // +1 hour
const int   daylightOffset_sec = 3600;   // +1h summer timetable

WiFiServer server(80);

#define SENSOR_POWER_PIN D1
#define SENSOR_SIGNAL_PIN A0

void setup() {
  Serial.begin(9600); // Init serial communication
  //Serial.setDebugOutput(true); // DEBUG

  pinMode(SENSOR_POWER_PIN, OUTPUT);
  digitalWrite(SENSOR_POWER_PIN, LOW); // Keep turn-off since the beginning

  connectToWiFi();
  syncClock();

  server.begin();
  Serial.println("\n✅ API server started");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available()) delay(1);

  String request = client.readStringUntil('\r');
  client.flush();

  handleRequest(client, request);
}


void connectToWiFi() {

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("❌ Failed static IP configuration");
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n✅ Connected to WiFi | ");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void syncClock(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  struct tm timeinfo;
  int retry = 0;
  const int maxRetries = 10;

  while (!getLocalTime(&timeinfo, 10000) && retry < maxRetries) {
    Serial.println("⌛ Waiting to synchronizing with NTP...");
    retry++;
    delay(1000);
  }

  if (retry == maxRetries) {
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

void handleRequest(WiFiClient& client, const String& request) {

  printLocalTime();
  Serial.print(" | Request: ");
  Serial.print(request);

  if (request.indexOf("GET /moisture") != -1) {
    int moisture = readSensor();
    printLog(moisture);
    sendResponse(client, moisture);
  } else {
    send404(client);
  }
}

int readSensor() {
  digitalWrite(SENSOR_POWER_PIN, HIGH);
  delay(500);
  int moisture = analogRead(SENSOR_SIGNAL_PIN);
  digitalWrite(SENSOR_POWER_PIN, LOW);
  return moisture;
}

void sendResponse(WiFiClient& client, int moisture) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  client.print("{\"moisture\": ");
  client.print(moisture);
  client.print("}");
}

void send404(WiFiClient& client) {
  client.println("HTTP/1.1 404 Not Found");
  client.println("Connection: close");
  client.println();
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
