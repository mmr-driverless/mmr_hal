#ifndef MMR_PULSE_H
#define MMR_PULSE_H

#include <stdint.h>
#include "timing.h"
#include "delay.h"

typedef struct {
  bool output;
  MmrDelay delay;
} MmrPulse;

void MMR_PULSE_Init(MmrPulse* pulse, Tick interval);
bool MMR_PULSE_RunAsync(MmrPulse* pulse);
void MMR_PULSE_ChangeInterval(MmrPulse* pulse, Tick interval);

#endif // !MMR_PULSE_H
