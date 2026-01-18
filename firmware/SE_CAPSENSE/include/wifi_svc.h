#pragma once
#include <stdbool.h>
#include <esp_err.h>
#include <stdint.h>

esp_err_t wifi_svc_init(void);
bool wifi_svc_is_connected(void);

// Wait up to timeout_ms for Wi-Fi to connect; returns ESP_OK if connected,
// ESP_ERR_TIMEOUT otherwise.
esp_err_t wifi_svc_wait_connected(uint32_t timeout_ms);

// Job to periodically report Wi‑Fi status (IP, RSSI)
void wifi_svc_job_report(void);

// Persistent Wi‑Fi credential APIs
#define WIFI_MAX_SSID_LEN 32
#define WIFI_MAX_PSK_LEN 64

esp_err_t wifi_svc_set_credentials(const char *ssid, const char *psk);
esp_err_t wifi_svc_clear_credentials(void);
// Attempt to connect using stored credentials (or compile-time defaults as fallback)
esp_err_t wifi_svc_connect(void);
// Read stored credentials (buffers must be provided): returns ESP_OK if found
esp_err_t wifi_svc_read_credentials(char *ssid_out, size_t ssid_sz, char *psk_out, size_t psk_sz);