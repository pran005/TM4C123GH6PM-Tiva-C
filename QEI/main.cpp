#include <tm4c123gh6pm.h>
#include "lcd.h" 


int main()
{
	SystemInit() ; 
	
	SYSCTL -> RCGCQEI |= (1<<0) ; 
	SYSCTL -> RCGCGPIO |= (1<<3) ;  

	GPIOD -> LOCK = 0x4C4F434B ; 
	GPIOD -> CR |= (1<<7) ; 
	GPIOD -> LOCK = 0x01 ; 
	GPIOD -> DEN |= (1<<7) | (1<<6) ; 
	GPIOD -> AFSEL |= (1<<6) | (1<<7) ; 
	GPIOD -> PCTL |= (1<<25)| (1<<26) | (1<<29) | (1<<30) ; 
	
	init_lcd() ; 
	QEI0->MAXPOS = 0xFFFFFFFF ; 
	QEI0->LOAD = 50000 ; 
	QEI0->CTL |= (1<<0) | (1<<3) | (1<<5) | (1<<13) ;
	
	while(1) 
	{
		clr_lcd() ;
		lcd((char *)"RPM : ") ;
		float rpm = ((float)1250000 * (float) QEI0->SPEED ) /(float)(QEI0->LOAD) ; 
		lcd(rpm) ; 
		cmd(0xc0) ;
		lcd((char *)"Ticks : ") ; 
		lcd(QEI0->POS/2400) ;		
		delayms(100) ;
	}
}
