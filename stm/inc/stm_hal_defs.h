#ifndef __STM32F4xx_HAL_H

#ifndef APP_STM_INC_STM_HAL_DEFS_H_
#define APP_STM_INC_STM_HAL_DEFS_H_

typedef enum {
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03
} HAL_StatusTypeDef;

#define HAL_MAX_DELAY      0xFFFFFFFFU
#endif

#endif