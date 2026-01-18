#pragma once
#include <esp_err.h>
#include <stddef.h>
#include <stdint.h>

esp_err_t i2c_bus_init(void);
esp_err_t i2c_rd(uint8_t addr, uint8_t reg, uint8_t *buf, size_t len);
esp_err_t i2c_wr(uint8_t addr, uint8_t reg, const uint8_t *buf, size_t len);