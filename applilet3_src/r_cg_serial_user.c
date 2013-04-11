/*
*******************************************************************************
* Copyright(C) 2011 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation. 
*
* This device driver was created by Applilet3 for RL78/G13
* 16-Bit Single-Chip Microcontrollers
* Filename:	CG_serial_user.c
* Abstract:	This file implements device driver for Serial module.
* APIlib:	Applilet3 for RL78/G13 E1.00b [14 Jan 2011]
* Device:	R5F100LE
* Compiler:	IAR Systems iccrl78
* Creation date:	9/21/2011
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "integer.h"
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
extern uint8_t * gp_csi10_rx_address;         /* csi10 receive buffer address */
extern uint16_t  g_csi10_rx_length;           /* csi10 receive data length */
extern uint16_t  g_csi10_rx_count;            /* csi10 receive data count */
extern uint8_t * gp_csi10_tx_address;         /* csi10 send buffer address */
extern uint16_t  g_csi10_send_length;         /* csi10 send data length */
extern uint16_t  g_csi10_tx_count;            /* csi10 send data count */
/* Start user code for global. Do not edit comment generated here */
volatile UCHAR G_UART_SendingData = 0;
volatile UCHAR G_UART_ReceivingData = 0;
volatile UCHAR G_SPI_SendingData = 0;
volatile UCHAR G_SPI_ReceivingData = 0;
volatile UCHAR G_IIC_SendingData = 0;
volatile UCHAR G_IIC_ReceivingData = 0;
volatile UCHAR dummy;
volatile UCHAR err_type;
/* End user code. Do not edit comment generated here */
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
void CSI10_ReceiveEndCallback(void)
{
  /* Start user code. Do not edit comment generated here */
  G_SPI_ReceivingData = 0;
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
void CSI10_ErrorCallback(UCHAR err_type)
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
void CSI10_SendEndCallback(void)
{
  /* Start user code. Do not edit comment generated here */
  G_SPI_SendingData = 0;
  /* End user code. Do not edit comment generated here */
}

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
__interrupt void MD_INTCSI10(void)
{

  delay_ms(1);
  dummy = SSR02;
  err_type = (UCHAR)(SSR02 & _0001_SAU_OVERRUN_ERROR);
  SIR02 = (USHORT)err_type;
  if (err_type == 1U)
	{
	  CSI10_ErrorCallback(err_type);	/* overrun error occurs */
	}
  //	else                                    /* it seems we're getting overruns once in a while.. please fix :) */
  {
	if (g_csi10_tx_count > 0U)
	  {
		if (g_csi10_rx_length > 0)          /* Check if we need to receive within this transmit*/
		  {             
			*gp_csi10_rx_address = SIO10;
			g_csi10_rx_count++;
			gp_csi10_rx_address++;
		  }
		else
		  {
			dummy = SIO10;             /* do we need to trigger a read if we don't need the data? */
		  }
		SIO10 = *gp_csi10_tx_address;
		gp_csi10_tx_address++;
		g_csi10_tx_count--;		       
	  }
	else if (g_csi10_rx_length > 1)              /* Read without sending data. Check on > 1 because fall through triggers another receive */
	  {
		*gp_csi10_rx_address = SIO10;
		g_csi10_rx_count++;
		gp_csi10_rx_address++;
		g_csi10_rx_length--;
		SIO10 = 0xFF;                     /* dummy byte */
	  }
	else 
	  {
		if (g_csi10_tx_count == 0U && (g_csi10_rx_length > 0) )
		  {
			*gp_csi10_rx_address = SIO10;
		  }
		CSI10_SendEndCallback();	/* complete send */
		CSI10_ReceiveEndCallback();	/* complete receive */
	  }
  }
}
