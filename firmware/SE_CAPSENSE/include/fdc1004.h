#pragma once
#include <esp_err.h>
#include <stdbool.h>

#define FDC1004_I2C_ADDR 0x50

esp_err_t fdc_init(void);
esp_err_t fdc_config_default(void);
esp_err_t fdc_read_pf(float out_pf[4], bool *saturated);