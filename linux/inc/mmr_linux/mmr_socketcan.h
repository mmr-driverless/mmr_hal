#ifndef APP_STM_INC_CAN0_H_
#define APP_STM_INC_CAN0_H_

#include <stdbool.h>
#include <can.h>

extern MmrCan can0;
bool MMR_SOCKETCAN_Start(const char* interface);

#endif // !APP_STM_INC_CAN0_H_
