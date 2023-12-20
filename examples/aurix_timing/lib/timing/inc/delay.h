#ifndef LIB_TIMING_INC_DELAY_H_
#define LIB_TIMING_INC_DELAY_H_

#ifndef sint64
#define sint64  long long
#endif

#include <stdint.h>
#include <stdbool.h>
#include "timing.h"

typedef struct MmrDelay {
  sint64 ms;
  Tick start;
} MmrDelay;

MmrDelay MMR_Delay(sint64 delayMs);

/** @brief The delay will elapse this.ms ticks after a call to this. */
void MMR_DELAY_Reset(MmrDelay *delay);

/** @brief The delay will ela  pse immediately after a call to this.
 * @note This only works when MMR_GetTick() > this.ms + 1
*/
void MMR_DELAY_Skip(MmrDelay *delay);

/** @brief Sets a new value for this.ms */
void MMR_DELAY_Change(MmrDelay *delay, sint64 delayMs);

/** @brief If the delay has elapsed, this resets the delay and returns true. */
bool MMR_DELAY_WaitAsync(MmrDelay *delay);

/** @brief Returns whether the delay has elapsed, without resetting the delay. */
bool MMR_DELAY_WaitOnceAsync(MmrDelay *delay);

#endif // !LIB_TIMING_INC_DELAY_H_


/*
IFX_INLINE sint32 IfxStm_getTicksFromMilliseconds(Ifx_STM *stm, uint32 milliSeconds)
{
    sint32 freq = (sint32)IfxStm_getFrequency(stm);
    return (freq / (1000)) * milliSeconds;

    ret millisec = tick*1000/get_freq
}

IFX_INLINE void IfxStm_waitTicks(Ifx_STM *stm, uint32 ticks)
{
    uint32 beginTime;

    beginTime = IfxStm_getLower(stm);
    return  ((IfxStm_getLower(stm) - beginTime) < ticks);


}
*/
