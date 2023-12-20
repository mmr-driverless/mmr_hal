
#include "inc/timing.h"
#include <IfxStm.h>

//static TickProvider __tickProvider;

/*
void MMR_SetTickProvider(TickProvider tickProvider) {
  __tickProvider = MMR_GetTick;
}
*/

Tick MMR_GetTick() {
  
  if (IfxStm_getFrequency( BSP_DEFAULT_TIMER ) == 0 ){
    return (Tick) -1;
  }

  return (Tick) ( IfxStm_get(IFXSTM_DEFAULT_TIMER) & TIME_INFINITE ) * 1000 / IfxStm_getFrequency( BSP_DEFAULT_TIMER );
}
