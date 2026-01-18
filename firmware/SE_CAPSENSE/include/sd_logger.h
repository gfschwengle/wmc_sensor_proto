#pragma once
#include <esp_err.h>

esp_err_t sdlog_init(void);
void sdlog_job_flush(void);