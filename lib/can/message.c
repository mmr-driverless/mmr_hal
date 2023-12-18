#include "inc/message.h"
#include <stdint.h>
#include <stdbool.h>

uint32_t MMR_CAN_MESSAGE_GetId(MmrCanMessage *message) {
  return message->id;
}

void MMR_CAN_MESSAGE_SetId(MmrCanMessage *message, uint32_t id) {
  message->id = id;
}

void MMR_CAN_MESSAGE_SetStandardId(MmrCanMessage *message, bool isStandardId) {
  message->isStandardId = isStandardId;
}

bool MMR_CAN_MESSAGE_IsStandardId(MmrCanMessage *message) {
  return message->isStandardId;
}

void MMR_CAN_MESSAGE_SetPayload(MmrCanMessage *message, uint8_t *payload, uint8_t length) {
  message->payload = payload;
  message->length = length;
}

uint8_t* MMR_CAN_MESSAGE_GetPayload(MmrCanMessage *message, uint8_t *length) {
  *length = message->length;
  return message->payload;
}
