#ifndef APP_LIB_AVERAGING_INC_AVERAGING_H_
#define APP_LIB_AVERAGING_INC_AVERAGING_H_

#include "../../ring/inc/ring.h"

typedef struct MmrEMA {
  float lastSmoothed;  /** s(t-1) previously smoothed value */
  float alpha;
} MmrEMA;

typedef struct MmrSMA {
  MmrRing *ring;
  float sma;
  uint8_t firstRunSamples;
} MmrSMA;


/**
 * @return mean value of the ring's elements
*/
float MMR_AVERAGING_Mean(MmrRing *ring);

MmrEMA MMR_AVERAGING_EMA(float alpha);
float MMR_AVERAGING_SimpleMovingAverage(MmrSMA *SMA, float incomingSample);

MmrSMA MMR_AVERAGING_SMA(MmrRing *ring);
float MMR_AVERAGING_ExpMovingAverage(MmrEMA *EMA, float incomingSample);

#endif  // !APP_LIB_AVERAGING_INC_AVERAGING_H_