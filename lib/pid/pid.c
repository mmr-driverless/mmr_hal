#include "inc/pid.h"
#include "../util/inc/interval.h"
#include <float.h>


MmrPid MMR_PID(float kp, float ki, float kd) {
  return (MmrPid) {
    .kp = kp,
    .ki = ki,
    .kd = kd,
    
    .lastError = 0,
	.lastInput = 0,
    .integral = 0,

    .sati_min = -FLT_MAX,
    .sati_max = FLT_MAX,
    
    .sat_min = -FLT_MAX,
    .sat_max = FLT_MAX,
  };
}


float MMR_PID_Compensate(MmrPid *self, float setpoint, float input) {
  self->lastInput = input;

  float error = setpoint - input;
  self->integral = clamp(self->integral + error, self->sati_min, self->sati_max);

  float proportional = error * self->kp;
  float integral = self->integral * self->ki;
  float derivative = (error - self->lastError) * self->kd;

  self->lastError = error;
  self->lastOutput = proportional + integral + derivative;
  return clamp(
    self->lastOutput,
    self->sat_min,
    self->sat_max
  );
}
