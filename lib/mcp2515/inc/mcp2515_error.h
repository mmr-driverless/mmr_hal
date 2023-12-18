#ifndef MCP2515_ERROR_H
#define MCP2515_ERROR_H

typedef enum {
  /** @brief No error occurred. */
  MMR_MCP2515_ERROR_OK = 0,

  /** @brief An error caused by the underlying SPI library. */
  MMR_MCP2515_ERROR_SPI = 1,

  MMR_MCP2515_ERROR_PAYLOAD_TOO_LONG = 2,
  MMR_MCP2515_ERROR_NOT_IN_CONFIGURATION_MODE = 3,
  MMR_MCP2515_ERROR_NO_FREE_TXBUF = 4,
  MMR_MCP2515_ERROR_NO_PENDING_MESSAGE = 5,
  MMR_MCP2515_ERROR_OUT_OF_ATTEMPTS = 6
} MmrMcp2515Error;

MmrMcp2515Error MMR_MCP2515_GetLastError();

#endif