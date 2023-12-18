#include <monitored_signal.h>
#include <stddef.h>

bool MMR_MONITORED_SIGNAL_IsOk(MmrMonitoredSignal* signal) {
  const bool is_ok =
    signal->ok == NULL
    || signal->ok()
    || !MMR_DELAY_WaitOnceAsync(&signal->okTimeout);

  const bool recently_updated =
    signal->updateTimeout.ms <= 0
    || !MMR_DELAY_WaitOnceAsync(&signal->updateTimeout);

  return is_ok && recently_updated;
}

void MMR_MONITORED_SIGNAL_Updated(MmrMonitoredSignal* signal) {
  if (signal->ok != NULL && signal->ok()) {
    MMR_DELAY_Reset(&signal->okTimeout);
  }

  MMR_DELAY_Reset(&signal->updateTimeout);
}
