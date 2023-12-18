#include "inc/pid.h"

float MMR_PID_GetOutputClampInf(MmrPid *self) {
  return self->sat_min;
}

float MMR_PID_GetIntegralClampInf(MmrPid *self) {
  return self->sati_min;
}

float MMR_PID_GetOutputClampSup(MmrPid *self) {
  return self->sat_max;
}

float MMR_PID_GetIntegralClampSup(MmrPid *self) {
  return self->sati_max;
}

float MMR_PID_GetKp(MmrPid *self) {
  return self->kp;
}

float MMR_PID_GetKi(MmrPid *self) {
  return self->ki;
}

float MMR_PID_GetKd(MmrPid *self) {
  return self->kd;
}
