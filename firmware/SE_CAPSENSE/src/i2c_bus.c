#include "board.h"
#include "driver/i2c.h"
#include "esp_log.h"

#define TAG "i2c"

esp_err_t i2c_bus_init(void){
i2c_config_t conf = {
.mode = I2C_MODE_MASTER,
.sda_io_num = I2C_SDA_GPIO,
.scl_io_num = I2C_SCL_GPIO,
.sda_pullup_en = GPIO_PULLUP_ENABLE,
.scl_pullup_en = GPIO_PULLUP_ENABLE,
.master.clk_speed = I2C_FREQ_HZ,
};
ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
    esp_err_t r = i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    ESP_LOGI(TAG, "i2c_bus_init: %d", r);
    return r;
}


static esp_err_t i2c_xfer(uint8_t addr, uint8_t reg, uint8_t *buf, size_t len, bool read){
i2c_cmd_handle_t cmd = i2c_cmd_link_create();
i2c_master_start(cmd);
i2c_master_write_byte(cmd, (addr<<1) | 0, true);
i2c_master_write_byte(cmd, reg, true);
if (read){
i2c_master_start(cmd);
i2c_master_write_byte(cmd, (addr<<1) | 1, true);
i2c_master_read(cmd, buf, len, I2C_MASTER_LAST_NACK);
} else {
i2c_master_write(cmd, buf, len, true);
}
i2c_master_stop(cmd);
esp_err_t r = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(20));
i2c_cmd_link_delete(cmd);
return r;
}

esp_err_t i2c_rd(uint8_t a, uint8_t r, uint8_t *b, size_t l){ return i2c_xfer(a,r,b,l,true); }
esp_err_t i2c_wr(uint8_t a, uint8_t r, const uint8_t *b, size_t l){ return i2c_xfer(a,r,(uint8_t*)b,l,false); }