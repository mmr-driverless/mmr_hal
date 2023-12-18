

#include <timing.h>
#include <time.h>
#include <stdio.h>

static uint32_t __gettick() {
	struct timespec ts;
	
	if (0 != clock_gettime(CLOCK_MONOTONIC, &ts)) {
		perror("Clock gettime failed.");
		return 0;
	}

	return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

void MMR_TIMING_SetTickProviderToLinuxClock() {
  MMR_SetTickProvider(__gettick);
}