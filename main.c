/*
    FreeRTOS V9.0.1 - Copyright (C) 2017 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


 /*
  * Created a task to read the temperature data using ADC
  * Obtained temperature data is sent using UART0 port of LPC1768
 */

/* Standard includes. */
#include "stdio.h"
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "adc.h"
#include "uart.h"
#include "delay.h"

#define LPC_GPIO2_FIOSET_NORMAL 0x88888888
#define LPC_GPIO2_FIOSET_CAUTION 0x11111111
#define LPC_GPIO2_FIOCLR 0xffffffff
#define LPC_PINCON_PINSEL4 0x000000
#define LPC_GPIO2_FIODIR 0xffffffff
#define BAUDRATE 115200

void vTemperatureUartTask(void);
void vLedblink(unsigned int, unsigned int);
void vDelay(uint32_t );

int main( void )
{
	void SystemInit();
	vAdcInit();
	vuart_init(BAUDRATE);
	xTaskCreate( vTemperatureUartTask, "Temprature_uart_task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	vTaskStartScheduler();


	/* Will only get here if there was insufficient memory to create the idle
	task.  The idle task is created within vTaskStartScheduler(). */
	for( ;; );
}

/*****************************************************************************************************************************/

void vLedblink(unsigned int time, unsigned int status)
{
	if (status==1)
	{
		LPC_GPIO2->FIOSET = LPC_GPIO2_FIOSET_NORMAL;
	}
	else
	{
		LPC_GPIO2->FIOSET = LPC_GPIO2_FIOSET_CAUTION;
	}
    vDelay(time);
    LPC_GPIO2->FIOCLR = LPC_GPIO2_FIOCLR;
    vDelay(time);
}

/*******************************************************************************************************************************/

void vTemperatureUartTask(void)
{
        volatile uint16_t ustemp_raw =0, ustemp_final=0;
        LPC_PINCON->PINSEL4 = LPC_PINCON_PINSEL4;  	//Configure the PORT2 Pins as GPIO;
        LPC_GPIO2->FIODIR = LPC_GPIO2_FIODIR; 		//Configure the PORT2 pins as OUTPUT;
        while(1)
        {
                ustemp_raw = ADC_GetAdcValue(2); /* Read Temp value connect to AD2(P0_25) */
                /* Converting the raw adc value to equivalent temperature with 3.3v as ADC reference using 12bit resolution.
                Step size of ADC= (3.3v/2^12)= 3.3/4096 = 0.0008056640625 = 0.0806mv
                For every degree celsius the Lm35 provides 10mv voltage change.
                1 degree celsius = 10mv = 10mv/0.0806mv = 12.41 units
                Hence the Raw ADC value can be divided by 12.41 to get equivalent temp*/

                ustemp_final = ustemp_raw/12.41;
                if(ustemp_final>25 && ustemp_final<30)
                {
                	vLedblink(100,1);
                }
                else
                {
                	vLedblink(100,0);
                }
                LPC_UART0->THR=ustemp_final;

        }
}

/**********************************************************************************************************************************/

void vDelay(uint32_t v_ms)
{
    uint32_t ultmp,ultmp1;

    for(ultmp=0;ultmp<v_ms;ultmp++)
    {
        for(ultmp1=0;ultmp1<1000;ultmp1++);
    }
}

/***********************************************************************************************************************************/
