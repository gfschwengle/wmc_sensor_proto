#pragma once
#include <stdint.h>

#define SAMPLE_PERIOD_MS 1000
#define SAMPLE_AVG_COUNT 4
#define SD_FLUSH_PERIOD_MS 5000
#define RECORD_RING_CAP 64
#define FDC_RATE_HZ 100
#define FDC_CAP_RANGE_PF 15.0f

#define WIFI_SSID "YOUR_SSID"
#define WIFI_PSK "YOUR_PASS"
#define MQTT_BROKER_URI "mqtt://raspberrypi.local"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "esp32c3-capboard-01"
#define MQTT_BASE_TOPIC "capboard/esp32c3-01"
#define MQTT_QOS 1
#define MQTT_PUB_PERIOD_MS 200
#define MQTT_QUEUE_DEPTH 128