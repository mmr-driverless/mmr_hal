#include <stdint.h>

#ifndef LIB_TIMING_INC_TIMING_H_
#define LIB_TIMING_INC_TIMING_H_

#define MODULE_STM0     (*(Ifx_STM*)0xF0000000u)
#define TIME_INFINITE   ((Ifx_TickTime)0x7FFFFFFFFFFFFFFFLL)

#ifndef IFXSTM_DEFAULT_TIMER
#define IFXSTM_DEFAULT_TIMER (&MODULE_STM0)
#endif

#ifndef BSP_DEFAULT_TIMER
#define BSP_DEFAULT_TIMER  (&MODULE_STM0)
#endif



typedef long long Tick;

Tick MMR_GetTick( void );


// typedef Tick (*TickProvider)();
// void MMR_SetTickProvider(TickProvider tickProvider);


#endif // !LIB_TIMING_INC_TIMING_H_
