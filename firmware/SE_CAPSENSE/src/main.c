#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "board.h"
#include "config.h"
#include "timebase.h"
#include "scheduler.h"
#include "sampler.h"
/* SD logging disabled for now while working on WiFi/MQTT
#include "sd_logger.h" */
#include "uart_cli.h"
#include "wifi_svc.h"
#include "mqtt_svc.h"
#include "i2c_bus.h"
#include "bme280_drv.h"


// ===== SET THESE TO YOUR PCB =====
#define I2C_PORT        I2C_NUM_0
#define I2C_SDA_GPIO    4       // <-- change
#define I2C_SCL_GPIO    3       // <-- change
#define I2C_FREQ_HZ     100000  // start slow; later 400000
// =================================

static void i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_GPIO,
        .scl_io_num = I2C_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,  // OK even if you have external pullups
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ,
        .clk_flags = 0,
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0));
}

static bool i2c_probe(uint8_t addr)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | 0, true); // write
    i2c_master_stop(cmd);

    esp_err_t err = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(25));
    i2c_cmd_link_delete(cmd);

    return (err == ESP_OK);
}

static const char *TAG = "app";

void app_main(void)
{
    ESP_LOGI(TAG, "Starting system");

    // Init I2C bus
    esp_err_t r = i2c_bus_init();
    if (r != ESP_OK) {
        ESP_LOGE(TAG, "i2c_bus_init failed: %d", r);
        // can't continue without I2C; halt
        while (1) vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Init BME sensor (log and continue if absent)
    r = bme_init();
    if (r != ESP_OK) {
        ESP_LOGW(TAG, "BME init failed: %d; continuing without BME", r);
    }

    // SD logging temporarily disabled while debugging WiFi/MQTT
    ESP_LOGI(TAG, "SD logging is disabled for this session");
    (void)r; // keep variable referenced

    // Configure LED GPIOs (ensure pins configured even if SD init failed)
    gpio_reset_pin(LED_SENSE_GPIO);
    gpio_reset_pin(LED_SD_GPIO);
    gpio_set_direction(LED_SENSE_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_SD_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_SENSE_GPIO, 0);
    gpio_set_level(LED_SD_GPIO, 0);

    // Init other modules
    sampler_init();

    // Start simple UART CLI so user can enter Wi‑Fi credentials if needed
    cli_init();

    // Init Wi-Fi (but DO NOT start sensor sampling until we have connectivity)
    r = wifi_svc_init();
    if (r != ESP_OK){
        ESP_LOGW(TAG, "wifi_svc_init failed: %d; continuing without Wi-Fi", r);
    } else {
        // schedule periodic Wi-Fi status reports every 10s
        sch_add(wifi_svc_job_report, 10000);

        // Wait for Wi‑Fi to connect (no timeout) — user must provide credentials via UART if none stored
        ESP_LOGI(TAG, "Waiting for Wi‑Fi connection... (enter credentials via UART if needed)");
        while (wifi_svc_wait_connected(15000) != ESP_OK){
            ESP_LOGW(TAG, "Still waiting for Wi‑Fi connection...");
            // continue looping until connected; user can use UART CLI to set credentials
        }

        ESP_LOGI(TAG, "Wi‑Fi connected; initializing MQTT and starting sampling");
        if (mqtt_svc_init() != ESP_OK){
            ESP_LOGW(TAG, "mqtt_svc_init failed; MQTT disabled");
        } else {
            // Schedule MQTT drain job if implementation available
            sch_add(mqtt_svc_job_drain, MQTT_PUB_PERIOD_MS);
        }

        // only start sampling after Wi‑Fi connected
        sampler_init();
        sch_add(sampler_job, 1000);
    }

    // flush records to SD every 5s
    // sd logging disabled; sch_add(sdlog_job_flush, 5000);

    // main loop: run scheduler
    while (1) {
        sch_run_due();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

