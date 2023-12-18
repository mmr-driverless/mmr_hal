#include <can_repeat.h>


void MMR_CAN_InitRepeatSendAsync(MmrCanRepeatSendAsyncState* state, MmrCan* can, const MmrCanMessage* message, int repetitions, uint32_t interval, bool ignoreErrors) {
  state->can = can;
  state->message = message;
  state->repetitions = repetitions;
  state->ignoreErrors = ignoreErrors;
  state->interval = (MmrDelay) { .ms = interval };
  state->__currentRepetition = 0;
};

void MMR_CAN_ResetRepeatSendAsync(MmrCanRepeatSendAsyncState* state) {
  state->__currentRepetition = 0;
}

MmrTaskResult MMR_CAN_RepeatSendAsync(MmrCanRepeatSendAsyncState* state) {
  const bool finished = state->__currentRepetition >= state->repetitions; 

  if (!finished && MMR_DELAY_WaitAsync(&state->interval)) {
    MmrTaskResult result = MMR_CAN_Send(state->can, state->message);

    if (!state->ignoreErrors && result == MMR_TASK_ERROR)
      return MMR_TASK_ERROR;

    ++state->__currentRepetition;
  }

  if (finished)
    return MMR_TASK_COMPLETED;

  return MMR_TASK_PENDING;
}