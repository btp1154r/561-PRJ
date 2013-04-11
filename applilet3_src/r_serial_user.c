/*
*******************************************************************************
* Copyright(C) 2010, 2011 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation. 
*
* This device driver was created by Applilet3 for RL78/G13
* 16-Bit Single-Chip Microcontrollers
* Filename:	r_serial_user.c
* Abstract:	This file implements device driver for Serial module.
* APIlib:	Applilet3 for RL78/G13 V1.01.00 [31 May 2011]
* Device:	R5F100LE
* Compiler:	IAR Systems iccrl78
* Creation date:	11-04-2013
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
extern uint8_t  *gp_Csi10RxAddress;	/* csi10 receive buffer address */
extern uint16_t g_Csi10RxLen;		/* csi10 receive data length */
extern uint16_t g_Csi10RxCnt;		/* csi10 receive data count */
extern uint8_t  *gp_Csi10TxAddress;	/* csi10 send buffer address */
extern uint16_t g_Csi10TxLen;		/* csi10 send data length */
extern uint16_t g_Csi10TxCnt;		/* csi10 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTCSI10 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTCSI10_vect
__interrupt void R_CSI10_Interrupt(void)
{
	uint8_t err_type;

	err_type = (uint8_t)(SSR02 & _0001_SAU_OVERRUN_ERROR);
	SIR02 = (uint16_t)err_type;
	if (err_type == 1U)
	{
		R_CSI10_Callback_Error(err_type);	/* overrun error occurs */
	}
	else
	{
		if (g_Csi10TxCnt > 0U)
		{
			*gp_Csi10RxAddress = SIO10;
			gp_Csi10RxAddress++;
			SIO10 = *gp_Csi10TxAddress;
			gp_Csi10TxAddress++;
			g_Csi10TxCnt--;		
		}
		else 
		{
			if (g_Csi10TxCnt == 0U)
			{
				*gp_Csi10RxAddress = SIO10;
			}
			R_CSI10_Callback_SendEnd();	/* complete send */
			R_CSI10_Callback_ReceiveEnd();	/* complete receive */
		}
	}
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is a callback function when CSI10 finishes reception.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_CSI10_Callback_ReceiveEnd(void)
{
	/* Start user code. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is a callback function when CSI10 reception error occurs.
**
**  Parameters:
**	err_type: error type value
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_CSI10_Callback_Error(uint8_t err_type)
{
	/* Start user code. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is a callback function when CSI10 finishes transmission.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_CSI10_Callback_SendEnd(void)
{
	/* Start user code. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
