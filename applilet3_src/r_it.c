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
* Filename:	r_it.c
* Abstract:	This file implements device driver for IT module.
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
#include "r_cg_it.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the IT module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_IT_Create(void)
{
	RTCEN = 1U;	/* supply IT clock */
	ITMC = _0000_IT_OPERATION_DISABLE;	/* disable IT operation */
	ITMK = 1U;	/* disable INTIT interrupt */
	ITIF = 0U;	/* clear INTIT interrupt flag */
	/* Set INTIT low priority */
	ITPR1 = 1U;
	ITPR0 = 1U;
	ITMC = _0020_ITMCMP_VALUE;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts IT module operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_IT_Start(void)
{
	ITMC |= _8000_IT_OPERATION_ENABLE;	/* enable IT operation */
	ITIF = 0U;	/* clear INTIT interrupt flag */
	ITMK = 0U;	/* enable INTIT interrupt */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops IT module operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_IT_Stop(void)
{
	ITMK = 1U;	/* disable INTIT interrupt */
	ITIF = 0U;	/* clear INTIT interrupt flag */
	ITMC &= (uint16_t)~_8000_IT_OPERATION_ENABLE;	/* disable IT operation */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
