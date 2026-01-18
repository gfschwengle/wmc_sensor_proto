#include "record.h"
#include "config.h"

static sample_t q[RECORD_RING_CAP];
static volatile size_t head=0, tail=0, count=0;

bool record_push(const sample_t *s){
if (count>=RECORD_RING_CAP) return false;
q[head] = *s; head=(head+1)%RECORD_RING_CAP; count++; return true;
}

bool record_pop(sample_t *s){
if (!count) return false;
*s = q[tail]; tail=(tail+1)%RECORD_RING_CAP; count--; return true;
}

size_t record_count(void){ return count; }