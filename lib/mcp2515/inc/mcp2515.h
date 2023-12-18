#ifndef INC_MMR_MCP2515_H_
#define INC_MMR_MCP2515_H_

#include <stdint.h>
#include <stdbool.h>

#include "spi.h"
#include "can.h"
#include "delay.h"

#include "mcp2515_mode.h"
#include "mcp2515_error.h"
#include "mcp2515_timing.h"

extern MmrCan mcp2515;

/** @brief Initializes the internal state of the MCP2515 library, without performing any action on the actual chip. */
void MMR_MCP2515_Init(MmrSpi* spi, int slaveIdx);

/** @brief Configures the MCP2515 CAN BUS timing.
 * @param timing The timing configuration.
 * @returns true if the timing has been configured, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515_ConfigureCANTiming(MmrMcp2515Timing timing);

/** @brief
 * Asynchronously switches the operating mode of the MCP2512.
 * The first call starts the procedure, then this function is supposed to be called in a loop
 * with a delay until it returns true or an error is returned by MMR_MCP2515_GetLastError
 * After it returns true, the next call will start a new reset procedure.
 * Therefore, you MUST run this until it returns true.
 *
 * @param requestedMode The mode which the MCP2515 should switch to.
 * @param max_attempts Akin to a timeout, the maximum amount of function calls to wait if the MCP2515 hasn't changed mode yet before terminating the procedure.
 * @returns true if the mode change has been requested, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 * @exception MMR_MCP2515_ERROR_OUT_OF_ATTEMPTS
 */
bool MMR_MCP2515_SwitchModeAsync(MmrMcp2515Mode requestedMode, int max_attempts);

/** @brief
 * Reads the operating mode of the MCP2515 chip.
 * 
 * @param mode A pointer to a location which will contain the operating mode if the function returns true.
 * @returns true if the mode has been read, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515_ReadMode(MmrMcp2515Mode* mode);

/**
 * @brief
 * Asynchronously resets the MCP2515 to the same state it would be if it had just been powered on.
 * The first call starts the procedure, then this function is supposed to be called in a loop
 * until it returns true or an error is returned by MMR_MCP2515_GetLastError
 * After it returns true, the next call will start a new reset procedure.
 * Therefore, you MUST run this until it returns true.
 * 
 * @param max_attempts Akin to a timeout, the maximum amount of reset attempts to perform if the MCP2515 hasn't reset yet before terminating the procedure.
 * @returns true if the operation completed successfully, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 * @exception MMR_MCP2515_ERROR_OUT_OF_ATTEMPTS
 */
bool MMR_MCP2515_ResetAsync(int max_attempts);

#endif
