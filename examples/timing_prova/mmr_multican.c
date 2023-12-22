/*
 * mmr_multican.c
 *
 *  Created on: 13 dic 2023
 *      Author: crist
 */


#include "mmr_multican.h"


MulticanManager multican;
AppLedType      g_led;
MmrPin pin_tx;
MmrPin pin_rx;


IFX_INTERRUPT(trasmesso_fun, 0, ISR_PRIORITY_CAN_TX);
IFX_INTERRUPT(ricevuto_fun, 0, ISR_PRIORITY_CAN_RX);



void MMR_init_multican(  MmrPin pin_t, MmrPin pin_r ){
    //Inizializzazione modulo

    pin_tx = pin_t;
    pin_rx = pin_r;

    IfxMultican_Can_initModuleConfig(&multican.canConfig, &MODULE_CAN);

    multican.canConfig.nodePointer[0].priority = ISR_PRIORITY_CAN_TX;
    multican.canConfig.nodePointer[1].priority = ISR_PRIORITY_CAN_RX;

    IfxMultican_Can_initModule(&multican.can, &multican.canConfig);

    //Inizializza il nodo sorgente
    IfxMultican_Can_Node_initConfig(&multican.canNodeConfig, &multican.can);
    multican.canNodeConfig.nodeId = IfxMultican_NodeId_0;
    multican.canNodeConfig.loopBackMode = TRUE;
    IfxMultican_Can_Node_init(&multican.canSrcNode, &multican.canNodeConfig);

    //Inizializza nodo destinatario
    IfxMultican_Can_Node_initConfig(&multican.canNodeConfig, &multican.can);
    multican.canNodeConfig.nodeId = IfxMultican_NodeId_1;
    multican.canNodeConfig.loopBackMode = TRUE;
    IfxMultican_Can_Node_init(&multican.canDstNode, &multican.canNodeConfig);

    //Source message object configuration and initialization:

    IfxMultican_Can_MsgObj_initConfig(&multican.canMsgObjConfig, &multican.canSrcNode);

    multican.canMsgObjConfig.msgObjId = 0;
    multican.canMsgObjConfig.messageId = 0x777;
    multican.canMsgObjConfig.frame = IfxMultican_Frame_transmit;
    multican.canMsgObjConfig.txInterrupt.enabled = TRUE;
    multican.canMsgObjConfig.txInterrupt.srcId = TX_INTERRUPT_SRC_ID;

    IfxMultican_Can_MsgObj_init(&multican.canSrcMsgObj, &multican.canMsgObjConfig);

    //Destination message object configuration and initialization:

    IfxMultican_Can_MsgObj_initConfig(&multican.canMsgObjConfig, &multican.canDstNode);

    multican.canMsgObjConfig.msgObjId = 1;
    multican.canMsgObjConfig.messageId = 0x777;
    multican.canMsgObjConfig.frame = IfxMultican_Frame_receive;
    multican.canMsgObjConfig.rxInterrupt.enabled = TRUE;
    multican.canMsgObjConfig.rxInterrupt.srcId = RX_INTERRUPT_SRC_ID;

    IfxMultican_Can_MsgObj_init(&multican.canDstMsgObj, &multican.canMsgObjConfig);

}



SplittedPayload MMR_split_payload(uint8_t* payload, uint8_t length){
    SplittedPayload splitted_payload;
    splitted_payload.high_bit = 0;
    splitted_payload.low_bit = 0;
    
    uint8_t i;
    for(i=0;i<4 && i<length;i++){
        splitted_payload.high_bit |= ((uint32_t)(payload[i])) << 8*(4-1-i) ;
    }
    
    for(;i<length;i++){
        splitted_payload.low_bit |= ((uint32_t)(payload[i])) << 8*(4-1-i) ;
    }

    return splitted_payload;
    
    return splitted_payload;

}

void MMR_transmit_message( MmrCanMessage *message ){
     /*
            Accettare i seguenti parametri:
                -MmrCanMessage [Struct]
                    id              : uint32_t
                    isStandardId    : bool
                    payload         : *uint8_t
                    length          : uint8_t

            Assegnare i seguenti parametri:

                multican.canMsgObjConfig.messageId = MmrCanMessage.id
                high_bit = MmrCanMessage.payoload[0:3]  //Bisogna sapere se little endian o big endian
                low_bit  = MmrCanMessage.payload[4:7]
                multican.canMsgObjConfig.control.messageLen = MmrCanMessage.length 

            Bisogna quindi spostare la init del messaggio da MMR_init_multican() a questa funzione
            
            Per convertire payload in highbit e lowbit usate la  struttura SplittedPayload
            Guardate che la funzione MMR_split_payload sia sensata
    */

    multican.canMsgObjConfig.messageId = message->id;
    multican.canMsgObjConfig.control.messageLen = message->length;

    SplittedPayload splittedPayload = MMR_split_payload(message->payload, message->length);

    /*Invalida messaggio ricevuto precedentemente*/
    IfxMultican_Message_init(   &multican.rxMsg,
                                0xFFFFFFFF,
                                0xDEADBEEF,
                                0xDEADBEEF,
                                multican.canMsgObjConfig.control.messageLen);

    /* Initialization of the TX message */
    IfxMultican_Message_init(&multican.txMsg,
                             multican.canMsgObjConfig.messageId,
                             splittedPayload.high_bit,
                             splittedPayload.low_bit,
                             multican.canMsgObjConfig.control.messageLen);

    /* Send the CAN message with the previously defined TX message content */
    while( IfxMultican_Status_notSentBusy ==
           IfxMultican_Can_MsgObj_sendMessage(&multican.canSrcMsgObj, &multican.txMsg) )
    {
    }
}


void trasmesso_fun(){
    //Accendi led 1
    MMR_PIN_write(&pin_tx, MMR_PIN_HIGH);
    //IfxPort_setPinHigh(g_led.led1.port, g_led.led1.pinIndex);
}
void ricevuto_fun(){
    //Leggo lo stato di ricezione e salvo il messaggio ricevuto
    IfxMultican_Status readStatus;
    readStatus = IfxMultican_Can_MsgObj_readMessage(&multican.canDstMsgObj, &multican.rxMsg);
    if( !( readStatus & IfxMultican_Status_newData ) )
    {
        while(1)
        {
        }
    }

    /* If new data has been received but with one message lost, report an error */
    if( readStatus ==  IfxMultican_Status_newDataButOneLost )
    {
        while(1)
        {
        }
    }

    /* Finally, check if the received data matches with the transmitted one */
    if( ( multican.rxMsg.data[0] == multican.txMsg.data[0] ) &&
        ( multican.rxMsg.data[1] == multican.txMsg.data[1] ) &&
        ( multican.rxMsg.id == multican.txMsg.id ) )
    {
        /* Turn on the LED2 to indicate correctness of the received message */
        MMR_PIN_write(&pin_rx, MMR_PIN_HIGH);
    }
}



void initLed(void)
{
    /* ======================================================================
     * Configuration of the pins connected to the LEDs:
     * ======================================================================
     *  - define the GPIO port
     *  - define the GPIO pin that is the connected to the LED
     *  - define the general GPIO pin usage (no alternate function used)
     *  - define the pad driver strength
     * ======================================================================
     */

}