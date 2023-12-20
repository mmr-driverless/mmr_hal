#include "inc/delay.h"
#include "inc/timing.h"


MmrDelay MMR_Delay(sint64 delayMs) {
  return (MmrDelay) {
    .ms = delayMs,
    .start = 0
  };
}


void MMR_DELAY_Reset(MmrDelay *delay) {
  delay->start = MMR_GetTick();
}


void MMR_DELAY_Change(MmrDelay *delay, sint64 delayMs) {
  delay->ms = delayMs;
}

void MMR_DELAY_Skip(MmrDelay *delay) {
  delay->start = 1;
}


bool MMR_DELAY_WaitAsync(MmrDelay *delay) {
  Tick tick = MMR_GetTick();
  if (delay->start == 0) {
    delay->start = tick;
  }

  if (tick - delay->start >= delay->ms) {
    MMR_DELAY_Reset(delay);
    return true;
  }

  return false;
}


bool MMR_DELAY_WaitOnceAsync(MmrDelay *delay) {
  Tick tick = MMR_GetTick();
  if (delay->start == 0) {
    delay->start = tick;
  }

  if (tick - delay->start >= delay->ms) {
    return true;
  }

  return false;
}
