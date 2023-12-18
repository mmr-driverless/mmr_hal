#include "inc/averaging.h"

float MMR_AVERAGING_Mean(MmrRing *ring) {
  float sum = 0;
  for (int i=0; i<ring->elementsNum; i++) {
    sum += *((float*) MMR_RING_Get(ring, i));
  }

  return sum / ring->elementsNum;
}

MmrEMA MMR_AVERAGING_EMA(float alpha) {
  return (MmrEMA) {
    .lastSmoothed = 0,
    .alpha = alpha,
  };
}

float MMR_AVERAGING_ExpMovingAverage(MmrEMA *EMA, float incomingSample) {
  EMA->lastSmoothed = EMA->lastSmoothed + EMA->alpha * (incomingSample - EMA->lastSmoothed);
  return EMA->lastSmoothed;
}

MmrSMA MMR_AVERAGING_SMA(MmrRing *ring) {
  return (MmrSMA) {
    .ring = ring,
    .sma = 0,
    .firstRunSamples = 0,
  };
}

float MMR_AVERAGING_SimpleMovingAverage(MmrSMA *SMA, float incomingSample) {
  MMR_RING_Insert(SMA->ring, (uint8_t*) &incomingSample);

  if (SMA->firstRunSamples < SMA->ring->elementsNum) {
    SMA->sma = incomingSample;
    SMA->firstRunSamples++;
    return SMA->sma;
  }

  SMA->sma += (incomingSample - *((float*) MMR_RING_Get(SMA->ring, 0)));
  return SMA->sma;
}