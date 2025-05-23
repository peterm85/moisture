#pragma once

// WiFi Credentials
#define WIFI_SSID "SSID_2.4GHz"
#define WIFI_PASSWORD "PASSWORD"

// Static IP Configuration
#define LOCAL_IP 192, 168, 1, 152
#define GATEWAY  192, 168, 1, 1
#define SUBNET   255, 255, 255, 0

// Time zone: ESP (UTC+1 / +2 on summer)
#define NTP_SERVER "162.159.200.1" // "pool.ntp.org";
#define GMT_OFFSET_SEC 3600        // +1 hour
#define DAYLIGHT_OFFSET_SEC 3600   // +1h summer timetable
#define MAX_RETRIES 10