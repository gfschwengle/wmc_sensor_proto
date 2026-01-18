#pragma once
#include <driver/gpio.h>
#include <driver/sdspi_host.h>

#define I2C_SCL_GPIO 3
#define I2C_SDA_GPIO 4
#define I2C_FREQ_HZ 400000

#define LED_GPIO_1 0
#define LED_GPIO_2 8
#define BUTTON_GPIO 9

// Named LED pins for functionality
#define LED_SENSE_GPIO LED_GPIO_1  // toggles during sensor read
#define LED_SD_GPIO    LED_GPIO_2  // toggles when writing to SD

// SD over SPI (example pins - adjust to your PCB)
#define SD_SPI_HOST SPI2_HOST
#define SD_MOSI_GPIO    7
#define SD_MISO_GPIO    2
#define SD_SCK_GPIO     6
#define SD_CS_GPIO      5

#define CONSOLE_TX_GPIO 12
#define CONSOLE_RX_GPIO 11