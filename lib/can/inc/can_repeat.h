#ifndef INC_MMR_CAN_REPEAT_H
#define INC_MMR_CAN_REPEAT_H

#include <util.h>
#include <can.h>
#include <delay.h>

typedef struct 
{
  MmrCan* can;
  const MmrCanMessage* message;
  int repetitions;
  bool ignoreErrors;
  MmrDelay interval;

  int __currentRepetition;
} MmrCanRepeatSendAsyncState;

void MMR_CAN_InitRepeatSendAsync(MmrCanRepeatSendAsyncState* state, MmrCan* can, const MmrCanMessage* message, int repetitions, uint32_t interval, bool ignoreErrors);

void MMR_CAN_ResetRepeatSendAsync(MmrCanRepeatSendAsyncState* state);
MmrTaskResult MMR_CAN_RepeatSendAsync(MmrCanRepeatSendAsyncState* state);

#endif