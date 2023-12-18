#include <lever.h>

MmrLever MMR_LEVER(float zeroReading, float oneReading, float emaAlpha) {
  return (MmrLever) {
    .readingEma = MMR_AVERAGING_EMA(emaAlpha),
    .zeroReading = zeroReading,
    .oneReading = oneReading
  };
}

void MMR_LEVER_AddReading(MmrLever* lever, float reading) {
  float avg = MMR_AVERAGING_ExpMovingAverage(&lever->readingEma, reading);
  lever->value = normalize(avg, lever->zeroReading, lever->oneReading);
}

float MMR_LEVER_GetValue(MmrLever* lever) {
  return lever->value;
}
