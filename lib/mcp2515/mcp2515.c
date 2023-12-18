#include "__inc/__mcp2515_errmgmt.h"
#include "inc/mcp2515.h"
#include "inc/mcp2515_lowlevel.h"

#define RESET_DELAY 100
static MmrDelay __resetProcDelay;

/*

MMR CAN Interface implementation

*/

/** @brief Configures the MCP2515 filter.
 * @param filter A pointer to the structure containing the filter configuration.
 * @returns true if the filter has been configured, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 * @exception MMR_MCP2515_ERROR_NOT_IN_CONFIGURATION_MODE
 */
static bool __mmr_mcp2515_trySetFilter(MmrCanFilter *filter) {
  bool isInConfigurationMode;
  if (!MMR_MCP2515LL_IsInMode(MMR_MCP2515_MODE_CONFIGURATION, &isInConfigurationMode)) {
    return false;
  }

  if (!isInConfigurationMode) {
    __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_NOT_IN_CONFIGURATION_MODE);
    return false;
  }

  /* 
  Our library supports only one filter. Therefore,

  we configure the MCP2515 by configuring all the filters to the same one
  (so that messages rejected by RXB0 are always rejected by the other buffers too)

  and enable RXB0 rollover in RXB1 (because if we would receive a valid message while RXB0 is full,
  even though RXB1 would accept it, it would be rejected and an overflow interrupt would be generated)
  */

  /* 
  store in RXM0 RXM1, RXF0 RXF2 to set it up for both buffers

  set RXBnCTRL[6:5] = 0b00 to enable filtering, 0b11 to disable it.
  set RXB0CTRL[2] = 1 to enable RXB0 rollover into RXB1
  */

  // setup RXBnCTRL
  if (!MMR_MCP2515LL_OpBitModify(MMR_MCP2515LL_REG_RXB0CTRL, MMR_MCP2515LL_RXB_MASK_FILMODE | MMR_MCP2515LL_RXB_MASK_BUKT, MMR_MCP2515LL_RXB_MASK_BUKT)
      || !MMR_MCP2515LL_OpBitModify(MMR_MCP2515LL_REG_RXB1CTRL, MMR_MCP2515LL_RXB_MASK_FILMODE, 0))
    return false;
  
  // Write filters
  return MMR_MCP2515LL_ConfigureFilterMask(MMR_MCP2515LL_FILTERMASK_0, filter->mask)
      && MMR_MCP2515LL_ConfigureFilterMask(MMR_MCP2515LL_FILTERMASK_1, filter->mask)
      && MMR_MCP2515LL_ConfigureFilterId(MMR_MCP2515LL_FILTERID_0, filter->id, !filter->isExtendedId)
      && MMR_MCP2515LL_ConfigureFilterId(MMR_MCP2515LL_FILTERID_1, filter->id, !filter->isExtendedId)
      && MMR_MCP2515LL_ConfigureFilterId(MMR_MCP2515LL_FILTERID_2, filter->id, !filter->isExtendedId)
      && MMR_MCP2515LL_ConfigureFilterId(MMR_MCP2515LL_FILTERID_3, filter->id, !filter->isExtendedId)
      && MMR_MCP2515LL_ConfigureFilterId(MMR_MCP2515LL_FILTERID_4, filter->id, !filter->isExtendedId)
      && MMR_MCP2515LL_ConfigureFilterId(MMR_MCP2515LL_FILTERID_5, filter->id, !filter->isExtendedId);
}

/**
 * @param txBuf Will contain the first free transmission buffer if the function returns true.
 * @returns Whether any transmission buffer is free.
 */
static bool __mmr_mcp2515_getFirstFreeTxBuffer(uint8_t status, MmrMcp2515LLTxBuf* txBuf) {
  if ((status & MMR_MCP2515LL_STATUS_MASK_TX0_PENDINGTX) == 0) {
    *txBuf = MMR_MCP2515LL_TXBUF0;
    return true;
  }
  
  if ((status & MMR_MCP2515LL_STATUS_MASK_TX1_PENDINGTX) == 0) {
    *txBuf = MMR_MCP2515LL_TXBUF1;
    return true;
  }
  
  if ((status & MMR_MCP2515LL_STATUS_MASK_TX2_PENDINGTX) == 0) {
    *txBuf = MMR_MCP2515LL_TXBUF2;
    return true;
  }

  return false;
}

/** @brief Schedules a message for transmission.
 * @param message A pointer to the structure which contains the message to transmit.
 * @returns true if a message has been scheduled for transmission, false otherwise
 * @exception MMR_MCP2515_ERROR_SPI
 * @exception MMR_MCP2515_ERROR_NO_FREE_TXBUF
 */
static bool __mmr_mcp2515_send(MmrCanMessage *message) {
  uint8_t status;
  if (!MMR_MCP2515LL_OpReadStatus(&status))
    return false;

  __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_OK);
  MmrMcp2515LLTxBuf txBuf;
  if (!__mmr_mcp2515_getFirstFreeTxBuffer(status, &txBuf)) {
    __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_NO_FREE_TXBUF);
    return false;
  }

  return MMR_MCP2515LL_OpLoadTxBuf(txBuf, message->id, message->isStandardId, message->payload, message->length)
      && MMR_MCP2515LL_OpRts(txBuf);
}

/** @brief Reads the first available message.
 * @param message A pointer to the structure which will contain the read message if the function returns true.
 * @returns true if a message has been read, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 * @exception MMR_MCP2515_ERROR_NO_PENDING_MESSAGE
 */
static bool __mmr_mcp2515_receive(MmrCanMessage *message) {
  uint8_t status;
  if (!MMR_MCP2515LL_OpReadStatus(&status))
    return false;

  __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_OK);
  MmrMcp2515LLRxBuf buf;
  if ((status & MMR_MCP2515LL_STATUS_MASK_RX0_FULL) != 0) {
    buf = MMR_MCP2515LL_RXBUF0;
  }

  else if ((status & MMR_MCP2515LL_STATUS_MASK_RX1_FULL) != 0) {
    buf = MMR_MCP2515LL_RXBUF1;
  }

  else {
    __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_NO_PENDING_MESSAGE);
    return false;
  }

  return MMR_MCP2515LL_OpReadRxBuf(buf, &(message->id), &(message->isStandardId), message->payload, &(message->length));
}

/** @brief Gets how many pending messages there are.
 * @returns -1 if an error occurs; 0, 1 or 2 otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
static uint8_t __mmr_mcp2515_getPendingMessages() {
  uint8_t status;
  if (!MMR_MCP2515LL_OpReadStatus(&status))
    return -1;
  
  return ((status & MMR_MCP2515LL_STATUS_MASK_RX0_FULL) != 0) + ((status & MMR_MCP2515LL_STATUS_MASK_RX1_FULL) != 0);
}

MmrCan mcp2515 = {
  .__trySend = __mmr_mcp2515_send,
  .__tryReceive = __mmr_mcp2515_receive,
  .__trySetFilter = __mmr_mcp2515_trySetFilter,
  .__getPendingMessages = __mmr_mcp2515_getPendingMessages,
};

/**
 * @brief Initializes the internal state of the MCP2515 library, without performing any action on the actual chip.
*/
void MMR_MCP2515_Init(MmrSpi* spi, int slaveIdx) {
  MMR_MCP2515LL_Init(spi, slaveIdx);
  __resetProcDelay = MMR_Delay(RESET_DELAY);
}

/**
 * @brief Asynchronously resets the MCP2515 to the same state it would be if it had just been powered on.
 *        The first call starts the procedure, then this function is supposed to be called in a loop
 *        until it returns true or an error is returned by MMR_MCP2515_GetLastError
 *        After it returns true, the next call will start a new reset procedure.
 *        Therefore, you MUST run this until it returns true.
 * @param max_attempts Akin to a timeout, the maximum amount of reset attempts to perform if the MCP2515 hasn't reset yet before terminating the procedure.
 * @returns true if the operation completed successfully, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 * @exception MMR_MCP2515_ERROR_OUT_OF_ATTEMPTS
 */
bool MMR_MCP2515_ResetAsync(int max_attempts) {
  static bool resetProcedureIsRunning = false;
  static int attempt = 0;
  __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_OK);

  /*
  The MCP2515 supports a reset operation:
  After invoking the operation, the MCP2515 takes some time to reset,
  and during that time it only writes zeroes in SPI data,
  which means that we read mode NORMAL (equal to 0).

  After the MCP2515 is successfully reset, it automatically goes into CONFIGURATION mode.

  We attempt the reset operation multiple times, as i had difficulties.
  This has been proposed some time ago on https://github.com/raspberrypi/linux/issues/2767
  */

  // If it is the first time we run the operation, invoke the reset operation before doing anything.
  if (!resetProcedureIsRunning) {
    if (!MMR_MCP2515LL_OpReset())
      return false;

    attempt = 1;
    resetProcedureIsRunning = true;
    MMR_DELAY_Reset(&__resetProcDelay);
    return false;
  }

  if (attempt > max_attempts) {
    resetProcedureIsRunning = false;
    __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_OUT_OF_ATTEMPTS);
    return false;
  }
  ++attempt;

  // If it is not the first time, then we wait until the reset delay is expired.
  if (!MMR_DELAY_WaitOnceAsync(&__resetProcDelay))
    return false;

  // After the delay has expired, we check that the mode is CONFIGURATION.
  MmrMcp2515Mode mode;
  if (!MMR_MCP2515LL_ReadMode(&mode)) {
    resetProcedureIsRunning = false;
    return false;
  }
  if (mode == MMR_MCP2515_MODE_CONFIGURATION) {
    // We're done.
    resetProcedureIsRunning = false;
    return true;
  }

  // The MCP2515 did not reset. We should try again.
  if (!MMR_MCP2515LL_OpReset()) {
    resetProcedureIsRunning = false;
    return false;
  }
  MMR_DELAY_Reset(&__resetProcDelay);
  return false;
}

/** @brief Asynchronously switches the operating mode of the MCP2512.
 *        The first call starts the procedure, then this function is supposed to be called in a loop
 *        with a delay until it returns true or an error is returned by MMR_MCP2515_GetLastError
 *        After it returns true, the next call will start a new reset procedure.
 *        Therefore, you MUST run this until it returns true.
 * @param requestedMode The mode which the MCP2515 should switch to.
 * @param max_attempts Akin to a timeout, the maximum amount of function calls to wait if the MCP2515 hasn't changed mode yet before terminating the procedure.
 * @returns true if the mode change has been requested, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 * @exception MMR_MCP2515_ERROR_OUT_OF_ATTEMPTS
 */
bool MMR_MCP2515_SwitchModeAsync(MmrMcp2515Mode requestedMode, int max_attempts) {
  static bool switchModeProcedureIsRunning = false;
  static int attempt = 0;

  if (!switchModeProcedureIsRunning) {
    // First time - request the mode change.
    if (!MMR_MCP2515LL_RequestMode(requestedMode))
      return false;
    
    attempt = 1;
    switchModeProcedureIsRunning = true;
    return false;
  }

  if (attempt > max_attempts) {
    // Out of attempts. The operation fails.
    switchModeProcedureIsRunning = false;
    __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_OUT_OF_ATTEMPTS);
    return false;
  }
  ++attempt;

  // Check that the current mode of the MCP2515 is the requested mode.
  MmrMcp2515Mode mode;
  if (!MMR_MCP2515LL_ReadMode(&mode)) {
    switchModeProcedureIsRunning = false;
    return false;
  }
  if (mode == requestedMode) {
    // We're done.
    switchModeProcedureIsRunning = false;
    return true;
  }

  // The MCP2515 hasn't switched mode yet.
  return false;
}

/** @brief Reads the operating mode of the MCP2515 chip.
 * @param mode A pointer to a location which will contain the operating mode if the function returns true.
 * @returns true if the mode has been read, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515_ReadMode(MmrMcp2515Mode* mode) {
  return MMR_MCP2515LL_ReadMode(mode);
}

/** @brief Configures the MCP2515 CAN BUS timing.
 * @param timing The timing configuration.
 * @returns true if the timing has been configured, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 * @exception MMR_MCP2515_ERROR_NOT_IN_CONFIGURATION_MODE
 */
bool MMR_MCP2515_ConfigureCANTiming(MmrMcp2515Timing timing) {
  bool isInConfigurationMode;

  if (!MMR_MCP2515LL_IsInMode(MMR_MCP2515_MODE_CONFIGURATION, &isInConfigurationMode)) {
    return false;
  }

  if (!isInConfigurationMode){
    __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_NOT_IN_CONFIGURATION_MODE);
    return false;
  }

  return MMR_MCP2515LL_ConfigureCanTiming(timing);
}

