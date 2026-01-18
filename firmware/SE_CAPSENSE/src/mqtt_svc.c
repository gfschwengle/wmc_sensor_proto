#include "mqtt_svc.h"
#include "config.h"
#include "esp_log.h"


static const char *TAG = "mqtt";


esp_err_t mqtt_svc_init(void){
ESP_LOGI(TAG, "MQTT init (stub) broker=%s", MQTT_BROKER_URI);
return ESP_OK;
}


bool mqtt_svc_is_connected(void){ return false; }


bool mqtt_svc_enqueue(const sample_t *s){ (void)s; return true; }


void mqtt_svc_job_drain(void){ /* TODO */ }