/**
 * @file ring.h
 * @brief circular buffer library
*/

#ifndef MMR_APP_LIB_RING_INC_RING_H_
#define MMR_APP_LIB_RING_INC_RING_H_

#include <stdint.h>

typedef struct MmrRing {
  uint8_t *buffer;
  uint8_t elementBytes;
  uint8_t elementsNum;
  uint8_t currentPos;
} MmrRing;

MmrRing MMR_RING(uint8_t *buffer, uint8_t elementsNum, uint8_t elementBytes);
void MMR_RING_Insert(MmrRing *self, const uint8_t *ele);
uint8_t* MMR_RING_Get(const MmrRing *self, uint8_t idx);
uint8_t* MMR_RING_GetLast(const MmrRing *self);

#endif  // !MMR_APP_LIB_RING_INC_RING_H_
