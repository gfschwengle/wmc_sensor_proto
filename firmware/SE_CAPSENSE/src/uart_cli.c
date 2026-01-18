#include "uart_cli.h"
#include "esp_log.h"
#include "driver/uart.h"
#include <string.h>
#include "wifi_svc.h"

static const char *TAG = "cli";
static void cli_task(void *arg){
    (void)arg;
    const int RX_BUF_SIZE = 256;
    uint8_t *data = malloc(RX_BUF_SIZE);
    char line[256];
    int line_pos = 0;
    ESP_LOGI(TAG, "UART CLI active. Commands: 'wifi set <ssid> <psk>', 'wifi show', 'wifi clear', 'wifi prompt', 'help'");

    // Use UART0 (console)
    uart_driver_install(UART_NUM_0, RX_BUF_SIZE * 2, 0, 0, NULL, 0);

    // If no Wi‑Fi credentials are stored, immediately prompt the user interactively at boot
    char ssid[WIFI_MAX_SSID_LEN] = {0};
    char psk[WIFI_MAX_PSK_LEN] = {0};
    if (wifi_svc_read_credentials(ssid, sizeof(ssid), psk, sizeof(psk)) != ESP_OK){
        ESP_LOGI(TAG, "No stored Wi‑Fi credentials. Starting interactive prompt now. Type 'skip' to bypass.");

        // Start interactive SSID/PSK prompt immediately
        ESP_LOGI(TAG, "Enter SSID (or 'skip' to continue without Wi‑Fi):");
        int ssid_len = 0;
        int r = uart_read_bytes(UART_NUM_0, (uint8_t*)ssid, sizeof(ssid)-1, pdMS_TO_TICKS(30000));
        if (r > 0){
            while (r > 0 && (ssid[r-1] == '\n' || ssid[r-1] == '\r')) r--;
            ssid[r] = '\0';
            ssid_len = r;
        }

        if (ssid_len > 0 && strcmp(ssid, "skip") != 0){
            ESP_LOGI(TAG, "Enter PSK (will echo):");
            int psk_len = 0;
            r = uart_read_bytes(UART_NUM_0, (uint8_t*)psk, sizeof(psk)-1, pdMS_TO_TICKS(30000));
            if (r > 0){
                while (r > 0 && (psk[r-1] == '\n' || psk[r-1] == '\r')) r--;
                psk[r] = '\0';
                psk_len = r;
            }
            if (psk_len > 0){
                if (wifi_svc_set_credentials(ssid, psk) == ESP_OK){
                    ESP_LOGI(TAG, "Credentials stored and connection attempted (SSID=%s)", ssid);
                } else {
                    ESP_LOGE(TAG, "Failed to store credentials");
                }
            } else {
                ESP_LOGI(TAG, "No PSK entered; cancelling prompt");
            }
        } else {
            ESP_LOGI(TAG, "Interactive Wi‑Fi prompt skipped");
        }
    }

    while (1){
        int len = uart_read_bytes(UART_NUM_0, data, RX_BUF_SIZE, pdMS_TO_TICKS(200));
        if (len > 0){
            for (int i=0;i<len;i++){
                uint8_t c = data[i];
                if (c == '\r' || c == '\n'){
                    if (line_pos == 0) continue;
                    line[line_pos] = '\0';
                    // process line
                    if (strncmp(line, "wifi set ", 9) == 0){
                        // parse ssid and psk
                        char ssid[WIFI_MAX_SSID_LEN];
                        char psk[WIFI_MAX_PSK_LEN];
                        int r = sscanf(line + 9, "%31s %63s", ssid, psk);
                        if (r == 2){
                            if (wifi_svc_set_credentials(ssid, psk) == ESP_OK){
                                ESP_LOGI(TAG, "Credentials stored and connection attempted (SSID=%s)", ssid);
                            } else {
                                ESP_LOGE(TAG, "Failed to store credentials");
                            }
                        } else {
                            ESP_LOGI(TAG, "Usage: wifi set <ssid> <psk>");
                        }
                    } else if (strcmp(line, "wifi show") == 0){
                        char ssid[WIFI_MAX_SSID_LEN] = {0};
                        char psk[WIFI_MAX_PSK_LEN] = {0};
                        if (wifi_svc_read_credentials(ssid, sizeof(ssid), psk, sizeof(psk)) == ESP_OK){
                            // mask psk
                            for (size_t i=0;i<strlen(psk);i++) psk[i] = '*';
                            ESP_LOGI(TAG, "Stored credentials: SSID='%s' PSK='%s'", ssid, psk);
                        } else {
                            ESP_LOGI(TAG, "No stored credentials");
                        }
                    } else if (strcmp(line, "wifi clear") == 0){
                        if (wifi_svc_clear_credentials() == ESP_OK) ESP_LOGI(TAG, "Credentials cleared"); else ESP_LOGE(TAG, "Failed to clear credentials");
                    } else if (strcmp(line, "wifi prompt") == 0){
                        ESP_LOGI(TAG, "Starting interactive Wi‑Fi prompt. Type SSID then PSK.");
                        // reuse the same logic as above
                        ESP_LOGI(TAG, "Enter SSID:");
                        int ssid_len = 0;
                        while (ssid_len == 0){
                            int r = uart_read_bytes(UART_NUM_0, (uint8_t*)ssid, sizeof(ssid)-1, pdMS_TO_TICKS(30000));
                            if (r > 0){
                                while (r > 0 && (ssid[r-1] == '\n' || ssid[r-1] == '\r')) r--;
                                ssid[r] = '\0';
                                ssid_len = r;
                            } else {
                                ESP_LOGI(TAG, "No SSID entered; cancelling prompt");
                                break;
                            }
                        }
                        if (ssid_len > 0){
                            ESP_LOGI(TAG, "Enter PSK (will echo):");
                            int psk_len = 0;
                            while (psk_len == 0){
                                int r = uart_read_bytes(UART_NUM_0, (uint8_t*)psk, sizeof(psk)-1, pdMS_TO_TICKS(30000));
                                if (r > 0){
                                    while (r > 0 && (psk[r-1] == '\n' || psk[r-1] == '\r')) r--;
                                    psk[r] = '\0';
                                    psk_len = r;
                                } else {
                                    ESP_LOGI(TAG, "No PSK entered; cancelling prompt");
                                    break;
                                }
                            }
                            if (psk[0] != '\0'){
                                if (wifi_svc_set_credentials(ssid, psk) == ESP_OK){
                                    ESP_LOGI(TAG, "Credentials stored and connection attempted (SSID=%s)", ssid);
                                } else {
                                    ESP_LOGE(TAG, "Failed to store credentials");
                                }
                            }
                        }
                    } else if (strcmp(line, "help") == 0){
                        ESP_LOGI(TAG, "Commands: 'wifi set <ssid> <psk>', 'wifi show', 'wifi clear', 'wifi prompt', 'help'");
                    } else {
                        ESP_LOGI(TAG, "Unknown command: %s", line);
                        ESP_LOGI(TAG, "Type 'help' for commands");
                    }
                    line_pos = 0;
                } else if (line_pos < (int)sizeof(line)-1){
                    line[line_pos++] = (char)c;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void cli_init(void){
    xTaskCreate(cli_task, "cli", 4096, NULL, 5, NULL);
}

void cli_job_poll(void){}
