#ifndef INC_MMR_SPI_H_
#define INC_MMR_SPI_H_

#include <stdint.h>
#include <stdbool.h>

typedef int (*MmrSpiGetSelectedSlaveFn)();
typedef bool (*MmrSpiSelectSlaveFn)(int idx);
typedef void (*MmrSpiDeselectSlaveFn)();
typedef bool (*MmrSpiTransmitFn)(uint8_t* buf, int size);
typedef bool (*MmrSpiReceiveFn)(uint8_t* buf, int size);
typedef bool (*MmrSpiTransmitReceiveFn)(uint8_t* txBuf, uint8_t* rxBuf, int size);

typedef struct {
  MmrSpiGetSelectedSlaveFn __getSelectedSlave;
  MmrSpiSelectSlaveFn __selectSlave;
  MmrSpiDeselectSlaveFn __deselectSlave;
  MmrSpiTransmitFn __transmit;
  MmrSpiReceiveFn __receive;
  MmrSpiTransmitReceiveFn __transmitReceive;
} MmrSpi;

/** @returns The index of the selected slave, or -1 if no slave is selected. */
int MMR_SPI_GetSelectedSlave(MmrSpi* mmrSpi);

/** @brief Selects the specified slave after deselecting the currently selected slave. */
bool MMR_SPI_SelectSlave(MmrSpi* mmrSpi, int idx);

/** @brief Deselects the currently selected slave. */
void MMR_SPI_DeselectSlave(MmrSpi* mmrSpi);

bool MMR_SPI_Transmit(MmrSpi* mmrSpi, uint8_t* buf, int size);
bool MMR_SPI_Receive(MmrSpi* mmrSpi, uint8_t* buf, int size);
bool MMR_SPI_TransmitReceive(MmrSpi* mmrSpi, uint8_t* txBuf, uint8_t* rxBuf, int size);

#endif