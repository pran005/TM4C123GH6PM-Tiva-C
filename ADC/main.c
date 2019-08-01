//VTSENS = 2.7 - ((TEMP + 55) / 75) 

#include <tm4c123gh6pm.h>
#include <stdint.h>
#include "tft_st7735.h"

volatile uint32_t ADC_result ; 

void init_ADC0(void)
{
	SYSCTL -> RCGCADC |= 0x01 ;								
	SYSCTL -> RCGCGPIO 	|= (1<<4) ; 
	
	GPIOE -> AFSEL |= 0x08 ; 
	GPIOE -> DEN &= ~0x08 ; 
	GPIOE -> AMSEL |= 0x08 ;

	ADC0 -> ACTSS &= ~(0x08) ; 
	ADC0 -> EMUX &= ~(0xF000) ; 
  ADC0 -> SSMUX3 = 0x00 ; 
	ADC0 -> SSCTL3 |= (1<<1) | (1<<2) ; 
	ADC0 -> ACTSS |= (0x08) ;
} 	


uint32_t ADC0_result() 
{
	ADC0 -> PSSI |= (0x08) ; 
	
	while((ADC0 -> RIS & 0x08)==0) ; 
	uint32_t result = (ADC0 -> SSFIFO3) ; 
	ADC0 -> ISC = 8 ; 
	return  result ; 	
}


int main()
{
	init_ADC0() ; 
	init_tft() ; 
	tft_drawaxes(AXISCOLOR,BGCOLOR,"TIME","ADC",LIGHTCOLOR,"",BLACK,4096,0) ; 
	tft_DrawString(1,0,"ADC PLOT : ",BLUE) ;
	tft_DrawString(12,0,"GAS",LIGHTCOLOR) ;
		
	while(1) 
	{
		
		ADC_result = ADC0_result() ; 
		tft_PlotPoint(ADC_result,GREEN) ; 
		tft_PlotIncrement() ; 
		delayms(10) ; 
	
	}



}



