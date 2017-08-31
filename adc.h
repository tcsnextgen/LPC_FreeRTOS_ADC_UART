#ifndef _ADC_H
#define _ADC_H

#include "LPC17xx.h"
#include "stdutils.h"
#include "gpio.h"

/***************************************************************************************************
                             Commonly used ADC macros/Constants
***************************************************************************************************/
#define M_AdcClockFreq		1000000		/* set to 1Mhz */

#define SBIT_BURST   16u
#define SBIT_START	 24u
#define SBIT_PDN	 21u
#define SBIT_EDGE	 27u

#define SBIT_DONE	 31u
#define SBIT_RESULT	  4u


#define M_GetClkDivValue(pclk)  (( pclk  / M_AdcClockFreq - 1u ) << 8u )



/***************************************************************************************************
                            Constants and Structures
***************************************************************************************************/
#define C_ucMaxAdcChannels   8u

typedef struct
{
  uint8_t pinNumber;
  uint8_t PinFunSel;
}adcChannelConfig_st;
/**************************************************************************************************/

/***************************************************************************************************
                             Function Prototypes
***************************************************************************************************/
void vAdcInit(void);
uint16_t ADC_GetAdcValue(uint8_t v_ucadcChannel);

/**************************************************************************************************/


#endif
