/***********************************************************************
 * $Id::                                                               $
 *
 * Project:	uart: Simple UART echo for MCUXpresso LPC1768
 * File:	uart.c
 * ***********************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

/*****************************************************************************
 *   History
 *   2017.08.29  VER 1.02    Added support for FreeRTOS , tested on MCUXpresso and LPC1768
 *   2010.07.01  ver 1.01    Added support for UART3, tested on LPCXpresso 1700
 *   2009.05.27  ver 1.00    Prelimnary version, first Release
 *
 *
******************************************************************************/
#include "LPC17xx.h"
#include "type.h"
#include "uart.h"
#include "stdutils.h"
#define SBIT_THRE 0x05u

volatile uint32_t ulUART0Status, ulUART1Status, ulUART3Status;
volatile uint8_t ucUART0TxEmpty = 1, ucUART1TxEmpty = 1, ucUART3TxEmpty=1;
volatile uint8_t ucUART0Buffer[BUFSIZE], ucUART1Buffer[BUFSIZE], ucUART3Buffer[BUFSIZE];
volatile uint32_t ulUART0Count = 0, ulUART1Count = 0, ulUART3Count = 0;



/*****************************************************************************
** Function name:		UARTSend
**
** Descriptions:		Send a block of data to the UART 0 port based
**						on the data length
**
** parameters:			portNum, buffer pointer, and data length
** Returned value:		None
** 
*****************************************************************************/
void vUARTSend( uint32_t v_ulportNum, uint8_t *v_ucBufferPtr, uint32_t v_ulLength )
{
  if ( v_ulportNum == 0 )
  {
    while ( v_ulLength != 0 )
    {
	  /* THRE status, contain valid data */
	  while ( !(ucUART0TxEmpty & 0x01) );
	  LPC_UART0->THR = *v_ucBufferPtr;
	  ucUART0TxEmpty = 0;	/* not empty in the THR until it shifts out */
	  v_ucBufferPtr++;
	  v_ulLength--;
	}
  }
  else if (v_ulportNum == 1)
  {
	while ( v_ulLength != 0 )
    {
	  /* THRE status, contain valid data */
	  while ( !(ucUART1TxEmpty & 0x01) );
	  LPC_UART1->THR = *v_ucBufferPtr;
	  ucUART1TxEmpty = 0;	/* not empty in the THR until it shifts out */
	  v_ucBufferPtr++;
	  v_ulLength--;
    }
  }
  else if ( v_ulportNum == 3 )
  {
    while ( v_ulLength != 0 )
    {
	  /* THRE status, contain valid data */
	  while ( !(ucUART3TxEmpty & 0x01) );
	  LPC_UART3->THR = *v_ucBufferPtr;
	  ucUART3TxEmpty = 0;	/* not empty in the THR until it shifts out */
	  v_ucBufferPtr++;
	  v_ulLength--;
	}
  }
  return;
}
/*******************************************************************************************************************/
void vuart_TxChar(char v_ch)
{
    while(util_IsBitCleared(LPC_UART0->LSR,SBIT_THRE)); // Wait for Previous transmission
    LPC_UART0->THR=v_ch;                                  // Load the data to be transmitted
}


/* Function to initialize the UART0 at specifief baud rate ********************************************************/
void vuart_init(uint32_t v_ulbaudrate)
{
    uint32_t var_ulUartPclk_u32,var_ulPclk_u32,var_ulRegValue_u32;

    LPC_PINCON->PINSEL0 &= ~LPC_PINCON_PINSEL0_AND;
    LPC_PINCON->PINSEL0 |= LPC_PINCON_PINSEL0_OR;            // Enable TxD0 P0.2 and p0.3

    LPC_UART0->FCR = (1<<SBIT_FIFO) | (1<<SBIT_RxFIFO) | (1<<SBIT_TxFIFO); // Enable FIFO and reset Rx/Tx FIFO buffers
    LPC_UART0->LCR = (0x03<<SBIT_WordLenght) | (1<<SBIT_DLAB);             // 8bit data, 1Stop bit, No parity


    /** Baud Rate Calculation :
       PCLKSELx registers contains the PCLK info for all the clock dependent peripherals.
       Bit6,Bit7 contains the Uart Clock(ie.UART_PCLK) information.
       The UART_PCLK and the actual Peripheral Clock(PCLK) is calculated as below.
       (Refer data sheet for more info)
       UART_PCLK    PCLK
         0x00       SystemFreq/4
         0x01       SystemFreq
         0x02       SystemFreq/2
         0x03       SystemFreq/8
     **/

    var_ulUartPclk_u32 = (LPC_SC->PCLKSEL0 >> 6) & 0x03;

    switch( var_ulUartPclk_u32 )
    {
    case 0x00:
        var_ulPclk_u32 = SystemCoreClock/4;
        break;
    case 0x01:
        var_ulPclk_u32 = SystemCoreClock;
        break;
    case 0x02:
        var_ulPclk_u32 = SystemCoreClock/2;
        break;
    case 0x03:
        var_ulPclk_u32 = SystemCoreClock/8;
        break;
    }


    var_ulRegValue_u32 = ( var_ulPclk_u32 / (16 * v_ulbaudrate ));
    LPC_UART0->DLL =  var_ulRegValue_u32 & 0xFF;
    LPC_UART0->DLM = (var_ulRegValue_u32 >> 0x08) & 0xFF;

    util_BitClear(LPC_UART0->LCR,(SBIT_DLAB));  // Clear DLAB after setting DLL,DLM
}

/*************************************************************************************************************************/
char uart_RxChar()
{
    char ch;
    while(util_IsBitCleared(LPC_UART0->LSR,SBIT_RDR));  // Wait till the data is received
    ch = LPC_UART0->RBR;                                // Read received data
    return ch;
}
/*******************************************************************************************************************************/
