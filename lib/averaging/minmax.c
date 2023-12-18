#include <minmax.h>
#include <math.h>
#include <util.h>

void MMR_MINMAX_AddValue(MmrMinMax* minmax, float value) {
  minmax->lastValue = value;

  float min = minmax->min;
  minmax->min = MIN(value, min);
  
  float max = minmax->max;
  minmax->max = MAX(value, max);
}

void MMR_MINMAX_Reset(MmrMinMax* minmax) {
  minmax->min = INFINITY;
  minmax->max = -INFINITY;
  minmax->lastValue = NAN;
}

float MMR_MINMAX_GetMostRecent(MmrMinMax* minmax) { return minmax->lastValue; }
float MMR_MINMAX_GetMin(MmrMinMax* minmax) { return minmax->min; }
float MMR_MINMAX_GetMax(MmrMinMax* minmax) { return minmax->max; }

float MMR_MINMAX_GetMaxAbs(MmrMinMax* minmax) {
  float absmin = fabsf(minmax->min);
  float absmax = fabsf(minmax->max);

  return MAX(absmin, absmax);
}