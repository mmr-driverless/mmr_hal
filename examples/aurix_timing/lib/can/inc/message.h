#ifndef CAN_INC_MESSAGE_H_
#define CAN_INC_MESSAGE_H_

#include "message_id.h"
#include "header.h"

#include <stdbool.h>

#define MMR_CAN_MAX_DATA_LENGTH 8

typedef uint8_t MmrCanBuffer[MMR_CAN_MAX_DATA_LENGTH];

typedef struct MmrCanMessage {
  uint32_t id;
  bool isStandardId;
  uint8_t *payload;
  uint8_t length;
} MmrCanMessage;


MmrCanMessage MMR_CAN_OutMessage(MmrCanHeader header);
MmrCanMessage MMR_CAN_OutStdMessage(MmrCanHeader header);

void MMR_CAN_MESSAGE_SetId(MmrCanMessage *message, uint32_t id);
uint32_t MMR_CAN_MESSAGE_GetId(MmrCanMessage *message);
void MMR_CAN_MESSAGE_SetStandardId(MmrCanMessage *message, bool isStandardId);
bool MMR_CAN_MESSAGE_IsStandardId(MmrCanMessage *message);
void MMR_CAN_MESSAGE_SetHeader(MmrCanMessage *message, MmrCanHeader header);
MmrCanHeader MMR_CAN_MESSAGE_GetHeader(MmrCanMessage *message);
void MMR_CAN_MESSAGE_SetPayload(MmrCanMessage *message, uint8_t *payload, uint8_t length);
uint8_t* MMR_CAN_MESSAGE_GetPayload(MmrCanMessage *message, uint8_t *length);

#endif /* CAN_INC_MESSAGE_H_ */
