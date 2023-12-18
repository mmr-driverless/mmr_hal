#ifndef APP_STM_INC_SPI0_H_
#define APP_STM_INC_SPI0_H_

#include "../../lib/spi/inc/spi.h"

extern MmrSpi spi0;

#include "stm_hal_spi_defs.h"
#include "../../lib/gpio/inc/pin.h"


bool MMR_SPI0_Init(SPI_HandleTypeDef* spiHandle, MmrPin** pins, int count);

#endif // !APP_STM_INC_SPI0_H_