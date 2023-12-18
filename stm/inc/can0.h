#ifndef APP_STM_INC_CAN0_H_
#define APP_STM_INC_CAN0_H_

#include "../../lib/can/inc/can.h"
#include "../../lib/can/inc/message.h"
#include <stdbool.h>

extern MmrCan can0;

// TODO: This is probably still wrong for other boards: f3xx defines CAN, f4xx defines CAN1 - other boards are untested, check if you get import errors
#if !defined(CAN) && !defined(CAN1)
#include "stm_hal_can_defs.h"
#endif

bool MMR_CAN0_Start(CAN_HandleTypeDef *hcan0);

#endif // !APP_STM_INC_CAN0_H_
