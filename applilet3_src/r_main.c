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
* Filename:	r_main.c
* Abstract:	This file implements main function.
* APIlib:	Applilet3 for RL78/G13 V1.01.00 [31 May 2011]
* Device:	R5F100LE
* Compiler:	IAR Systems iccrl78
* Creation date:	10-04-2013
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "r_cg_it.h"
/* Start user code for include. Do not edit comment generated here */
#include "lcd.h"
#include "rtc_sched.h"
#include "tasks.h"
#include "profile.h"
#include "region.h"

#include<integer.h>
#include "ff.h"


/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint8_t opbyte0 = 0xEFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte1 = 0xFFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte2 = 0xE8U;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte3 = 0x04U;

/* Set security ID */
#pragma location = "SECUID"
__root const uint8_t secuid[10] = 
	{0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function implements main function.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void  main(void)
{
	/* Start user code. Do not edit comment generated here */

  INIT_LEDS;
	
  R_IT_Start(); 						/* Start interval timer (for RTC_Scheduler) */
  R_CSI10_Start();          /* LCD SPI initialization */
 // LCDInit();              	/* LCD controller initialization*/
  
  SPI_Init();
	
 //LCDPrintf(1,0,"hello");
  Init_RTC_Scheduler();

  Add_Task(Task1, RTC_FREQ_TO_TICKS(11), 2);  /* Flash LED at 5.5 Hz */
  Add_Task(Task4, RTC_FREQ_TO_TICKS(1), 3);   /* Calculate distances */
  Add_Task(Task3, RTC_FREQ_TO_TICKS(1), 4);   /* Update LCD at 1 Hz task frequency */
  Add_Task(FileReadTask, RTC_FREQ_TO_TICKS(10), 1);
//  Add_Task(Task3, RTC_FREQ_TO_TICKS(100), 3);   /* Update LCD at 100 Hz task frequency */

	Init_Profiling();	
  Disable_Profiling();
  Run_RTC_Scheduler();

  /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
