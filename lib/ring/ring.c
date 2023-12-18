#include "inc/ring.h"

#include <string.h>

MmrRing MMR_RING(uint8_t *buffer, uint8_t elementsNum, uint8_t elementBytes) {
  return (MmrRing) {
    .buffer = buffer,
    .elementBytes = elementBytes,
    .elementsNum = elementsNum,
    .currentPos = 0
  };
}

void MMR_RING_Insert(MmrRing *self, const uint8_t *ele) {
  memcpy(self->buffer + self->currentPos * self->elementBytes, ele, self->elementBytes);
  self->currentPos = (self->currentPos + 1) % self->elementsNum;
}

uint8_t* MMR_RING_Get(const MmrRing *self, uint8_t idx) {
  return self->buffer + idx * self->elementBytes;
}

uint8_t* MMR_RING_GetLast(const MmrRing *self) {
  return MMR_RING_Get(self, (self->currentPos - 1 + self->elementsNum) % (self->elementsNum));
}