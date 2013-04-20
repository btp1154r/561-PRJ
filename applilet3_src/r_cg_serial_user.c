/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2010, 2011 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_serial_user.c
* Version      : Applilet3 for RL78/G13 V1.03.01 [11 Oct 2011]
* Device(s)    : R5F100LE
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for Serial module.
* Creation Date: 12-04-2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
#include "integer.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern uint8_t * gp_uart0_tx_address;         /* uart0 send buffer address */
extern uint16_t  g_uart0_tx_count;            /* uart0 send data number */
extern uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
extern uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
extern uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
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


uint8_t sbuffer[128];



CurGPSInfo info;


    uint8_t rx_data ;
    
    uint16_t checksum=0;
    uint16_t c=0;
    uint8_t sum[3];
    int x;
    uint8_t readCount=0;
    uint8_t str[20];
    int n;
    int msg_count=0;
    char start=0;
    char substr[10];
    char i; //small loops
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_receive
* Description  : This function is INTSR0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTSR0_vect
__interrupt static void r_uart0_interrupt_receive(void)
{
  
    
    
    rx_data = RXD0;
        
    if (g_uart0_rx_length > g_uart0_rx_count)
    {
        *gp_uart0_rx_address = rx_data;
        gp_uart0_rx_address++;
        g_uart0_rx_count++;

        if (g_uart0_rx_length == g_uart0_rx_count)
        {
            r_uart0_callback_receiveend();
        }
    }
          
          if(rx_data == '\r')  
          {
            
            if(sbuffer[3]=='R')    //change GPRMC lines check
            {
                checksum=0;
                c=0;
                //checksum generation for sbuffer
                x=1;
                while(sbuffer[x]!='*')
                {
                    c ^= sbuffer[x];
                    x++;
                    if(x==128) break;
                }
                
                
                
                //checksum extraction from sbuffer
                x=0;
                while(sbuffer[x]!='\n')
                {
                    if(sbuffer[x]=='*')
                    {
                       sum[0] = sbuffer[x+1];
                       sum[1] = sbuffer[x+2];
                       sum[2] = '\0';
                       checksum= (short)strtol(sum,NULL,16);
                     }
                      x++;
                      if(x==128) break;
                }
                
                //check for proper transmission
              //  if(checksum==c )
                {
                 
                  
                  
#if 0
                   info.hour = 10*(sbuffer[7]-'0') + (sbuffer[8]-'0');  
                   info.min = 10*(sbuffer[9]-'0') + (sbuffer[10]-'0');  
                   info.sec = 10*(sbuffer[11]-'0') + (sbuffer[12]-'0');  
#endif
                   info.lat_deg = 10*(sbuffer[16]-'0') + (sbuffer[17]-'0');
                   
                   for(x=18,i=0;sbuffer[x]!=',';x++,i++)
                   {
                     substr[i]=sbuffer[x];
                   }
                   substr[i] = 0; 
                   info.lat_min =  strtof(substr,NULL);
                   
                   x=x+3;                     
                   info.long_deg = 100*(sbuffer[x]-'0')+10*(sbuffer[x+1]-'0') + (sbuffer[x+2]-'0');
                   
                   
                   for(x=x+3,i=0;sbuffer[x]!=',';x++,i++)
                   {
                     substr[i]=sbuffer[x];
                   }
                   substr[i] = 0;
                   info.long_min = strtof(substr,NULL);
#if 0
                   
                   for(x=x+3,i=0;sbuffer[x]!=',';x++,i++)
                   {
                     substr[i]=sbuffer[x];
                   }
                   substr[i] = 0;
                   info.speed = strtof(substr,NULL);
                   
                   for(x=x+1,i=0;sbuffer[x]!=',';x++,i++)
                   {
                     substr[i]=sbuffer[x];
                   }
                   substr[i] = 0;
                   info.angle = strtof(substr,NULL);
                   
                   x=x+1;
                   info.day = 10*(sbuffer[x]-'0') + (sbuffer[x+1]-'0');  
                   info.month = 10*(sbuffer[x+2]-'0') + (sbuffer[x+3]-'0');  
                   info.year = 10*(sbuffer[x+4]-'0') + (sbuffer[x+5]-'0'); 
                   
                   for(x=x+7,i=0;sbuffer[x]!=',';x++,i++)
                   {
                     substr[i]=sbuffer[x];
                   }
                   substr[i] = 0;
                   info.var = strtof(substr,NULL);
#endif
                   
                   info.valid=1;
                }
                   
               
            }
                
                readCount =0;
            
         }
          
                 
          sbuffer[readCount] = rx_data;
          readCount++;
          if(readCount==128)
            readCount =0;
        
}

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_error
* Description  : This function is INTSRE0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTSRE0_vect
__interrupt static void r_uart0_interrupt_error(void)
{
    uint8_t err_type;

    *gp_uart0_rx_address = RXD0;
    r_uart0_callback_error(err_type);
}

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_send
* Description  : This function is INTST0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTST0_vect
__interrupt static void r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        r_uart0_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_error
* Description  : This function is a callback function when UART0 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi10_interrupt
* Description  : This function is INTCSI10 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTCSI10_vect
__interrupt static void r_csi10_interrupt(void)
{
    uint8_t err_type;
  //delay_ms(1);
  dummy = SSR02;
  err_type = (UCHAR)(SSR02 & _0001_SAU_OVERRUN_ERROR);
  SIR02 = (USHORT)err_type;
  if (err_type == 1U)
	{
	  r_csi10_callback_error(err_type);	/* overrun error occurs */
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
		r_csi10_callback_sendend();	/* complete send */
		r_csi10_callback_receiveend();	/* complete receive */
	  }
  }
}

/***********************************************************************************************************************
* Function Name: r_csi10_callback_receiveend
* Description  : This function is a callback function when CSI10 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi10_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
  G_SPI_ReceivingData=0;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi10_callback_error
* Description  : This function is a callback function when CSI10 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_csi10_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi10_callback_sendend
* Description  : This function is a callback function when CSI10 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi10_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
  G_SPI_SendingData = 0;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
