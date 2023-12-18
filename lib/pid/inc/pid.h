#ifndef APP_LIB_PID_INC_PID_H_
#define APP_LIB_PID_INC_PID_H_

typedef struct MmrPid {
  float kp;  /** proportional coefficient */
  float ki;  /** integral coefficient */
  float kd;  /** derivative coefficient */

  float lastError;  /** last pid compensator output */
  float integral;  /** summation of previous errors */

  float lastInput;
  float lastOutput;

  float sati_min;  /** lower bound of integral part saturation */
  float sati_max;  /** upper bound of integral part saturation */

  float sat_min;  /** lower bound of outpout error saturation */
  float sat_max;  /** upper bound of outpout error saturation */
} MmrPid;

/**
 * MmrPid minimal constructor
*/
MmrPid MMR_PID(float kp, float ki, float kd);

/**
 * Compute PID compensation
 * @param self instance of MmrPid "object"
 * @param setpoint control's desired target
 * @param processvar current value of the process variable
 * @return compensation value clamped between sat_min and sat_max
*/
float MMR_PID_Compensate(MmrPid *self, float setpoint, float processvar);

void MMR_PID_SetOutputClamp(MmrPid *self, float inf, float sup);
void MMR_PID_SetProportionalClamp(MmrPid *self, float inf, float sup);
void MMR_PID_SetIntegralClamp(MmrPid *self, float inf, float sup);
void MMR_PID_SetDerivativeClamp(MmrPid *self, float inf, float sup);

float MMR_PID_GetOutputClampInf(MmrPid *self);
float MMR_PID_GetIntegralClampInf(MmrPid *self);
float MMR_PID_GetOutputClampSup(MmrPid *self);
float MMR_PID_GetIntegralClampSup(MmrPid *self);
float MMR_PID_GetKp(MmrPid *self);
float MMR_PID_GetKi(MmrPid *self);
float MMR_PID_GetKd(MmrPid *self);

#endif  // !APP_LIB_PID_INC_PID_H_
