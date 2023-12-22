#include "IfxMultican_Can.h"
#include "lib/can/inc/message.h"
#include "lib/pin/inc/aurix_pin.h"
// #include "lib/can/inc/can.h"
#include <stdint.h>

#define ISR_PRIORITY_CAN_TX 1
#define ISR_PRIORITY_CAN_RX 2

#define TX_INTERRUPT_SRC_ID 0
#define RX_INTERRUPT_SRC_ID 1
typedef struct
{
    IfxMultican_Can                 can;                   /* CAN module handle to HW module SFR set                 */
    IfxMultican_Can_Config          canConfig;             /* CAN module configuration structure                     */
    IfxMultican_Can_Node            canSrcNode;            /* CAN source node handle data structure                  */
    IfxMultican_Can_Node            canDstNode;            /* CAN destination node handle data structure             */
    IfxMultican_Can_NodeConfig      canNodeConfig;         /* CAN node configuration structure                       */
    IfxMultican_Can_MsgObj          canSrcMsgObj;          /* CAN source message object handle data structure        */
    IfxMultican_Can_MsgObj          canDstMsgObj;          /* CAN destination message object handle data structure   */
    IfxMultican_Can_MsgObjConfig    canMsgObjConfig;       /* CAN message object configuration structure             */
    IfxMultican_Message             txMsg;                 /* Transmitted CAN message structure                      */
    IfxMultican_Message             rxMsg;                 /* Received CAN message structure                         */
} MulticanManager;

typedef struct
{
    IfxPort_Pin_Config              led1;                  /* LED1 configuration structure                           */
    IfxPort_Pin_Config              led2;                  /* LED2 configuration structure                           */
} AppLedType;


typedef struct 
{
    uint32_t low_bit;
    uint32_t high_bit;
} SplittedPayload;


//splitta il payload in high e low
SplittedPayload MMR_split_payload(uint8_t* payload, uint8_t length);

//Inizializza il modulo multican e la struttura che lo gestisca
void MMR_init_multican( MmrPin pin_tx, MmrPin pin_rx );

void MMR_transmit_message(MmrCanMessage *message);

