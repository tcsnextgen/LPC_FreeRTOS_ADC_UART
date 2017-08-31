/* $Id::                                                               $
 *
 * Project:	adc: Simple adc driver for MCUXpresso LPC1768
 * File:	adc.c
 * Description:
 * 			Simple ADC driver to read the raw data on LPC1768.
 *
 *****************************************************************************
 *   History
 *   2017.08.29  VER 1.02    Added support for FreeRTOS , tested on MCUXpresso and LPC1768
 *
******************************************************************************/
#include"adc.h"
#include"delay.h"
#include "stdutils.h"
#include "gpio.h"

const adcChannelConfig_st AdcConfig[C_ucMaxAdcChannels]=
{
  { P0_23, PINSEL_FUNC_1}, /* AD0[0] is on P0.23 second alternative function */
  { P0_24, PINSEL_FUNC_1}, /* AD0[1] is on P0.24 second alternative function */
  { P0_25, PINSEL_FUNC_1}, /* AD0[2] is on P0.25 second alternative function */
  { P0_26, PINSEL_FUNC_1}, /* AD0[3] is on P0.26 second alternative function */
  { P1_30, PINSEL_FUNC_3}, /* AD0[4] is on P1.30 third alternative function */
  { P1_31, PINSEL_FUNC_3}, /* AD0[5] is on P1.31 third alternative function */
  { P0_3,  PINSEL_FUNC_2}, /* AD0[6] is on P0.3  third alternative function */
  { P0_2,  PINSEL_FUNC_2}  /* AD0[7] is on P0.2  third alternative function */
};

static uint32_t prvadc_GetPclk(void);

/***************************************************************************************************
                         void vAdcInit()
****************************************************************************************************
 * I/P Arguments: none.
 * Return value	: none
 * description :This function initializes the ADC module.
				Refer adc.h for ADC configuration
***************************************************************************************************/
void vAdcInit()
{
	 uint32_t v_ulPclk;

   /* Enable CLOCK for to controller */
	LPC_SC->PCONP |= (1 << 12);

	v_ulPclk = prvadc_GetPclk();
	v_ulPclk = M_GetClkDivValue(v_ulPclk);

	LPC_ADC->ADCR = ((1<<SBIT_PDN) | v_ulPclk);  // no START, no BURST, ADC normal operation, with selected channel

}



/***************************************************************************************************
                         uint16_t ADC_GetAdcValue(uint8_t v_uadcChannel_u8)
****************************************************************************************************
 * I/P Arguments: uint8_t(channel number).
 * Return value	: uint16_t(12 bit ADC result)
 * description  :This function does the ADC conversion for the Selected Channel
                 and returns the converted 12 bit result
                 The ADC value per bit depends on the resolution of the ADC.
***************************************************************************************************/
uint16_t ADC_GetAdcValue(uint8_t v_ucadcChannel)
 {
	uint16_t v_usadcResult;
    if(v_ucadcChannel < C_ucMaxAdcChannels)
    {
       /* Select channel is with range, COnfigure the channel for ADC and DO the A/D conversion */
	   GPIO_PinFunction(AdcConfig[v_ucadcChannel].pinNumber,AdcConfig[v_ucadcChannel].PinFunSel);
       LPC_ADC->ADCR  = (LPC_ADC->ADCR  & 0xFFFFFF00) | (0x01 << v_ucadcChannel);		/* set the channel */

       DELAY_us(10);		/* allow the channel voltage to stabilize*/

	   util_BitSet(LPC_ADC->ADCR,SBIT_START);			/*Start ADC conversion*/
	   while(util_GetBitStatus(LPC_ADC->ADGDR,SBIT_DONE)==0);	/* wait till conversion completes */

	   v_usadcResult = (LPC_ADC->ADGDR >> SBIT_RESULT) & 0xfff; /*Read the 12bit adc result*/
    }
    else
    {
        /* Channel is out of range, return 0*/
      v_usadcResult = 0;
    }

	return(v_usadcResult);                        			/* Return the 12-bit result */
 }

/***************************************************************************************************
						static uint32_t prvadc_GetPclk( void )
 ***************************************************************************************************
 * I/P Arguments: -
 * Return value	: v_Pclk_u32 in terms of system frequency
 * description  :This function is used to calculate pclock in terms of system frequency.
***************************************************************************************************/

static uint32_t prvadc_GetPclk(void)
{
    uint32_t v_ulAdcPclk,v_ulPclk;
	/**
	   PCLKSELx registers contains the PCLK info for all the clock dependent peripherals.
	   Bit6,Bit7 contains the Uart Clock(ie.UART_PCLK) information.
	   The UART_PCLK and the actual Peripheral Clock(PCLK) is calculated as below.
	   (Refer data sheet for more info)

	   ADC_PCLK    PCLK
	     0x00       SystemFreq/4
		 0x01       SystemFreq
		 0x02       SystemFreq/2
		 0x03       SystemFreq/8
	 **/

	v_ulAdcPclk = (LPC_SC->PCLKSEL0 >> 6) & 0x03;

	switch( v_ulAdcPclk )
	{
	      case 0x00:
			v_ulPclk = SystemCoreClock/4;
			break;
		  case 0x01:
			v_ulPclk = SystemCoreClock;
			break;
		  case 0x02:
			v_ulPclk = SystemCoreClock/2;
			break;
		  case 0x03:
			v_ulPclk = SystemCoreClock/8;
			break;
	}
	return (v_ulPclk);
}
/*********************************************************************************************************************************/
