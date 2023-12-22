//#include "inc/pin.h"
#include "inc/aurix_pin.h"

#define IFX_INLINE         static inline __attribute__ ((always_inline))            /*Makes the function always inlined */

IFX_INLINE uint32_t Ifx__extru(uint32_t a, uint32_t p, uint32_t w)
{
    uint32_t res;

    return res;
}
#define Ifx__getbit( address, bitoffset ) __extru( *(address), bitoffset, 1 )

//static inline bool IfxPort_getPinState(Ifx_P *port, uint8_t pinIndex);
//static inline void IfxPort_setPinState(Ifx_P *port, uint8_t pinIndex, IfxPort_State action);

static IfxPort_State mmrToGpio(MmrPin *pin, MmrPinState state);
static MmrPinState gpioToMmr(MmrPin *pin, IfxPort_State state);


MmrPin MMR_Pin(Ifx_P *port, uint8_t pin, bool hasInvertedLogic) {
    MmrPin mmrPin;

    mmrPin.port = port;
    mmrPin.pin = pin;
    mmrPin.hasInvertedLogic = hasInvertedLogic;

    return mmrPin;

    /*return (MmrPin) {
    .port = port,
    .pin = pin,
    .hasInvertedLogic = hasInvertedLogic,
  };*/
}

MmrPinState MMR_PIN_read(MmrPin* pin)
{
    //IfxPort_State value=IfxPort_getPinState(pin->port, pin->pin);
    IfxPort_State value= (__getbit(&(pin->port)->IN.U, pin->pin) != 0) ? true : false;
    return gpioToMmr(pin,value);
 
}

void MMR_PIN_write(MmrPin* pin, MmrPinState value)
{
    IfxPort_State new_value= mmrToGpio(pin,value);
    //IfxPort_setPinState(pin->port, pin->pin, new_value);

    pin->port->OMR.U = new_value << pin->pin;
    
}

static IfxPort_State mmrToGpio(MmrPin *pin, MmrPinState state) {
  if (pin->hasInvertedLogic) {
    return state == MMR_PIN_HIGH
      ? IfxPort_State_low
      : IfxPort_State_high;
  }

  return state == MMR_PIN_HIGH
    ? IfxPort_State_high
    : IfxPort_State_low;
}

static MmrPinState gpioToMmr(MmrPin *pin, IfxPort_State state) {
  if (pin->hasInvertedLogic) {
    return state == IfxPort_State_high
      ? MMR_PIN_LOW
      : MMR_PIN_HIGH;
  }

  return state == IfxPort_State_high
    ? MMR_PIN_HIGH
    : MMR_PIN_LOW;
}


