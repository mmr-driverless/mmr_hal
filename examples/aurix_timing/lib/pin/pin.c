#include "inc/pin.h"


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
    IfxPort_State value=IfxPort_getPinState(pin->port, pin->pin);
    return gpioToMmr(pin,value);
 
}

void MMR_PIN_write(MmrPin* pin, MmrPinState value)
{
    IfxPort_State new_value= mmrToGpio(pin,value);
    IfxPort_setPinState(pin->port, pin->pin, new_value);
    
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


