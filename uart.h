/***********************************************************************
 * $Id::                                                               $
 *
 * Project:	uart: Simple UART echo for MCUXpresso LPC1768
 * File:	uart.c
 *
 ***********************************************************************
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


#ifndef __UART_H 
#define __UART_H

#define SBIT_WordLenght    0x00u
#define SBIT_DLAB          0x07u
#define SBIT_FIFO          0x00u
#define SBIT_RxFIFO        0x01u
#define SBIT_TxFIFO        0x02u

#define SBIT_RDR           0x00u
#define SBIT_THRE 0x05u

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define BUFSIZE		0x40

#define LPC_PINCON_PINSEL0_AND 0x000000F0
#define LPC_PINCON_PINSEL0_OR 0x00000050


void vUARTSend(uint32_t v_ulportNum, uint8_t *v_ucBufferPtr, uint32_t v_ulLength);
void vuart_TxChar(char v_ch);
void vuart_init(uint32_t v_ulbaudrate);
char uart_RxChar();
#endif /* end __UART_H */
/*****************************************************************************/
