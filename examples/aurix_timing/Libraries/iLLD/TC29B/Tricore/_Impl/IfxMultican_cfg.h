/**
 * \file IfxMultican_cfg.h
 * \brief MULTICAN on-chip implementation data 
 * \ingroup IfxLld_Multican 
 *
 * \version iLLD_1_0_1_16_1
 * \copyright Copyright (c) 2017 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees. If and as long as no such terms
 * of use are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * \defgroup IfxLld_Multican MULTICAN
 * \ingroup IfxLld
 * \defgroup IfxLld_Multican_Impl Implementation
 * \ingroup IfxLld_Multican
 * \defgroup IfxLld_Multican_Std Standard Driver
 * \ingroup IfxLld_Multican
 */

#ifndef IFXMULTICAN_CFG_H
#define IFXMULTICAN_CFG_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Cpu/Std/Ifx_Types.h"
#include "IfxCan_reg.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief Number of CAN message objects 
 */
#define IFXMULTICAN_NUM_MESSAGE_OBJECTS (256)
                
/** \brief Number of CAN nodes 
 */
#define IFXMULTICAN_NUM_NODES (4)
                
/** \brief Number of service requests 
 */
#define IFXMULTICAN_NUM_SRC (16)
                
/** \brief Number of CANR message objects 
 */
#define IFXMULTICAN_R_NUM_MESSAGE_OBJECTS (128)
                
/** \brief Number of CANR nodes 
 */
#define IFXMULTICAN_R_NUM_NODES (2)
                
/** \brief Number of CANR service requests 
 */
#define IFXMULTICAN_R_NUM_SRC (8)
                
#define IFXMULTICAN_NUM_MODULES (2)
                
/** \brief Number of CANR service requests 
 */
#define IFXMULTICAN_R_NUM_SRC (8)
                


/******************************************************************************/
/*-------------------------------Enumerations---------------------------------*/
/******************************************************************************/

/** \brief List of the available multican resource.  
 */
typedef enum
{
    IfxMultican_Index_none = -1,                   /**< \brief Not Selected */
    IfxMultican_Index_0 = 0,      /**< \brief MULTICAN index 0  */
    IfxMultican_Index_1           /**< \brief MULTICAN index 1  */
} IfxMultican_Index;

/** \brief CAN Nodes definition  
 */
typedef enum
{
    IfxMultican_NodeId_none = -1,                    /**< \brief None of the Ifx_CAN Nodes */
    IfxMultican_NodeId_0 = 0,      /**< \brief Ifx_CAN Node 0  */
    IfxMultican_NodeId_1,          /**< \brief Ifx_CAN Node 1  */
    IfxMultican_NodeId_2,          /**< \brief Ifx_CAN Node 2  */
    IfxMultican_NodeId_3           /**< \brief Ifx_CAN Node 3  */
} IfxMultican_NodeId;

/** \brief Service request ID  
 */
typedef enum
{
    IfxMultican_SrcId_0 = 0,      /**< \brief Service request ID 0  */
    IfxMultican_SrcId_1,          /**< \brief Service request ID 1  */
    IfxMultican_SrcId_2,          /**< \brief Service request ID 2  */
    IfxMultican_SrcId_3,          /**< \brief Service request ID 3  */
    IfxMultican_SrcId_4,          /**< \brief Service request ID 4  */
    IfxMultican_SrcId_5,          /**< \brief Service request ID 5  */
    IfxMultican_SrcId_6,          /**< \brief Service request ID 6  */
    IfxMultican_SrcId_7,          /**< \brief Service request ID 7  */
    IfxMultican_SrcId_8,          /**< \brief Service request ID 8  */
    IfxMultican_SrcId_9,          /**< \brief Service request ID 9  */
    IfxMultican_SrcId_10,         /**< \brief Service request ID 10  */
    IfxMultican_SrcId_11,         /**< \brief Service request ID 11  */
    IfxMultican_SrcId_12,         /**< \brief Service request ID 12  */
    IfxMultican_SrcId_13,         /**< \brief Service request ID 13  */
    IfxMultican_SrcId_14,         /**< \brief Service request ID 14  */
    IfxMultican_SrcId_15          /**< \brief Service request ID 15  */
} IfxMultican_SrcId;


/******************************************************************************/
/*-------------------Global Exported Variables/Constants----------------------*/
/******************************************************************************/

IFX_EXTERN IFX_CONST IfxModule_IndexMap IfxMultican_cfg_indexMap[IFXMULTICAN_NUM_MODULES];


#endif /* IFXMULTICAN_CFG_H */
