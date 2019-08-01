#include "TM4C123.h"                    // Device header
#include "lcd.h" 
#include <stdlib.h> 

/* delay.h remaining */ 

int main() 
{

	//SystemInit() ; 
	

	init_lcd() ; 
	while(1) 
	{
		clr_lcd() ;
		cmd(0x80) ; 
		lcd((char *)" Mini Project-II") ; 
		//lcd_data('a') ; 
		delayms(1000) ;
		//clr_lcd() ;
		cmd(0xc0) ; 
		lcd((char *)"  I'm Node 1 ") ;
    delayms(1000) ; 
		
		clr_lcd() ;
		cmd(0X80) ; 
		lcd((char *)"I'll Monitor ") ;
		
		delayms(1000) ;
		cmd(0xc0) ; 
		lcd((char *)"Machine Health") ;
		delayms(1000) ;
		
	}
		



}
