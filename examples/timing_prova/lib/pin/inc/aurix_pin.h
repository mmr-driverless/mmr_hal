#ifndef APP_AURIX_INC_AURIX_PIN_H_
#define APP_AURIX_INC_AURIX_PIN_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef IFXPORT_REGDEF_H
typedef enum
{
    IfxPort_State_high       = (0 << 16) | (1U << 0), /**< \brief Ifx_P pin is set to high. */
    IfxPort_State_low        = (1U << 16) | (0 << 0), /**< \brief Ifx_P pin is set to low. */
} IfxPort_State;

typedef union
{
    unsigned int U;                         /**< \brief Unsigned access */
    signed int I;                           /**< \brief Signed access */
} Ifx_P_IN;

typedef union
{
    unsigned int U;                         /**< \brief Unsigned access */
    signed int I;                           /**< \brief Signed access */
} Ifx_P_OMR;

typedef volatile struct _Ifx_P
{
    Ifx_P_OMR OMR;                          /**< \brief 4, Port Output Modification Register */
    Ifx_P_IN IN;                            /**< \brief 24, Port Input Register */
} Ifx_P;


#endif

typedef struct{
  Ifx_P *port;
  uint8_t pin;
  bool hasInvertedLogic;
} MmrPin;

typedef enum{
    MMR_PIN_LOW = 0,
    MMR_PIN_HIGH
}MmrPinState;

/*Lettura PIN*/
MmrPinState MMR_PIN_read(MmrPin* pin);

/*Scrittura di un pin
    -Value: bisogna passargli le define definite sopra
*/
void MMR_PIN_write(MmrPin* pin, MmrPinState value);


MmrPin MMR_Pin(Ifx_P *port, uint8_t pin, bool hasInvertedLogic);

#endif // !APP_AURIX_INC_AURIX_PIN_H_
