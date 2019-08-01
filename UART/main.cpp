#include <tm4c123gh6pm.h>
#include "UART.h"
#include "lcd.h"

int main()
{
	init_lcd()  ; 
	init_UART0() ; 
		int i = 0 ; 
	while(1)
	{
	
		//for (int i=0 ; i<64 ; i++)
		clr_lcd() ; 
		lcd((char*)"SEE") ; 
		UART0_Tx(i++) ; 
		//delayms(100) ; 
		//UART0_Tx((char*)"\r\n") ; 
		
			//delayms(1000) ; 
	}
}
