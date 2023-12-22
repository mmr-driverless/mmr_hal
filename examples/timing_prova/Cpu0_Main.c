/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of 
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and 
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all 
 * derivative works of the Software, unless such copies or derivative works are solely in the form of 
 * machine-executable object code generated by a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *
 *********************************************************************************************************************/
#include <IfxPort_regdef.h>
#include <IfxPort.h>
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "pin.h"
#include "aurix_pin.h"
#include "Bsp.h"
#include "delay.h"



#define WAIT_TIME   1000                                                    /* Wait time constant in milliseconds   */


IfxCpu_syncEvent g_cpuSyncEvent = 0;




int core0_main(void)
{

    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);


    //Poi inizia a blinkare un led
    MmrPin blinky_led = MMR_Pin(&MODULE_P33, 12 , TRUE);

    MMR_PIN_write(&blinky_led, MMR_PIN_LOW);
    IfxPort_setPinModeOutput(blinky_led.port, blinky_led.pin, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinPadDriver(blinky_led.port, blinky_led.pin, IfxPort_PadDriver_cmosAutomotiveSpeed1);

    MmrDelay mmr_delay = MMR_Delay(WAIT_TIME);
    bool status = FALSE;

    while(1)
    {
        if( MMR_DELAY_WaitAsync(&mmr_delay) ){
            if(status){
                MMR_PIN_write(&blinky_led, MMR_PIN_LOW);
                status = FALSE;
            }else{
                MMR_PIN_write(&blinky_led, MMR_PIN_HIGH);
                status = TRUE;
            }
        }
    }
    return (1);
}





//For Testing


