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
* Filename:	r_cg_userdefine.h
* Abstract:	This file includes user definition.
* APIlib:	Applilet3 for RL78/G13 V1.01.00 [31 May 2011]
* Device:	R5F100LE
* Compiler:	IAR Systems iccrl78
* Creation date:	10-04-2013
*******************************************************************************
*/

#ifndef _MD_USER_DEF_
#define _MD_USER_DEF_
/*
*******************************************************************************
**  User define
*******************************************************************************
*/

/* Start user code for function. Do not edit comment generated here */

// YRDKRL78 devices

#define DIR_OUT	0
#define DIR_IN  1

#define RLED1 	P5_bit.no5
#define RLED2 	P6_bit.no2
#define RLED3 	P6_bit.no3
#define GLED4 	P5_bit.no2
#define GLED5 	P5_bit.no3
#define GLED6 	P5_bit.no4
#define BACKLIGHT P7_bit.no0

#define RLED1_M 	PM5_bit.no5
#define RLED2_M 	PM6_bit.no2
#define RLED3_M 	PM6_bit.no3
#define GLED4_M 	PM5_bit.no2
#define GLED5_M 	PM5_bit.no3
#define GLED6_M 	PM5_bit.no4
#define BACKLIGHT_M PM7_bit.no0

#define INIT_LEDS { RLED1_M = RLED2_M = RLED3_M = GLED4_M = GLED5_M = GLED6_M = DIR_OUT; }

#define MAX(a,b) ((a>b)? a : b)
#define MIN(a,b) ((a<b)? a : b)

/* End user code. Do not edit comment generated here */
#endif
