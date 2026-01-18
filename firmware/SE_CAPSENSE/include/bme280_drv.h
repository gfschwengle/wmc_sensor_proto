#pragma once
#include <esp_err.h>

#define BME280_I2C_ADDR 0x76

esp_err_t bme_init(void);
esp_err_t bme_read(float *temp_c, float *hum_pct, float *pres_hpa);