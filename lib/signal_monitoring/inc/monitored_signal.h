#ifndef MMR_MONITORED_SIGNAL_H
#define MMR_MONITORED_SIGNAL_H

#include <delay.h>
#include <stdbool.h>

typedef struct MmrMonitoredSignal {
  /** @brief Maximum time allowed between updates. */
  MmrDelay updateTimeout;

  /** @brief Maximum time allowed between OK updates. */
  MmrDelay okTimeout;

  /** @brief A predicate which determines whether a signal is OK or not. */
  bool (*ok)();
} MmrMonitoredSignal;

bool MMR_MONITORED_SIGNAL_IsOk(MmrMonitoredSignal* signal);
void MMR_MONITORED_SIGNAL_Updated(MmrMonitoredSignal* signal);

#endif