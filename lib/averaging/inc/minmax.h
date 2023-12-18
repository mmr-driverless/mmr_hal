#ifndef INC_MINMAX_H_
#define INC_MINMAX_H_

typedef struct {
  float lastValue;
  float min;
  float max;
} MmrMinMax;

void MMR_MINMAX_AddValue(MmrMinMax*, float value);
void MMR_MINMAX_Reset(MmrMinMax*);

float MMR_MINMAX_GetMostRecent(MmrMinMax* minmax);
float MMR_MINMAX_GetMaxAbs(MmrMinMax* minmax);
float MMR_MINMAX_GetMin(MmrMinMax* minmax);
float MMR_MINMAX_GetMax(MmrMinMax* minmax);

#endif /* INC_MINMAX_H_ */
