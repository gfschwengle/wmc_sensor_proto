# capboard-fw

- Build: `pio run -e esp32c3`
- Flash: `pio run -e esp32c3 -t upload`
- Monitor: `pio device monitor -b 115200`

Folders of interest:
- `src/` super-loop app and services
- `include/` headers
- `test/` PlatformIO Unity tests

Next steps:
1. Fill `fdc1004.c` with real register config + conversion.
2. Implement CSV writer in `sd_logger.c`.
3. Hook up ESP-IDF `esp_mqtt_client` in `mqtt_svc.c` and publish JSON to `MQTT_BASE_TOPIC/samples`.