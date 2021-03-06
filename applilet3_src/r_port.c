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
* Filename:	r_port.c
* Abstract:	This file implements device driver for PORT module.
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
#include "r_cg_port.h"
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
**	This function initializes the Port I/O.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_PORT_Create(void)
{
	P1 = _00_Pn0_OUTPUT_0 | _00_Pn5_OUTPUT_0;
	P3 = _00_Pn0_OUTPUT_0;
	P4 = _00_Pn2_OUTPUT_0;
	P5 = _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0;
	P6 = _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0;
	PM1 = _00_PMn0_MODE_OUTPUT | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE | _00_PMn5_MODE_OUTPUT | _40_PMn6_NOT_USE | _80_PMn7_NOT_USE;
	PM3 = _00_PMn0_MODE_OUTPUT | _02_PMn1_NOT_USE | _FC_PM3_DEFAULT;
	PM4 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _00_PMn2_MODE_OUTPUT | _08_PMn3_NOT_USE | _F0_PM4_DEFAULT;
	PM5 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _C0_PM5_DEFAULT;
	PM6 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | _F0_PM6_DEFAULT;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
