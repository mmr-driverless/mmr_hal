#include "inc/spi.h"

int MMR_SPI_GetSelectedSlave(MmrSpi* mmrSpi) {
  return mmrSpi->__getSelectedSlave(); 
}

bool MMR_SPI_SelectSlave(MmrSpi* mmrSpi, int idx) {
  return mmrSpi->__selectSlave(idx); 
}

void MMR_SPI_DeselectSlave(MmrSpi* mmrSpi) {
  mmrSpi->__deselectSlave(); 
}

bool MMR_SPI_Transmit(MmrSpi* mmrSpi, uint8_t* buf, int size) {
  return mmrSpi->__transmit(buf, size); 
}

bool MMR_SPI_Receive(MmrSpi* mmrSpi, uint8_t* buf, int size) {
  return mmrSpi->__receive(buf, size); 
}

bool MMR_SPI_TransmitReceive(MmrSpi* mmrSpi, uint8_t* txBuf, uint8_t* rxBuf, int size) {
  return mmrSpi->__transmitReceive(txBuf, rxBuf, size); 
}
