#include "inc/pid.h"

void MMR_PID_SetOutputClamp(MmrPid *self, float inf, float sup) {
  self->sat_min = inf;
  self->sat_max = sup;
}

void MMR_PID_SetIntegralClamp(MmrPid *self, float inf, float sup) {
  self->sati_min = inf;
  self->sati_max = sup;
}
