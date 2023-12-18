#ifndef MMR_LEVER_H
#define MMR_LEVER_H

#include <averaging.h>
#include <interval.h>

typedef struct {
  MmrEMA readingEma;
  float value;

  float zeroReading;
  float oneReading;
} MmrLever;

/**
 * @param zeroReading lever's adc value when clutch is fully released
 * @param oneReading lever's adc value when clutch is fully engaged
*/
MmrLever MMR_LEVER(float zeroReading, float oneReading, float emaAlpha);

void MMR_LEVER_AddReading(MmrLever* lever, float reading);
float MMR_LEVER_GetValue(MmrLever* lever);

#endif