#ifndef STM32F4xx_HAL_SPI_H

#ifndef APP_STM_INC_STM_HAL_SPI_DEFS_H_
#define APP_STM_INC_STM_HAL_SPI_DEFS_H_

#include <stdint.h>
#include "stm_hal_defs.h"

typedef struct SPI_HandleTypeDef SPI_HandleTypeDef;

HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);

#endif
#endif