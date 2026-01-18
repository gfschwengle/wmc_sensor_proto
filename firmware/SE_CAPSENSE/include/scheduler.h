#pragma once
#include <stdint.h>

typedef void (*job_fn)(void);
void sch_add(job_fn fn, uint32_t period_ms);
void sch_run_due(void);