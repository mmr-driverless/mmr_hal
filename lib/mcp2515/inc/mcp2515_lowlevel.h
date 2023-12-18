#ifndef MCP2515_LOWLEVEL_H
#define MCP2515_LOWLEVEL_H

#include <stdint.h>
#include <stdbool.h>

#include "spi.h"

#include "mcp2515_error.h"
#include "mcp2515_timing.h"
#include "mcp2515_mode.h"

typedef enum {
  MMR_MCP2515LL_RXB_MASK_BUKT = 0b00000100,
  MMR_MCP2515LL_RXB_MASK_FILMODE = 0b011000000
} MmrMcp2515LLRxbCtrlMask;

typedef enum {
  MMR_MCP2515LL_CANSTAT_READMODE_MASK = 0b11100000
} MmrMcp2515LLCanStatMask;

typedef enum {
  MMR_MCP2515LL_CANCTRL_REQMODE_MASK = 0b11100000
} MmrMcp2515LLCanCtrlMask;

typedef enum {
  MMR_MCP2515LL_STATUS_MASK_RX0_FULL = 1,
  MMR_MCP2515LL_STATUS_MASK_RX1_FULL = 2,
  MMR_MCP2515LL_STATUS_MASK_TX0_PENDINGTX = 4,
  MMR_MCP2515LL_STATUS_MASK_TX1_PENDINGTX = 16,
  MMR_MCP2515LL_STATUS_MASK_TX2_PENDINGTX = 64,
} MmrMcp2515LLStatusMask;

typedef enum {
  MMR_MCP2515LL_TXBUF0 = 0,
  MMR_MCP2515LL_TXBUF1 = 1,
  MMR_MCP2515LL_TXBUF2 = 2
} MmrMcp2515LLTxBuf;

typedef enum {
  MMR_MCP2515LL_RXBUF0 = 0,
  MMR_MCP2515LL_RXBUF1 = 1
} MmrMcp2515LLRxBuf;

typedef enum {
  MMR_MCP2515LL_REG_CANSTAT = 0b00001110,
  MMR_MCP2515LL_REG_CANCTRL = 0b00001111,
  MMR_MCP2515LL_REG_TXB0SIDH = 0b00110001,
  MMR_MCP2515LL_REG_RXB0CTRL = 0b01100000,
  MMR_MCP2515LL_REG_RXB0SIDH = 0b01100001,
  MMR_MCP2515LL_REG_RXB1CTRL = 0b01110000,
  MMR_MCP2515LL_REG_RXF0 = 0b00000000,
  MMR_MCP2515LL_REG_RXF1 = 0b00000100,
  MMR_MCP2515LL_REG_RXF2 = 0b00001000,
  MMR_MCP2515LL_REG_RXF3 = 0b00010000,
  MMR_MCP2515LL_REG_RXF4 = 0b00010100,
  MMR_MCP2515LL_REG_RXF5 = 0b00011000,
  MMR_MCP2515LL_REG_RXM0 = 0b00100000,
  MMR_MCP2515LL_REG_RXM1 = 0b00100100,
  MMR_MCP2515LL_REG_CNF = 0b00101000,
  MMR_MCP2515LL_REG_EFLG = 0b00101101
} MmrMcp2515LLRegister;

typedef enum {
  MMR_MCP2515LL_FILTERID_0 = MMR_MCP2515LL_REG_RXF0,
  MMR_MCP2515LL_FILTERID_1 = MMR_MCP2515LL_REG_RXF1,
  MMR_MCP2515LL_FILTERID_2 = MMR_MCP2515LL_REG_RXF2,
  MMR_MCP2515LL_FILTERID_3 = MMR_MCP2515LL_REG_RXF3,
  MMR_MCP2515LL_FILTERID_4 = MMR_MCP2515LL_REG_RXF4,
  MMR_MCP2515LL_FILTERID_5 = MMR_MCP2515LL_REG_RXF5,
} MmrMcp2515LLFilterId;

typedef enum {
  MMR_MCP2515LL_FILTERMASK_0 = MMR_MCP2515LL_REG_RXM0,
  MMR_MCP2515LL_FILTERMASK_1 = MMR_MCP2515LL_REG_RXM1,
} MmrMcp2515LLFilterMask;


#define CAN_MSG_MAX_LEN 8


void MMR_MCP2515LL_Init(MmrSpi* spi, int slaveIdx);

/**
 * @brief Resets the MCP2515 to the same state it would be if it had just been powered on.
 * @returns true if the operation completed successfully, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpReset();

/**
 * @brief Executes the READ MEMORY instruction.
 * @param buf A pointer to a location which will contain the read data.
 * @param address The register address from which to start reading.
 * @param size How many bytes to read.
 * @returns true if the operation completed successfully, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpReadMemory(uint8_t* buf, uint8_t address, uint8_t size);

/**
 * @brief Executes the WRITE MEMORY instruction.
 * @param buf A pointer to a location containing the data that should be written.
 * @param address The register address from which to start writing.
 * @param size How many bytes to write.
 * @returns true if the operation completed successfully, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpWriteMemory(uint8_t* buf, uint8_t address, uint8_t size);

/**
 * @brief Executes the BIT MODIFY instruction.
 * @param addr The register address.
 * @param mask The mask that specifies which bits are changed.
 * @param data The value to write.
 * @returns true if the operation completed successfully, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpBitModify(uint8_t addr, uint8_t mask, uint8_t data);

/**
 * @brief Writes a message in the specified transmission buffer.
 * @param txBuf The buffer to write into.
 * @param id The CAN ID of the transmitted message.
 * @param isStandardId Whether the CAN ID is standard or extended.
 * @param payload A pointer to a location which contains the payload of the message.
 * @param payloadSize How many bytes to transmit. It should always be less or equal to 8.
 * @returns true if the buffer has been read, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 * @exception MMR_MCP2515_ERROR_PAYLOAD_TOO_LONG
 */
bool MMR_MCP2515LL_OpLoadTxBuf(MmrMcp2515LLTxBuf txBuf, uint32_t id, bool isStandardId, uint8_t* payload, uint8_t payloadSize);

/**
 * @brief Reads the content of a specific reception buffer, and allows new messages to be stored in that buffer.
 * @param rxBuf The buffer to read.
 * @param id A pointer to a location which will contain the CAN ID of the received message.
 * @param isStandardId A pointer to a location which will contain whether the CAN ID is standard or extended.
 * @param payload A pointer to a location in which the payload will be stored - this area of memory must have a size greater or equal to 8 bytes.
 * @param payloadSize A pointer to a location which will contain how many bytes have been received and copied into payload. This value is always less than or equal to 8 bytes.
 * @returns true if the buffer has been read, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpReadRxBuf(MmrMcp2515LLRxBuf rxBuf, uint32_t* id, bool* isStandardId, uint8_t* payload, uint8_t* payloadSize);

/**
 * @brief Requests transmission for a specific transmission buffer.
 * @param txBuf The transmission buffer for which to request transmission.
 * @returns true if the transmission has been requested, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpRts(MmrMcp2515LLTxBuf txBuf);

/**
 * @brief Executes the READ STATUS instruction. 
 * @param status A location which will contain the read status.
 * @returns true if the status has been read, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpReadStatus(uint8_t* status);

/** @brief Reads the operating mode of the MCP2515 chip.
 * @param mode A pointer to a location which will contain the operating mode if the function returns true.
 * @returns true if the mode has been read, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_ReadMode(MmrMcp2515Mode* mode);

/** @brief Checks whether the MCP2515 is in the specified operating mode.
 * @param isInMode A pointer to an area which will contain whether the MCP2515 is in the specified operating mode.
 * @returns true if the operation succeded, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_IsInMode(MmrMcp2515Mode mode, bool* isInMode);

/** @brief Requests the MCP2512 to switch operating mode.
 * @param mode The mode which the MCP2515 should switch to.
 * @returns true if the mode change has been requested, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_RequestMode(MmrMcp2515Mode mode);

/** @brief Configures an MCP2515 filter mask.
 *  @param filter The filter to configure.
 *  @param id_mask The mask to load.
*/
bool MMR_MCP2515LL_ConfigureFilterMask(MmrMcp2515LLFilterMask filter, uint32_t id_mask);

/** @brief Configures an MCP2515 filter id.
 *  @param filter The filter to configure.
 *  @param id_mask The id to load.
*/
bool MMR_MCP2515LL_ConfigureFilterId(MmrMcp2515LLFilterId filter, uint32_t id, bool isStandardId);

/** @brief Configures the MCP2515 CAN BUS timing.
 * @param timing The timing configuration.
 * @returns true if the timing has been configured, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_ConfigureCanTiming(MmrMcp2515Timing timing);

#endif