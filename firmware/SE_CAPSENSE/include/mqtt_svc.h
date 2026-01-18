#pragma once
#include <stdbool.h>
#include <esp_err.h>
#include "record.h"

esp_err_t mqtt_svc_init(void);
bool mqtt_svc_is_connected(void);
bool mqtt_svc_enqueue(const sample_t *s);
void mqtt_svc_job_drain(void);