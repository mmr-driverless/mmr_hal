#ifndef LIB_TIMING_INC_DELAY_H_
#define LIB_TIMING_INC_DELAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "timing.h"

typedef struct MmrDelay {
  uint32_t ms;
  Tick start;
} MmrDelay;

MmrDelay MMR_Delay(uint32_t delayMs);

/** @brief The delay will elapse this.ms ticks after a call to this. */
void MMR_DELAY_Reset(MmrDelay *delay);

/** @brief The delay will elapse immediately after a call to this. 
 * @note This only works when MMR_GetTick() > this.ms + 1
*/
void MMR_DELAY_Skip(MmrDelay *delay);

/** @brief Sets a new value for this.ms */
void MMR_DELAY_Change(MmrDelay *delay, uint32_t delayMs);

/** @brief If the delay has elapsed, this resets the delay and returns true. */
bool MMR_DELAY_WaitAsync(MmrDelay *delay);

/** @brief Returns whether the delay has elapsed, without resetting the delay. */
bool MMR_DELAY_WaitOnceAsync(MmrDelay *delay);

#endif // !LIB_TIMING_INC_DELAY_H_
