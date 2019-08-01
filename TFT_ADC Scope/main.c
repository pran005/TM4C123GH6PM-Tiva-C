#include "TM4C123.h"                    // Device header
#include "tft_st7735.h"

void ADC0_Init(void)
{
  /* enable clocks */
    SYSCTL->RCGCGPIO |= 0x10;   /* enable clock to GPIOE (AIN0 is on PE3) */
    SYSCTL->RCGCADC |= 1;       /* enable clock to ADC0 */
    
    /* initialize PE3 for AIN0 input  */
    GPIOE->AFSEL |= 8;       /* enable alternate function */
    GPIOE->DEN &= ~8;        /* disable digital function */
    GPIOE->AMSEL |= 8;       /* enable analog function */
   
    /* initialize ADC0 */
    ADC0->ACTSS &= ~8;        /* disable SS3 during configuration */
    ADC0->EMUX &= ~0xF000;    /* software trigger conversion */
    ADC0->SSMUX3 = 0;         /* get input from channel 0 */
    ADC0->SSCTL3 |= 6;        /* take one sample at a time, set flag at 1st sample */
    ADC0->ACTSS |= 8;       /* enable ADC0 sequencer 3 */
}


	volatile uint32_t _result;

 uint32_t ADC0_getResult(void)
{

	 ADC0->PSSI |= 8;        /* start a conversion sequence 3 */
   while((ADC0->RIS & 8) == 0){} ;   /* wait for conversion complete */
   _result = ADC0->SSFIFO3; /* read conversion result */
   ADC0->ISC = 8;          /* clear completion flag */
	
	 return _result;
}

volatile uint32_t value ; 

int main()
{
	init_tft() ; 
	ADC0_Init() ; 
	tft_drawaxes(AXISCOLOR, BGCOLOR, "Time", "ADC", LIGHTCOLOR, "", 0, 4096, 0);
	tft_DrawString(1,0,"ADC PLOT : ",BLUE) ;
	tft_DrawString(14,0,"SWAG",LIGHTCOLOR) ;
	int i = 0 , j= 0 ; 
	while(1)
	{
		/*for(i=0;i<8;i++)
		{
			for(j=0;j<8;j++)
			tft_Fill_rectangle(i*15,j*15, 15, 15, RED+i*20+j*20) ; 
		
		}*/
		value = ADC0_getResult() ; 
		tft_PlotPoint(value,GREEN) ; 
		tft_PlotIncrement() ; 
		delayms(10) ; 
	}
	
}
