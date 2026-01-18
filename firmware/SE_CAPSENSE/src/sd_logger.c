#include "sd_logger.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "sdlog";

/*
 * SD logging stubbed out so firmware can be developed without SD hardware.
 * This keeps the symbols available but makes the module a harmless no-op.
 */

esp_err_t sdlog_init(void){
    ESP_LOGI(TAG, "SD logging is stubbed out for this build");
    return ESP_OK;
}

void sdlog_job_flush(void){
    ESP_LOGD(TAG, "sdlog_job_flush: stub no-op");
}
