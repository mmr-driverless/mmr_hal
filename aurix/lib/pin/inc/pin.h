#include "aurix_pin.h"

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



