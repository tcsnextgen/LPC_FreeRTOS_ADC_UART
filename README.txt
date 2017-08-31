************************************************************************************************************
              UART and ADC driver integration with FreeRTOS on LPC1768 Platform
************************************************************************************************************

Description : LPC1768 has ADC channles and 3 UART ports. LM35 Temparature sensor is interfaced through ADC channel1.
	      ADC driver is used to read the temparature and send it through UART port0 using UART driver.
			  

Source Files : adc.c,adc.h,uart.c and uart.h 

Test Source File : main.c

API List :  
    1) vAdcInit() : Initialize the ADC Channel.
    2) vuart_init() : Initialize the UART port.
    3) ADC_GetAdcValue(): Read the raw adc data.
    4) vUart_TxChar() : Write the data through uart port.

Build Procedure : 
    1) Launch the MCUXpresso IDE and import the existing workspace directory FreeRTOSv9.0.0/FreeRTOS/Demo/CORTEX_LPC1768_GCC_RedSuite
    2) Add both adc.c,adc.h,uart.c and uart.h into the src directory in MCUXpresso.
    3) Build the source.
Test Procedure: 
    1) Replace the main.c with one provided by us.
    2) Build the source and flash the images on to the board using Segger J-Link debugger.
    3) Observe the data sent through UART port 0 using LED blinks on the board.
       YELLOW LED : 25C to 30C
       BLUE LED : below 25C or above 30C
	
************************************************************************************************************
************************************************************************************************************

