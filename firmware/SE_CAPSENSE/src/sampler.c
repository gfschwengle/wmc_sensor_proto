#include "sampler.h"
#include "record.h"
#include "fdc1004.h"
#include "bme280_drv.h"
#include "config.h"
#include "timebase.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "board.h"

static const char *TAG = "sampler";

void sampler_init(void){ fdc_init(); fdc_config_default(); }

void sampler_job(void){
    sample_t s = {0};
    s.t_ms = tb_now_ms();
    bool sat=false; float cap[4];

    // Indicate sensor read started
    gpio_set_level(LED_SENSE_GPIO, 1);

    if (fdc_read_pf(cap, &sat)==ESP_OK){
        for (int i=0;i<4;i++) s.cap_pf[i]=cap[i];
        if (sat) s.flags |= (1u<<0);
    }
    esp_err_t r = bme_read(&s.temp_c, &s.hum_pct, &s.pres_hpa);
    if (r != ESP_OK){
        ESP_LOGW(TAG, "BME read failed: %d", r);
        // leave defaults/zeroed values
    } else {
        ESP_LOGI(TAG, "BME: T=%.2fC H=%.2f%% P=%.2f hPa", s.temp_c, s.hum_pct, s.pres_hpa);
    }

    // Sensor read finished
    gpio_set_level(LED_SENSE_GPIO, 0);

    bool pushed = record_push(&s);
    if (!pushed) {
        ESP_LOGW(TAG, "record_push failed: ring full");
    } else {
        ESP_LOGD(TAG, "record pushed, pending=%u", (unsigned)record_count());
    }
} 