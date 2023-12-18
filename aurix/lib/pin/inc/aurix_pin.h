#ifndef APP_AURIX_INC_AURIX_PIN_H_
#define APP_AURIX_INC_AURIX_PIN_H_

#include <stdint.h>
#include <stdbool.h>
#include <IfxPort_regdef.h>
#include <IfxPort.h>


typedef struct{
  Ifx_P *port;
  uint8_t pin;
  bool hasInvertedLogic;
}MmrPin;


MmrPin MMR_Pin(Ifx_P *port, uint8_t pin, bool hasInvertedLogic);

#endif // !APP_AURIX_INC_AURIX_PIN_H_
