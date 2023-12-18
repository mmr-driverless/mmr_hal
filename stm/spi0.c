/*
STM implementation of the MMR SPI interface.
The library is suited only to be used as an SPI master, not as a slave.
*/

#include "inc/spi0.h"
#include <stddef.h>


int __selectedSlave = -1;
MmrPin** __slavePins = NULL;
int __slavePinsCount = 0;
SPI_HandleTypeDef* __spiHandle = NULL;

/* Interface implementation */

static int __mmr_spi0_GetSelectedSlave() {
  return __selectedSlave;
}

static void __mmr_spi0_DeselectSlave() {
  if (__selectedSlave != -1) {
    MMR_PIN_Write(__slavePins[__selectedSlave], MMR_PIN_LOW);
    __selectedSlave = -1;
  }
}

static bool __mmr_spi0_SelectSlave(int idx) {
  if (__slavePins == 0 || idx < 0 || idx >= __slavePinsCount) 
    return false;

  __mmr_spi0_DeselectSlave();

  MMR_PIN_Write(__slavePins[idx], MMR_PIN_HIGH);
  __selectedSlave = idx;

  return true;
}

static bool __mmr_spi0_Transmit(uint8_t* buf, int size) {
  HAL_StatusTypeDef status = HAL_SPI_Transmit(__spiHandle, buf, size, HAL_MAX_DELAY);
  return status == HAL_OK;
}
static bool __mmr_spi0_Receive(uint8_t* buf, int size) {
  HAL_StatusTypeDef status = HAL_SPI_Receive(__spiHandle, buf, size, HAL_MAX_DELAY);
  return status == HAL_OK;
}
static bool __mmr_spi0_TransmitReceive(uint8_t* txBuf, uint8_t* rxBuf, int size) {
  HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(__spiHandle, txBuf, rxBuf, size, HAL_MAX_DELAY);
  return status == HAL_OK;
}

MmrSpi spi0 = {
  .__deselectSlave = __mmr_spi0_DeselectSlave,
  .__getSelectedSlave = __mmr_spi0_GetSelectedSlave,
  .__receive = __mmr_spi0_Receive,
  .__selectSlave = __mmr_spi0_SelectSlave,
  .__transmit = __mmr_spi0_Transmit,
  .__transmitReceive = __mmr_spi0_TransmitReceive
};

/* Additional functions */


/** @brief Initializes the SPI library.
 *  @param slavePins A pointer to a list of MmrPins. No copy is performed, so the memory location should have the same lifetime as the library itself (mark it as static!).
*/
bool MMR_SPI0_Init(SPI_HandleTypeDef* spiHandle, MmrPin** slavePins, int slavePinsCount) {
  if (__slavePins != 0)
    __mmr_spi0_DeselectSlave();

  __slavePins = slavePins;
  __spiHandle = spiHandle;
  __slavePinsCount = slavePinsCount;

  return true;
}
