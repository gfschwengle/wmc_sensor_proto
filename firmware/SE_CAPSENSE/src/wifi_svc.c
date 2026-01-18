#include "wifi_svc.h"
#include "config.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include <string.h>
#include <nvs.h>
#include <nvs_flash.h>

static const char *TAG = "wifi";
static EventGroupHandle_t s_wifi_event_group;
static const int WIFI_CONNECTED_BIT = BIT0;
static esp_netif_t *s_sta_netif = NULL;

// NVS keys
static const char *NVS_NAMESPACE = "wifi";
static const char *NVS_KEY_SSID = "ssid";
static const char *NVS_KEY_PSK = "psk";

// Local cached credentials (loaded from NVS)
#define WIFI_MAX_SSID_LEN 32
#define WIFI_MAX_PSK_LEN 64
static char s_ssid[WIFI_MAX_SSID_LEN] = {0};
static char s_psk[WIFI_MAX_PSK_LEN] = {0};


static void got_ip(void* arg, esp_event_base_t base, int32_t id, void* data){
    ip_event_got_ip_t* ev = (ip_event_got_ip_t*)data;
    char ip_str[16];
    esp_ip4addr_ntoa(&ev->ip_info.ip, ip_str, sizeof(ip_str));
    ESP_LOGI(TAG, "Got IP: %s", ip_str);
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    (void)arg; (void)base; (void)id;
}

static void on_disconnect(void* arg, esp_event_base_t base, int32_t id, void* data){
    ESP_LOGW(TAG, "Wi-Fi disconnected, will attempt reconnect");
    xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    // Try to reconnect
    esp_wifi_connect();
    (void)arg; (void)base; (void)id; (void)data;
}

esp_err_t wifi_svc_init(void){
    esp_err_t r = nvs_flash_init();
    if (r == ESP_ERR_NVS_NO_FREE_PAGES || r == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    } else {
        ESP_ERROR_CHECK(r);
    }

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();
    // store STA netif handle for later status queries
    s_sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (!s_sta_netif) {
        ESP_LOGW(TAG, "Could not get handle for STA netif");
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    s_wifi_event_group = xEventGroupCreate();
    if (!s_wifi_event_group) {
        ESP_LOGE(TAG, "Failed to create Wi-Fi event group");
        return ESP_FAIL;
    }

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, got_ip, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, on_disconnect, NULL, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Try to read stored credentials and connect if present
    if (wifi_svc_read_credentials(s_ssid, sizeof(s_ssid), s_psk, sizeof(s_psk)) == ESP_OK){
        ESP_LOGI(TAG, "Found stored Wi‑Fi credentials for SSID '%s' — attempting connect", s_ssid);
        wifi_svc_connect();
    } else {
        ESP_LOGI(TAG, "No stored Wi‑Fi credentials found; use 'wifi set <ssid> <psk>' via UART to configure");
    }

    return ESP_OK;
}

bool wifi_svc_is_connected(void){
    return (xEventGroupGetBits(s_wifi_event_group) & WIFI_CONNECTED_BIT) != 0;
}

esp_err_t wifi_svc_wait_connected(uint32_t timeout_ms){
    if (!s_wifi_event_group) return ESP_ERR_INVALID_STATE;
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdFALSE, pdMS_TO_TICKS(timeout_ms));
    if (bits & WIFI_CONNECTED_BIT) return ESP_OK;
    return ESP_ERR_TIMEOUT;
}

void wifi_svc_job_report(void){
    if (!s_wifi_event_group){
        ESP_LOGW(TAG, "wifi_svc_job_report: service not initialized");
        return;
    }

    if (wifi_svc_is_connected()){
        esp_netif_ip_info_t ip_info;
        if (s_sta_netif && esp_netif_get_ip_info(s_sta_netif, &ip_info) == ESP_OK){
            char ip_str[16];
            esp_ip4addr_ntoa(&ip_info.ip, ip_str, sizeof(ip_str));
            // get RSSI
            wifi_ap_record_t ap_info;
            int rssi = 0;
            if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) rssi = ap_info.rssi;
            ESP_LOGI(TAG, "Wi-Fi status: IP=%s RSSI=%d dBm", ip_str, rssi);
        } else {
            ESP_LOGI(TAG, "Wi-Fi status: connected, but no IP info available");
        }
    } else {
        ESP_LOGI(TAG, "Wi-Fi status: not connected");
    }
}

// Read stored credentials from NVS. Returns ESP_OK if both keys exist
esp_err_t wifi_svc_read_credentials(char *ssid_out, size_t ssid_sz, char *psk_out, size_t psk_sz){
    nvs_handle_t h;
    esp_err_t r = nvs_open(NVS_NAMESPACE, NVS_READONLY, &h);
    if (r != ESP_OK) return r;

    size_t required = ssid_sz;
    r = nvs_get_str(h, NVS_KEY_SSID, ssid_out, &required);
    if (r != ESP_OK){ nvs_close(h); return r; }

    required = psk_sz;
    r = nvs_get_str(h, NVS_KEY_PSK, psk_out, &required);
    nvs_close(h);
    return r;
}

// Store credentials in NVS and update local cache. Returns ESP_OK on success
esp_err_t wifi_svc_set_credentials(const char *ssid, const char *psk){
    if (!ssid || !psk) return ESP_ERR_INVALID_ARG;
    if (strlen(ssid) >= WIFI_MAX_SSID_LEN || strlen(psk) >= WIFI_MAX_PSK_LEN) return ESP_ERR_INVALID_SIZE;

    nvs_handle_t h;
    esp_err_t r = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &h);
    if (r != ESP_OK) return r;
    r = nvs_set_str(h, NVS_KEY_SSID, ssid);
    if (r == ESP_OK) r = nvs_set_str(h, NVS_KEY_PSK, psk);
    if (r == ESP_OK) r = nvs_commit(h);
    nvs_close(h);
    if (r == ESP_OK){
        strncpy(s_ssid, ssid, WIFI_MAX_SSID_LEN);
        strncpy(s_psk, psk, WIFI_MAX_PSK_LEN);
        ESP_LOGI(TAG, "Stored Wi‑Fi credentials for SSID '%s'", s_ssid);
        // attempt connect immediately
        return wifi_svc_connect();
    }
    return r;
}

esp_err_t wifi_svc_clear_credentials(void){
    nvs_handle_t h;
    esp_err_t r = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &h);
    if (r != ESP_OK) return r;
    nvs_erase_key(h, NVS_KEY_SSID);
    nvs_erase_key(h, NVS_KEY_PSK);
    r = nvs_commit(h);
    nvs_close(h);
    if (r == ESP_OK){
        memset(s_ssid, 0, sizeof(s_ssid));
        memset(s_psk, 0, sizeof(s_psk));
        ESP_LOGI(TAG, "Cleared stored Wi‑Fi credentials");
    }
    return r;
}

// Use stored credentials (or fallback to compile-time ones) to set Wi-Fi config and connect
esp_err_t wifi_svc_connect(void){
    char ssid[WIFI_MAX_SSID_LEN] = {0};
    char psk[WIFI_MAX_PSK_LEN] = {0};

    esp_err_t r = wifi_svc_read_credentials(ssid, sizeof(ssid), psk, sizeof(psk));
    if (r != ESP_OK){
        // fallback to compile-time default if defined
#ifdef WIFI_SSID
        strncpy(ssid, WIFI_SSID, sizeof(ssid)-1);
#else
        ESP_LOGW(TAG, "No stored SSID and no compile-time WIFI_SSID available");
        return ESP_ERR_NOT_FOUND;
#endif
#ifdef WIFI_PSK
        strncpy(psk, WIFI_PSK, sizeof(psk)-1);
#endif
    }

    wifi_config_t wc;
    memset(&wc, 0, sizeof(wc));
    snprintf((char*)wc.sta.ssid, sizeof(wc.sta.ssid), "%s", ssid);
    snprintf((char*)wc.sta.password, sizeof(wc.sta.password), "%s", psk);

    ESP_LOGI(TAG, "Connecting to SSID '%s'...", ssid);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wc));
    r = esp_wifi_connect();
    return r;
}