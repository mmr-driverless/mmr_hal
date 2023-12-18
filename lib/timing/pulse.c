#include <pulse.h>

void MMR_PULSE_Init(MmrPulse* pulse, Tick interval) {
  pulse->delay = MMR_Delay(interval);
  pulse->output = false;
}

bool MMR_PULSE_RunAsync(MmrPulse* pulse) {
  if (MMR_DELAY_WaitAsync(&pulse->delay))
    return pulse->output = !pulse->output;

  return pulse->output;
}

void MMR_PULSE_ChangeInterval(MmrPulse* pulse, Tick interval) {
  MMR_DELAY_Change(&pulse->delay, interval);
}
