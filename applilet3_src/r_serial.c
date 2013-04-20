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
* Creation date:	12-04-2013
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
uint8_t  *gp_Uart0TxAddress;	/* uart0 transmit buffer address */
uint16_t g_Uart0TxCnt;		/* uart0 transmit data number */
uint8_t  *gp_Uart0RxAddress;	/* uart0 receive buffer address */
uint16_t g_Uart0RxCnt;		/* uart0 receive data number */
uint16_t g_Uart0RxLen;		/* uart0 receive data length */
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
	SPS0 = _0004_SAU_CK00_FCLK_4 | _0090_SAU_CK01_FCLK_9;
	R_UART0_Create();
	R_CSI10_Create();
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the UART0 module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_UART0_Create(void)
{
	ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;	/* disable UART0 receive and transmit */
	STMK0 = 1U;	/* disable INTST0 interrupt */
	STIF0 = 0U;	/* clear INTST0 interrupt flag */
	SRMK0 = 1U;	/* disable INTSR0 interrupt */
	SRIF0 = 0U;	/* clear INTSR0 interrupt flag */
	SREMK0 = 1U;	/* disable INTSRE0 interrupt */
	SREIF0 = 0U;	/* clear INTSRE0 interrupt flag */
	/* Set INTST0 low priority */
	STPR10 = 1U;
	STPR00 = 1U;
	/* Set INTSR0 low priority */
	SRPR10 = 1U;
	SRPR00 = 1U;
	/* Set INTSRE0 low priority */
	SREPR10 = 1U;
	SREPR00 = 1U;
	SMR00 = _0020_SAU_SMRMN_INITIALVALUE | _8000_SAU_CLOCK_SELECT_CK01 | _0000_SAU_TRIGGER_SOFTWARE | _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
	SCR00 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 | _0007_SAU_LENGTH_8;
	SDR00 = _CE00_UART0_TRANSMIT_DIVISOR;
	NFEN0 |= _01_SAU_RXD0_FILTER_ON;
	SIR01 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;	/* clear error flag */
	SMR01 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL | _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
	SCR01 = _4000_SAU_RECEPTION | _0400_SAU_INTSRE_ENABLE | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 | _0007_SAU_LENGTH_8;
	SDR01 = _CE00_UART0_RECEIVE_DIVISOR;
	SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;
	SOL0 |= _0000_SAU_CHANNEL0_NORMAL;	/* output level normal */
	SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;	/* enable UART0 output */
	/* Set RxD0 pin */
	PM1 |= 0x02U;
	/* Set TxD0 pin */
	P1 |= 0x04U;
	PM1 &= 0xFBU;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts the UART0 module operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_UART0_Start(void)
{
	STIF0 = 0U;	/* clear INTST0 interrupt flag */
	STMK0 = 0U;	/* enable INTST0 interrupt */
	SRIF0 = 0U;	/* clear INTSR0 interrupt flag */
	SRMK0 = 0U;	/* enable INTSR0 interrupt */
	SREIF0 = 0U;	/* clear INTSRE0 interrupt flag */
	SREMK0 = 0U;	/* enable INTSRE0 interrupt */
	SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;	/* output level normal */
	SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;	/* enable UART0 output */
	SS0 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;	/* enable UART0 receive and transmit */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops the UART0 module operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_UART0_Stop(void)
{
	ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;	/* disable UART0 receive and transmit */
	SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;	/* disable UART0 output */
	STMK0 = 1U;	/* disable INTST0 interrupt */
	STIF0 = 0U;	/* clear INTST0 interrupt flag */
	SRMK0 = 1U;	/* disable INTSR0 interrupt */
	SRIF0 = 0U;	/* clear INTSR0 interrupt flag */
	SREMK0 = 1U;	/* disable INTSRE0 interrupt */
	SREIF0 = 0U;	/* clear INTSRE0 interrupt flag */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function receives UART0 data.
**
**  Parameters:
**	rxbuf: receive buffer pointer
**	rxnum: buffer size
**
**  Returns:
**	MD_OK
**	MD_ARGERROR
**
**-----------------------------------------------------------------------------
*/
MD_STATUS R_UART0_Receive(uint8_t *rxbuf, uint16_t rxnum)
{
	MD_STATUS status = MD_OK;

	if (rxnum < 1U)
	{
		status = MD_ARGERROR;
	}
	else
	{
		g_Uart0RxCnt = 0U;
		g_Uart0RxLen = rxnum;
		gp_Uart0RxAddress = rxbuf;
	}

	return (status);
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function sends UART0 data.
**
**  Parameters:
**	txbuf: transfer buffer pointer
**	txnum: buffer size
**
**  Returns:
**	MD_OK
**	MD_ARGERROR
**
**-----------------------------------------------------------------------------
*/
MD_STATUS R_UART0_Send(uint8_t* txbuf, uint16_t txnum)
{
	MD_STATUS status = MD_OK;

	if (txnum < 1U)
	{
		status = MD_ARGERROR;
	}
	else
	{
		gp_Uart0TxAddress = txbuf;
		g_Uart0TxCnt = txnum;
		STMK0 = 1U;	/* disable INTST0 interrupt */
		TXD0 = *gp_Uart0TxAddress;
		gp_Uart0TxAddress++;
		g_Uart0TxCnt--;
		STMK0 = 0U;	/* enable INTST0 interrupt */
	}

	return (status);
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
	/* Set INTCSI10 high priority */
	CSIPR110 = 0U;
	CSIPR010 = 0U;
	SIR02 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;	/* clear error flag */
	SMR02 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS | _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
	SCR02 = _C000_SAU_RECEPTION_TRANSMISSION | _0000_SAU_TIMING_1 | _0000_SAU_MSB | _0007_SAU_LENGTH_8;
	SDR02 = _0400_CSI10_DIVISOR;
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
