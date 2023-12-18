#include "inc/can0.h"

#define MMR_CAN0_MAILBOXES_COUNT 3

static CAN_HandleTypeDef *__hcan0;


bool MMR_CAN0_Start(CAN_HandleTypeDef *hcan0) {
  __hcan0 = hcan0;

  MmrCanFilter noFilter = {};
  if (!MMR_CAN_SetFilter(&can0, &noFilter))
    return false;

  return HAL_CAN_Start(__hcan0) == HAL_OK;
}


static uint32_t __mmr_can0_mailboxes[MMR_CAN0_MAILBOXES_COUNT] = {};
static uint8_t __mmr_can0_currentMailbox = 0;

static uint32_t *__mmr_can0_getNextMailbox() {
  __mmr_can0_currentMailbox++;
  __mmr_can0_currentMailbox %= MMR_CAN0_MAILBOXES_COUNT;

  return __mmr_can0_mailboxes + __mmr_can0_currentMailbox;
}


static bool __mmr_can0_trySetFilter(MmrCanFilter *filter) {
  CAN_FilterTypeDef f = {
    .FilterActivation = CAN_FILTER_ENABLE,
    .FilterFIFOAssignment = CAN_FILTER_FIFO0,
    .FilterBank = 0,
    .FilterIdHigh = filter->id,
    .FilterIdLow = filter->id,
    .FilterMaskIdHigh = filter->mask,
    .FilterMaskIdLow = filter->mask,
    .SlaveStartFilterBank = 10,
    .FilterMode = CAN_FILTERMODE_IDMASK,
    .FilterScale = CAN_FILTERSCALE_32BIT,
  };

  return HAL_CAN_ConfigFilter(__hcan0, &f) == HAL_OK;
}

static bool __mmr_can0_send(const MmrCanMessage *message) {
  CAN_TxHeaderTypeDef tx = {
    .IDE = message->isStandardId ? CAN_ID_STD : CAN_ID_EXT,
    .RTR = CAN_RTR_DATA,
    .DLC = message->length,
    .TransmitGlobalTime = DISABLE,
  };

  if (message->isStandardId) {
    tx.StdId = message->id;
  }
  else {
    tx.ExtId = message->id;
  }

  HAL_StatusTypeDef status =
    HAL_CAN_AddTxMessage(__hcan0, &tx, message->payload, __mmr_can0_getNextMailbox());

  return status == HAL_OK;
}


static bool __mmr_can0_receive(MmrCanMessage *message) {
  CAN_RxHeaderTypeDef rx = {};
  HAL_StatusTypeDef status =
    HAL_CAN_GetRxMessage(__hcan0, CAN_RX_FIFO0, &rx, message->payload);

  message->length = rx.DLC;
  if (rx.IDE == CAN_ID_STD) {
    MMR_CAN_MESSAGE_SetId(message, rx.StdId);
  }
  else {
    MMR_CAN_MESSAGE_SetId(message, rx.ExtId);
  }

  return status == HAL_OK;
}


static uint8_t __mmr_can0_pendingMessages() {
  return HAL_CAN_GetRxFifoFillLevel(__hcan0, CAN_RX_FIFO0);
}



MmrCan can0 = {
  .__trySend = __mmr_can0_send,
  .__tryReceive = __mmr_can0_receive,
  .__trySetFilter = __mmr_can0_trySetFilter,
  .__getPendingMessages = __mmr_can0_pendingMessages,
};
