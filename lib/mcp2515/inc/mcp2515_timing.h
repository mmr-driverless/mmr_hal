#ifndef MCP2515_TIMING_H
#define MCP2515_TIMING_H

typedef enum {
  // copied from https://github.com/autowp/arduino-mcp2515/blob/master/mcp2515.h
  // took out the macros and ran regex search/replace
  // find: #define MCP_(.*)_(.*)_CFG. \((0x.+)\)\n.*\(0x(.+)\)\n.*\(0x(.+)\)\s?$
  // replace: MMR_MCP2515_TIMING_$1_$2 = $3$4$5,
  
  // (MSB) CFG1 CFG2 CFG3 (LSB)

  // 8MHz
  MMR_MCP2515_TIMING_8MHz_1000kBPS = 0x008080,
  MMR_MCP2515_TIMING_8MHz_500kBPS = 0x009082,
  MMR_MCP2515_TIMING_8MHz_250kBPS = 0x00B185,
  MMR_MCP2515_TIMING_8MHz_200kBPS = 0x00B486,
  MMR_MCP2515_TIMING_8MHz_125kBPS = 0x01B185,
  MMR_MCP2515_TIMING_8MHz_100kBPS = 0x01B486,
  MMR_MCP2515_TIMING_8MHz_80kBPS = 0x01BF87,
  MMR_MCP2515_TIMING_8MHz_50kBPS = 0x03B486,
  MMR_MCP2515_TIMING_8MHz_40kBPS = 0x03BF87,
  MMR_MCP2515_TIMING_8MHz_33k3BPS = 0x47E285,
  MMR_MCP2515_TIMING_8MHz_31k25BPS = 0x07A484,
  MMR_MCP2515_TIMING_8MHz_20kBPS = 0x07BF87,
  MMR_MCP2515_TIMING_8MHz_10kBPS = 0x0FBF87,
  MMR_MCP2515_TIMING_8MHz_5kBPS = 0x1FBF87,

  // 16MHz
  MMR_MCP2515_TIMING_16MHz_1000kBPS = 0x00D082,
  MMR_MCP2515_TIMING_16MHz_500kBPS = 0x00F086,
  MMR_MCP2515_TIMING_16MHz_250kBPS = 0x41F185,
  MMR_MCP2515_TIMING_16MHz_200kBPS = 0x01FA87,
  MMR_MCP2515_TIMING_16MHz_125kBPS = 0x03F086,
  MMR_MCP2515_TIMING_16MHz_100kBPS = 0x03FA87,
  MMR_MCP2515_TIMING_16MHz_80kBPS = 0x03FF87,
  MMR_MCP2515_TIMING_16MHz_83k3BPS = 0x03BE07,
  MMR_MCP2515_TIMING_16MHz_50kBPS = 0x07FA87,
  MMR_MCP2515_TIMING_16MHz_40kBPS = 0x07FF87,
  MMR_MCP2515_TIMING_16MHz_33k3BPS = 0x4EF185,
  MMR_MCP2515_TIMING_16MHz_20kBPS = 0x0FFF87,
  MMR_MCP2515_TIMING_16MHz_10kBPS = 0x1FFF87,
  MMR_MCP2515_TIMING_16MHz_5kBPS = 0x3FFF87,

  // 20MHz
  MMR_MCP2515_TIMING_20MHz_1000kBPS = 0x00D982,
  MMR_MCP2515_TIMING_20MHz_500kBPS = 0x00FA87,
  MMR_MCP2515_TIMING_20MHz_250kBPS = 0x41FB86,
  MMR_MCP2515_TIMING_20MHz_200kBPS = 0x01FF87,
  MMR_MCP2515_TIMING_20MHz_125kBPS = 0x03FA87,
  MMR_MCP2515_TIMING_20MHz_100kBPS = 0x04FA87,
  MMR_MCP2515_TIMING_20MHz_83k3BPS = 0x04FE87,
  MMR_MCP2515_TIMING_20MHz_80kBPS = 0x04FF87,
  MMR_MCP2515_TIMING_20MHz_50kBPS = 0x09FA87,
  MMR_MCP2515_TIMING_20MHz_40kBPS = 0x09FF87,
  MMR_MCP2515_TIMING_20MHz_33k3BPS = 0x0BFF87,
} MmrMcp2515Timing;

#endif