#include "buffer.h"

static inline MmrEncoding getArchEndianness();
static inline void writeBytes(uint8_t *outBuffer, const uint8_t *inVal, unsigned short inSize, unsigned short offset, MmrEncoding targetEncoding);
static inline void swapBytes(uint8_t *outBuffer, const uint8_t *inVal, unsigned short inSize, unsigned short offset);
static inline void copyBytes(uint8_t *outBuffer, const uint8_t *inVal, unsigned short inSize, unsigned short offset);

void MMR_BUFFER_WriteBool(uint8_t *buffer, bool value, uint8_t offset) { buffer[offset] = value; }
void MMR_BUFFER_WriteByte(uint8_t *buffer, uint8_t value, uint8_t offset) { buffer[offset] = value; }
void MMR_BUFFER_WriteInt16(uint8_t *buffer, int16_t value, uint8_t offset, MmrEncoding targetEncoding) { writeBytes(buffer, (uint8_t*)&value, 2, offset, targetEncoding); }
void MMR_BUFFER_WriteUInt16(uint8_t *buffer, uint16_t value, uint8_t offset, MmrEncoding targetEncoding) { writeBytes(buffer, (uint8_t*)&value, 2, offset, targetEncoding); }
void MMR_BUFFER_WriteInt32(uint8_t *buffer, int32_t value, uint8_t offset, MmrEncoding targetEncoding) { writeBytes(buffer, (uint8_t*)&value, 4, offset, targetEncoding); }
void MMR_BUFFER_WriteUInt32(uint8_t *buffer, uint32_t value, uint8_t offset, MmrEncoding targetEncoding) { writeBytes(buffer, (uint8_t*)&value, 4, offset, targetEncoding); }
void MMR_BUFFER_WriteInt64(uint8_t *buffer, uint64_t value, uint8_t offset, MmrEncoding targetEncoding) { writeBytes(buffer, (uint8_t*)&value, 8, offset, targetEncoding); }
void MMR_BUFFER_WriteUint64(uint8_t *buffer, uint64_t value, uint8_t offset, MmrEncoding targetEncoding) { writeBytes(buffer, (uint8_t*)&value, 8, offset, targetEncoding); }
void MMR_BUFFER_WriteFloat(uint8_t *buffer, float value, uint8_t offset, MmrEncoding targetEncoding) { writeBytes(buffer, (uint8_t*)&value, 4, offset, targetEncoding); }


static inline MmrEncoding getArchEndianness() {
  static const uint16_t rawMem = 0x0001U;
  return *((uint8_t*) &rawMem) ? MMR_ENCODING_LITTLE_ENDIAN : MMR_ENCODING_BIG_ENDIAN;
}

static inline void writeBytes(uint8_t *outBuffer, const uint8_t *inVal, unsigned short inSize, unsigned short offset, MmrEncoding targetEncoding) {
  if (getArchEndianness() == targetEncoding) {
    copyBytes(outBuffer, inVal, inSize, offset);
  }
  else {
    swapBytes(outBuffer, inVal, inSize, offset);
  }
}

static inline void swapBytes(uint8_t *outBuffer, const uint8_t *inVal, unsigned short inSize, unsigned short offset) {
  for (int idx=0; idx<inSize; idx++) {
    outBuffer[offset+inSize-idx-1] = inVal[idx];
  }
}

static inline void copyBytes(uint8_t *outBuffer, const uint8_t *inVal, unsigned short inSize, unsigned short offset) {
  for (int idx=0; idx<inSize; idx++) {
    outBuffer[offset+idx] = inVal[idx];
  }
}
