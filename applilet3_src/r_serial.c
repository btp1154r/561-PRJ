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
* Filename:	r_serial.c
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
uint8_t  *gp_Csi10RxAddress;	/* csi10 receive buffer address */
uint16_t g_Csi10RxLen;		/* csi10 receive data length */
uint16_t g_Csi10RxCnt;		/* csi10 receive data count */
uint8_t  *gp_Csi10TxAddress;	/* csi10 send buffer address */
uint16_t g_Csi10TxLen;		/* csi10 send data length */
uint16_t g_Csi10TxCnt;		/* csi10 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the SAU0 module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_SAU0_Create(void)
{
	SAU0EN = 1U;	/* supply SAU0 clock */
	NOP();
	NOP();
	NOP();
	NOP();
	SPS0 = _0000_SAU_CK00_FCLK_0 | _0000_SAU_CK01_FCLK_0;
	R_CSI10_Create();
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the CSI10 module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_CSI10_Create(void)
{
	ST0 |= _0004_SAU_CH2_STOP_TRG_ON;	/* disable CSI10 */
	CSIMK10 = 1U;	/* disable INTCSI10 interrupt */
	CSIIF10 = 0U;	/* clear INTCSI10 interrupt flag */
	/* Set INTCSI10 low priority */
	CSIPR110 = 1U;
	CSIPR010 = 1U;
	SIR02 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;	/* clear error flag */
	SMR02 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS | _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
	SCR02 = _C000_SAU_RECEPTION_TRANSMISSION | _0000_SAU_TIMING_1 | _0000_SAU_MSB | _0007_SAU_LENGTH_8;
	SDR02 = _6400_CSI10_DIVISOR;
	SO0 |= _0400_SAU_CH2_CLOCK_OUTPUT_1;	/* CSI10 clock initial level */
	SO0 &= ~_0004_SAU_CH2_DATA_OUTPUT_1;	/* CSI10 SO initial level */
	SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;	/* enable CSI10 output */
	/* Set SI10 pin */
	PM0 |= 0x08U;
	/* Set SO10 pin */
	P0 |= 0x04U;
	PM0 &= 0xFBU;
	/* Set SCK10 pin */
	P0 |= 0x10U;
	PM0 &= 0xEFU;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts the CSI10 module operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_CSI10_Start(void)
{
	CSIIF10 = 0U;	/* clear INTCSI10 interrupt flag */
	CSIMK10 = 0U;	/* enable INTCSI10 */
	SO0 |= _0400_SAU_CH2_CLOCK_OUTPUT_1;	/* CSI10 clock initial level */
	SO0 &= ~_0004_SAU_CH2_DATA_OUTPUT_1;	/* CSI10 SO initial level */
	SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;	/* enable CSI10 output */
	SS0 |= _0004_SAU_CH2_START_TRG_ON;	/* enable CSI10 */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops the CSI10 module operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_CSI10_Stop(void)
{
	ST0 |= _0004_SAU_CH2_STOP_TRG_ON;	/* disable CSI10 */
	SOE0 &= ~_0004_SAU_CH2_OUTPUT_ENABLE;	/* disable CSI10 output */
	CSIMK10 = 1U;	/* disable INTCSI10 interrupt */
	CSIIF10 = 0U;	/* clear INTCSI10 interrupt flag */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function sends and receives CSI10 data.
**
**  Parameters:
**	txbuf: transfer buffer pointer
**	txnum: buffer size
**	rxbuf: receive buffer pointer
**
**  Returns:
**	MD_OK
**	MD_ARGERROR
**
**-----------------------------------------------------------------------------
*/
MD_STATUS R_CSI10_Send_Receive(uint8_t *txbuf, uint16_t txnum, uint8_t *rxbuf)
{
	MD_STATUS status = MD_OK;

	if (txnum < 1U)
	{
		status = MD_ARGERROR;
	}
	else
	{
		g_Csi10TxCnt = txnum;	/* send data count */
		gp_Csi10TxAddress = txbuf;	/* send buffer pointer */
		gp_Csi10RxAddress = rxbuf;	/* receive buffer pointer */
		CSIMK10 = 1U;	/* disable INTCSI10 interrupt */
		SIO10 = *gp_Csi10TxAddress;	/* started by writing data to SDR[7:0] */
		gp_Csi10TxAddress++;
		g_Csi10TxCnt--;
		CSIMK10 = 0U;	/* enable INTCSI10 interrupt */
	}

	return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
