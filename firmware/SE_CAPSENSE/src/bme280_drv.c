#include "bme280_drv.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include <stdint.h>
#include <string.h>

#define TAG "bme"

// Registers
#define BME_REG_CHIPID          0xD0
#define BME_REG_RESET           0xE0
#define BME_REG_CTRL_HUM        0xF2
#define BME_REG_STATUS          0xF3
#define BME_REG_CTRL_MEAS       0xF4
#define BME_REG_CONFIG          0xF5
#define BME_REG_MEAS            0xF7 // pressure(3), temp(3), hum(2)

static struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;
} calib;

static int32_t t_fine;

static uint16_t read_u16_le(const uint8_t *b){ return (uint16_t)b[0] | ((uint16_t)b[1] << 8); }
static int16_t read_s16_le(const uint8_t *b){ return (int16_t)read_u16_le(b); }

esp_err_t bme_init(void){
    uint8_t id;
    esp_err_t r = i2c_rd(BME280_I2C_ADDR, BME_REG_CHIPID, &id, 1);
    if (r != ESP_OK){ ESP_LOGE(TAG, "i2c read chip id failed: %d", r); return r; }
    if (id != 0x60){ ESP_LOGE(TAG, "unexpected chip id 0x%02x", id); return ESP_FAIL; }

    // Read calibration block 0x88..0xA1
    uint8_t buf1[26];
    r = i2c_rd(BME280_I2C_ADDR, 0x88, buf1, sizeof(buf1));
    if (r != ESP_OK){ ESP_LOGE(TAG, "i2c read calib1 failed: %d", r); return r; }

    memcpy(&calib.dig_T1, &buf1[0], 2);
    calib.dig_T2 = (int16_t)read_u16_le(&buf1[2]);
    calib.dig_T3 = (int16_t)read_u16_le(&buf1[4]);
    calib.dig_P1 = read_u16_le(&buf1[6]);
    calib.dig_P2 = (int16_t)read_u16_le(&buf1[8]);
    calib.dig_P3 = (int16_t)read_u16_le(&buf1[10]);
    calib.dig_P4 = (int16_t)read_u16_le(&buf1[12]);
    calib.dig_P5 = (int16_t)read_u16_le(&buf1[14]);
    calib.dig_P6 = (int16_t)read_u16_le(&buf1[16]);
    calib.dig_P7 = (int16_t)read_u16_le(&buf1[18]);
    calib.dig_P8 = (int16_t)read_u16_le(&buf1[20]);
    calib.dig_P9 = (int16_t)read_u16_le(&buf1[22]);
    calib.dig_H1 = buf1[25];

    // Read humidity calibration 0xE1..0xE7
    uint8_t buf2[7];
    r = i2c_rd(BME280_I2C_ADDR, 0xE1, buf2, sizeof(buf2));
    if (r != ESP_OK){ ESP_LOGE(TAG, "i2c read calib2 failed: %d", r); return r; }
    calib.dig_H2 = (int16_t)read_u16_le(&buf2[0]);
    calib.dig_H3 = buf2[2];
    // H4 is ((buf2[3] << 4) | (buf2[4] & 0x0F)) signed
    calib.dig_H4 = (int16_t)((buf2[3] << 4) | (buf2[4] & 0x0F));
    // H5 is ((buf2[4] >> 4) | (buf2[5] << 4)) signed
    calib.dig_H5 = (int16_t)(((buf2[4] >> 4) & 0x0F) | (buf2[5] << 4));
    calib.dig_H6 = (int8_t)buf2[6];

    // Configure sensor: osrs_h = 1, osrs_t = 1, osrs_p = 1, mode = normal
    uint8_t ctrl_hum = 0x01; // osrs humidity x1
    r = i2c_wr(BME280_I2C_ADDR, BME_REG_CTRL_HUM, &ctrl_hum, 1);
    if (r != ESP_OK){ ESP_LOGE(TAG, "i2c write ctrl_hum failed: %d", r); return r; }

    uint8_t ctrl_meas = (1<<5) | (1<<2) | 0x03; // osrs_t=1, osrs_p=1, mode=normal
    r = i2c_wr(BME280_I2C_ADDR, BME_REG_CTRL_MEAS, &ctrl_meas, 1);
    if (r != ESP_OK){ ESP_LOGE(TAG, "i2c write ctrl_meas failed: %d", r); return r; }

    uint8_t config = 0x00; // no filter, standby 0.5ms
    r = i2c_wr(BME280_I2C_ADDR, BME_REG_CONFIG, &config, 1);
    if (r != ESP_OK){ ESP_LOGE(TAG, "i2c write config failed: %d", r); return r; }

    ESP_LOGI(TAG, "BME280 initialized (id=0x%02x)", id);
    return ESP_OK;
}

static int32_t comp_temp(int32_t adc_T){
    int64_t var1, var2;
    var1 = ((((int64_t)adc_T >> 3) - ((int64_t)calib.dig_T1 << 1)) * (int64_t)calib.dig_T2) >> 11;
    {
        int64_t x = (((int64_t)adc_T >> 4) - (int64_t)calib.dig_T1);
        var2 = ((x * x) >> 12) * (int64_t)calib.dig_T3 >> 14;
    }
    t_fine = (int32_t)(var1 + var2);
    int32_t T = (t_fine * 5 + 128) >> 8; // temperature in 0.01 deg C
    return T;
}

static uint32_t comp_pres(int32_t adc_P){
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib.dig_P3) >> 8) + ((var1 * (int64_t)calib.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1) * (int64_t)calib.dig_P1) >> 33;
    if (var1 == 0) return 0; // avoid exception
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)calib.dig_P7) << 4);
    return (uint32_t)p; // pressure in Pa << 8
}

static int32_t comp_hum(int32_t adc_H){
    int32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((int32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)calib.dig_H4) << 20) - (((int32_t)calib.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * (int32_t)calib.dig_H6) >> 10) * (((v_x1_u32r * (int32_t)calib.dig_H3) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * (int32_t)calib.dig_H2 + 8192) >> 14));
    v_x1_u32r = v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * (int32_t)calib.dig_H1) >> 4);
    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    return (v_x1_u32r >> 12); // humidity in % * 1024
}

esp_err_t bme_read(float *t, float *h, float *p){
    uint8_t buf[8];
    esp_err_t r = i2c_rd(BME280_I2C_ADDR, BME_REG_MEAS, buf, sizeof(buf));
    if (r != ESP_OK){ ESP_LOGE(TAG, "i2c read meas failed: %d", r); return r; }

    int32_t adc_P = ( (int32_t)buf[0] << 12 ) | ( (int32_t)buf[1] << 4 ) | (buf[2] >> 4 );
    int32_t adc_T = ( (int32_t)buf[3] << 12 ) | ( (int32_t)buf[4] << 4 ) | (buf[5] >> 4 );
    int32_t adc_H = ( (int32_t)buf[6] << 8 ) | buf[7];

    int32_t Traw = comp_temp(adc_T); // in 0.01 C
    uint32_t Praw = comp_pres(adc_P); // Pa << 8
    int32_t Hraw = comp_hum(adc_H); // % * 1024

    if (t) *t = ((float)Traw) / 100.0f;
    if (p) *p = ((float)Praw) / 256.0f / 100.0f; // convert Pa<<8 to hPa
    if (h) *h = ((float)Hraw) / 1024.0f;

    return ESP_OK;
}