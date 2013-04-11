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
* Filename:	r_cg_it.h
* Abstract:	This file implements device driver for IT module.
* APIlib:	Applilet3 for RL78/G13 V1.01.00 [31 May 2011]
* Device:	R5F100LE
* Compiler:	IAR Systems iccrl78
* Creation date:	11-04-2013
*******************************************************************************
*/

#ifndef _MDIT_
#define _MDIT_
/*
*******************************************************************************
**  Register bit define
*******************************************************************************
*/
/* 
	Interval timer control register (ITMC)
*/
/* Interval timer operation enable/disable specification (RINTE) */
#define	_0000_IT_OPERATION_DISABLE	0x0000U	/* disable interval timer operation */
#define	_8000_IT_OPERATION_ENABLE	0x8000U	/* enable interval timer operation */
/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
/* Interval timer compare value (ITMCMP11 - 0) */
#define _0020_ITMCMP_VALUE		0x0020U
/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
void R_IT_Create(void);
void R_IT_Start(void);
void R_IT_Stop(void);
__interrupt void R_IT_Interrupt(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
