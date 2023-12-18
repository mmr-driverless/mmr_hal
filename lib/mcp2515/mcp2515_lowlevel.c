#include "__inc/__mcp2515_errmgmt.h"
#include "inc/mcp2515_lowlevel.h"
#include <stddef.h>

static MmrSpi* __spi = NULL;
static int __slaveIdx = -1;

void MMR_MCP2515LL_Init(MmrSpi* spi, int slaveIdx) {
  __spi = spi;
  __slaveIdx = slaveIdx;
}

typedef enum {
  MMR_MCP2515LL_OP_RESET = 0b11000000,
  MMR_MCP2515LL_OP_READ_STATUS = 0b10100000,
  MMR_MCP2515LL_OP_READ = 0b00000011,
  MMR_MCP2515LL_OP_BIT_MODIFY = 0b00000101,
  MMR_MCP2515LL_OP_WRITE = 0b00000010,
  MMR_MCP2515LL_OP_RTS_TXB0 = 0b10000001,
  MMR_MCP2515LL_OP_LOAD_TX_BUF0 = 0b01000000,
  MMR_MCP2515LL_OP_READ_RX_BUF0 = 0b10010000
} __mmr_mcp2515ll_op_t;


/* The implementations of these helpers are at the end of the file. */
#define FILTER_LEN 4
#define TXRXBUF_HEADER_LEN 5
static inline void __mmr_mcp2515ll_make_txbuf_header(uint32_t id, bool isStandardId, bool rtr, uint8_t dataLen, uint8_t* buf);
static inline void __mmr_mcp2515ll_parse_rxbuf_header(uint8_t* buf, uint32_t* id, bool* isStandardId, bool* rtr, uint8_t* dataLen);
static inline void __mmr_mcp2515ll_make_filter(uint32_t id, bool isStandardId, uint8_t* buf);
static inline bool __mmr_mcp2515ll_tx(uint8_t* txData, uint8_t txSize);
static inline bool __mmr_mcp2515ll_tx2(uint8_t* txData1, uint8_t txSize1, uint8_t* txData2, uint8_t txSize2);
static inline bool __mmr_mcp2515ll_txrx(uint8_t* txData, uint8_t txSize, uint8_t* rxData, uint8_t rxSize);
static inline bool __mmr_mcp2515ll_txrx2(uint8_t* txData, uint8_t txSize, uint8_t* rxData1, uint8_t rxSize1, uint8_t* rxData2, uint8_t rxSize2);

/* 

MCP2515 SPI commands

*/

/**
 * @brief Resets the MCP2515 to the same state it would be if it had just been powered on.
 * @returns true if the operation completed successfully, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpReset() {
  uint8_t op = MMR_MCP2515LL_OP_RESET;
  return __mmr_mcp2515ll_tx(&op, 1);
}

/**
 * @brief Executes the READ MEMORY instruction.
 * @param buf A pointer to a location which will contain the read data.
 * @param address The register address from which to start reading.
 * @param size How many bytes to read.
 * @returns true if the operation completed successfully, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpReadMemory(uint8_t* buf, uint8_t address, uint8_t size) {
  uint8_t head[] = { MMR_MCP2515LL_OP_READ, address };
  return __mmr_mcp2515ll_txrx(head, sizeof(head), buf, size);
}

/**
 * @brief Executes the WRITE MEMORY instruction.
 * @param buf A pointer to a location containing the data that should be written.
 * @param address The register address from which to start writing.
 * @param size How many bytes to write.
 * @returns true if the operation completed successfully, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpWriteMemory(uint8_t* buf, uint8_t address, uint8_t size) {
  uint8_t head[] = { MMR_MCP2515LL_OP_WRITE, address };
  return __mmr_mcp2515ll_tx2(head, sizeof(head), buf, size);
}

/**
 * @brief Executes the BIT MODIFY instruction.
 * @param addr The register address.
 * @param mask The mask that specifies which bits are changed.
 * @param data The value to write.
 * @returns true if the operation completed successfully, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpBitModify(uint8_t addr, uint8_t mask, uint8_t data) {
  uint8_t msg[] = { MMR_MCP2515LL_OP_BIT_MODIFY, addr, mask, data };
  return __mmr_mcp2515ll_tx(msg, sizeof(msg));
}

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
bool MMR_MCP2515LL_OpLoadTxBuf(MmrMcp2515LLTxBuf txBuf, uint32_t id, bool isStandardId, uint8_t* payload, uint8_t payloadSize) {
  __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_OK);
  if (payloadSize > CAN_MSG_MAX_LEN) {
    __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_PAYLOAD_TOO_LONG);
    return false;
  }

  uint8_t msgHeader[1 + TXRXBUF_HEADER_LEN] = {
    [0] = MMR_MCP2515LL_OP_LOAD_TX_BUF0 | (txBuf << 1), // opcode, leave LSB as 0 to write starting from TXBxSIDH
  };

  __mmr_mcp2515ll_make_txbuf_header(id, isStandardId, false, payloadSize, msgHeader + 1);

  return __mmr_mcp2515ll_tx2(msgHeader, sizeof(msgHeader), payload, payloadSize);
}

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
bool MMR_MCP2515LL_OpReadRxBuf(MmrMcp2515LLRxBuf rxBuf, uint32_t* id, bool* isStandardId, uint8_t* payload, uint8_t* payloadSize) {
  uint8_t opcode = MMR_MCP2515LL_OP_READ_RX_BUF0 | (rxBuf << 2); // opcode - LSB bit is 0, second LSB is 0 to write sid/eid/dlc too
  uint8_t rxBufferHeader[TXRXBUF_HEADER_LEN];

  if (!__mmr_mcp2515ll_txrx2(&opcode, 1, rxBufferHeader, TXRXBUF_HEADER_LEN, payload, CAN_MSG_MAX_LEN))
    return false;

  bool rtr;
  __mmr_mcp2515ll_parse_rxbuf_header(rxBufferHeader, id, isStandardId, &rtr, payloadSize);
  return true;
}

/**
 * @brief Requests transmission for a specific transmission buffer.
 * @param txBuf The transmission buffer for which to request transmission.
 * @returns true if the transmission has been requested, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpRts(MmrMcp2515LLTxBuf txBuf) {
  uint8_t msg[] = { MMR_MCP2515LL_OP_RTS_TXB0 | (1 << txBuf) };
  return __mmr_mcp2515ll_tx(msg, sizeof(msg));
}

/**
 * @brief Executes the READ STATUS instruction. 
 * @param status A location which will contain the read status.
 * @returns true if the status has been read, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_OpReadStatus(uint8_t* status) {
  uint8_t op = MMR_MCP2515LL_OP_READ_STATUS;
  return __mmr_mcp2515ll_txrx(&op, 1, status, 1);
}



/*

  MCP2515 operations derived from SPI commands

*/

/** @brief Reads the operating mode of the MCP2515 chip.
 * @param mode A pointer to a location which will contain the operating mode if the function returns true.
 * @returns true if the mode has been read, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_ReadMode(MmrMcp2515Mode* mode) {
  uint8_t canctrl;

  if (!MMR_MCP2515LL_OpReadMemory(&canctrl, MMR_MCP2515LL_REG_CANSTAT, 1))
    return false;

  *mode = (MmrMcp2515Mode)(canctrl & MMR_MCP2515LL_CANSTAT_READMODE_MASK);
  return true;
}


/** @brief Checks whether the MCP2515 is in the specified operating mode.
 * @param isInMode A pointer to an area which will contain whether the MCP2515 is in the specified operating mode.
 * @returns true if the operation succeded, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_IsInMode(MmrMcp2515Mode mode, bool* isInMode) {
  MmrMcp2515Mode currentMode;
  bool ok = MMR_MCP2515LL_ReadMode(&currentMode);
  *isInMode = currentMode == mode;
  return ok;
}

/** @brief Requests the MCP2512 to switch operating mode.
 * @param mode The mode which the MCP2515 should switch to.
 * @returns true if the mode change has been requested, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_RequestMode(MmrMcp2515Mode mode) {
  return MMR_MCP2515LL_OpBitModify(MMR_MCP2515LL_REG_CANCTRL, MMR_MCP2515LL_CANCTRL_REQMODE_MASK, mode);
}

/** @brief Configures an MCP2515 filter id.
 *  @param filter The filter to configure.
 *  @param id_mask The id to load.
*/
bool MMR_MCP2515LL_ConfigureFilterId(MmrMcp2515LLFilterId filter, uint32_t id, bool isStandardId) {
  uint8_t filterId[FILTER_LEN];
  __mmr_mcp2515ll_make_filter(id, isStandardId, filterId);
  return MMR_MCP2515LL_OpWriteMemory(filterId, filter, FILTER_LEN);
}

/** @brief Configures an MCP2515 filter mask.
 *  @param filter The filter to configure.
 *  @param id_mask The mask to load.
*/
bool MMR_MCP2515LL_ConfigureFilterMask(MmrMcp2515LLFilterMask filter, uint32_t id_mask) {
  uint8_t filterMask[FILTER_LEN];
  __mmr_mcp2515ll_make_filter(id_mask, true, filterMask);
  return MMR_MCP2515LL_OpWriteMemory(filterMask, filter, FILTER_LEN);
}

/** @brief Configures the MCP2515 CAN BUS timing.
 * @param timing The timing configuration.
 * @returns true if the timing has been configured, false otherwise.
 * @exception MMR_MCP2515_ERROR_SPI
 */
bool MMR_MCP2515LL_ConfigureCanTiming(MmrMcp2515Timing timing) {
  uint8_t cfg[3] = {
    timing & 0xFF, // CF3
    (timing >> 8) & 0xFF, // CF2
    (timing >> 16) & 0xFF, // CF1
  };

  return MMR_MCP2515LL_OpWriteMemory(cfg, MMR_MCP2515LL_REG_CNF, sizeof(cfg));
}


/* 

SPI command abstractions

*/

#define __MMR_MCP2515LL_USE_SLAVE(action) \
  do { \
    if (MMR_SPI_SelectSlave(__spi, __slaveIdx)) { \
      __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_OK); \
    } \
    else { \
      __mmr_mcp2515_set_error(MMR_MCP2515_ERROR_SPI); \
      return false; \
    } \
    \
    __mmr_mcp2515_set_error((action) \
      ? MMR_MCP2515_ERROR_OK \
      : MMR_MCP2515_ERROR_SPI); \
    \
    MMR_SPI_DeselectSlave(__spi); \
    return MMR_MCP2515_GetLastError() == MMR_MCP2515_ERROR_OK; \
  } \
  while(0)


static inline bool __mmr_mcp2515ll_tx(uint8_t* txData, uint8_t txSize) {
  __MMR_MCP2515LL_USE_SLAVE(
    MMR_SPI_Transmit(__spi, txData, txSize)
  );
}

static inline bool __mmr_mcp2515ll_tx2(uint8_t* txData1, uint8_t txSize1, uint8_t* txData2, uint8_t txSize2) {
  __MMR_MCP2515LL_USE_SLAVE(
    MMR_SPI_Transmit(__spi, txData1, txSize1) &&
    MMR_SPI_Transmit(__spi, txData2, txSize2)
  );
}

static inline bool __mmr_mcp2515ll_txrx(uint8_t* txData, uint8_t txSize, uint8_t* rxData, uint8_t rxSize) {
  __MMR_MCP2515LL_USE_SLAVE(
    MMR_SPI_Transmit(__spi, txData, txSize) &&
    MMR_SPI_Receive(__spi, rxData, rxSize)
  );
}

static inline bool __mmr_mcp2515ll_txrx2(uint8_t* txData, uint8_t txSize, uint8_t* rxData1, uint8_t rxSize1, uint8_t* rxData2, uint8_t rxSize2) {
  __MMR_MCP2515LL_USE_SLAVE(
    MMR_SPI_Transmit(__spi, txData, txSize) &&
    MMR_SPI_Receive(__spi, rxData1, rxSize1) &&
    MMR_SPI_Receive(__spi, rxData2, rxSize2)
  );
}


/*

PACKING / UNPACKING HELPERS

*/

#define STD_ID_MASK 0x7FF
#define EXT_ID_MASK 0x3FFFF
#define EXT_ID_LENGTH 18

static inline void __split_unified_can_id(uint32_t id, uint32_t* sid, uint32_t* eid) {
  *sid = (id >> EXT_ID_LENGTH) & STD_ID_MASK;
  *eid = id & EXT_ID_MASK;
}

static inline void __make_unified_can_id(uint32_t sid, uint32_t eid, uint32_t* id) {
  *id = eid & EXT_ID_MASK;
  *id |= (sid & STD_ID_MASK) << EXT_ID_LENGTH;
}

static inline void __mmr_mcp2515ll_make_txbuf_header(uint32_t id, bool isStandardId, bool rtr, uint8_t dataLen, uint8_t* buf) {
  /*        most significant bit       ->      least significant bit
  buf[0] = { SID10, SID9,  SID8,  SID7,  SID6,  SID5,  SID4,  SID3  }
  buf[1] = { SID2,  SID1,  SID0,  --,    EXIDE, --,    EID17, EID16 }
  buf[2] = { EID15, EID14, EID13, EID12, EID11, EID10, EID9,  EID8  }
  buf[3] = { EID7,  EID6,  EID5,  EID4,  EID3,  EID2,  EID1,  EID0  }
  buf[4] = { --,    RTR,   --,    --,    DLC3,  DLC2,  DLC1,  DLC0  }
  */
  
  uint32_t sid, eid;
  if (isStandardId)
  {
    sid = id & STD_ID_MASK;
    eid = 0; // ignored
  } else
    __split_unified_can_id(id, &sid, &eid);
  
  buf[0] = sid >> 3;
  buf[1] = (sid << 5) | ((!isStandardId) << 3) | ((eid >> 16) & 3);
  buf[2] = eid >> 8;
  buf[3] = eid;
  buf[4] = (rtr << 6) | (dataLen & 0xF);
}

static inline void __mmr_mcp2515ll_parse_rxbuf_header(uint8_t* buf, uint32_t* id, bool* isStandardId, bool* rtr, uint8_t* dataLen) {
  /*        most significant bit       ->      least significant bit
  buf[0] = { SID10, SID9,  SID8,  SID7,  SID6,  SID5,  SID4,  SID3  }
  buf[1] = { SID2,  SID1,  SID0,  SRR,   IDE,   --,    EID17, EID16 }
  buf[2] = { EID15, EID14, EID13, EID12, EID11, EID10, EID9,  EID8  }
  buf[3] = { EID7,  EID6,  EID5,  EID4,  EID3,  EID2,  EID1,  EID0  }
  buf[4] = { --,    RTR,   --,    --,    DLC3,  DLC2,  DLC1,  DLC0  }
  */

  uint32_t sid = (buf[0] << 3) | (buf[1] >> 5);
  *isStandardId = ((~buf[1]) >> 3) & 1;

  if (*isStandardId) {
    *id = sid & STD_ID_MASK;
    *rtr = (buf[1] >> 4) & 1;
  } else {
    uint32_t eid = ((buf[1] & 0x3) << 16) | (buf[2] << 8) | buf[3];
    __make_unified_can_id(sid, eid, id);
    *rtr = (buf[4] >> 6) & 1;
  }

  *dataLen = (buf[4] & 0xF);
}

static inline void __mmr_mcp2515ll_make_filter(uint32_t id, bool isStandardId, uint8_t* buf) {
  // Same structure for both the filter mask and filter id.
  // The EXIDE field is ignored for masks.
  // When using standard IDs, the  the MCP2515 uses EID[15:0] as data filters for the first two payload bytes.
  
  /*        most significant bit       ->      least significant bit
  buf[0] = { SID10, SID9,  SID8,  SID7,  SID6,  SID5,  SID4,  SID3  }
  buf[1] = { SID2,  SID1,  SID0,  --,    EXIDE, --,    EID17, EID16 }
  buf[2] = { EID15, EID14, EID13, EID12, EID11, EID10, EID9,  EID8  }
  buf[3] = { EID7,  EID6,  EID5,  EID4,  EID3,  EID2,  EID1,  EID0  }
  */

  uint32_t sid, eid;
  if (isStandardId)
  {
    sid = id & STD_ID_MASK;
    eid = 0; // Set the EID to 0, so that data filtering is not applied (because the mask is 0)
  } else
    __split_unified_can_id(id, &sid, &eid);
  
  buf[0] = sid >> 3;
  buf[1] = (sid << 5) | ((!isStandardId) << 3) | ((eid >> 16) & 3);
  buf[2] = eid >> 8;
  buf[3] = eid;
}