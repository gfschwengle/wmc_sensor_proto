#include "fdc1004.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include "config.h"
#include <stdint.h>
#include <stdbool.h>

#define FDC_REG_MANUF_ID 0xFE
#define FDC_REG_DEVICE_ID 0xFF
#define FDC_REG_RESULT_BASE 0x00 // results: 4 * 2 bytes

static const char *TAG = "fdc";

esp_err_t fdc_init(void){
    uint8_t buf[2];
    if (i2c_rd(FDC1004_I2C_ADDR, FDC_REG_MANUF_ID, buf, 2)!=ESP_OK) {
        ESP_LOGE(TAG, "Failed to read manufacturer ID");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "manuf_id=0x%02X%02X", buf[0], buf[1]);

    if (i2c_rd(FDC1004_I2C_ADDR, FDC_REG_DEVICE_ID, buf, 2)!=ESP_OK) {
        ESP_LOGE(TAG, "Failed to read device ID");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "device_id=0x%02X%02X", buf[0], buf[1]);

    return ESP_OK;
}

esp_err_t fdc_config_default(void){
    // NOTE: real configuration (MEASx mapping, conversion rate, range) is TODO.
    // For now, we assume the sensor is in a mode that produces results at 0x00..0x07
    // and will be measured externally or by default settings. If you want, I can
    // implement full register config once we confirm register map/sheet.
    ESP_LOGI(TAG, "fdc_config_default: no-op (TODO: implement full config)");
    return ESP_OK;
}

esp_err_t fdc_read_pf(float out_pf[4], bool *saturated){
    uint8_t buf[8];
    if (i2c_rd(FDC1004_I2C_ADDR, FDC_REG_RESULT_BASE, buf, sizeof(buf)) != ESP_OK){
        ESP_LOGW(TAG, "fdc_read: failed to read result registers");
        for (int i=0;i<4;i++) out_pf[i]=0.0f;
        if (saturated) *saturated = false;
        return ESP_FAIL;
    }

    // Interpret consecutive pairs as signed 16-bit values (big-endian)
    for (int i=0;i<4;i++){
        int16_t raw = (int16_t)((buf[2*i] << 8) | buf[2*i+1]);
        // Convert raw to pF using simple scaling: assume full-scale corresponds to FDC_CAP_RANGE_PF
        // This is an approximation; we'll refine after observing raw values.
        float pf = ((float)raw) / 32768.0f * FDC_CAP_RANGE_PF;
        out_pf[i] = pf;
    }
    if (saturated) *saturated = false; // detection not implemented yet

    ESP_LOGD(TAG, "fdc_raw=%02x%02x %02x%02x %02x%02x %02x%02x", buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
    ESP_LOGI(TAG, "FDC: %0.3f pF, %0.3f pF, %0.3f pF, %0.3f pF", out_pf[0], out_pf[1], out_pf[2], out_pf[3]);

    return ESP_OK;
}