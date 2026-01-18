#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "config.h"

typedef struct {
uint64_t t_ms;
float cap_pf[4];
float temp_c, hum_pct, pres_hpa;
uint16_t flags;
} sample_t;

bool record_push(const sample_t *s);
bool record_pop(sample_t *s);
size_t record_count(void);