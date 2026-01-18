#include "scheduler.h"
#include "timebase.h"

typedef struct { job_fn fn; uint32_t period; uint64_t next_due; } job_t;
static job_t jobs[12];
static int njobs = 0;

void sch_add(job_fn fn, uint32_t period){
jobs[njobs++] = (job_t){ .fn=fn, .period=period, .next_due=tb_now_ms()+period };
}

void sch_run_due(void){
uint64_t now = tb_now_ms();
for (int i=0;i<njobs;i++){
if ((int64_t)(now - jobs[i].next_due) >= 0){
jobs[i].fn();
jobs[i].next_due += jobs[i].period;
}
}
}