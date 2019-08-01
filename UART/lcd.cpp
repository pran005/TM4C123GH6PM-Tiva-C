#include "TM4C123.h"                    // Device header
#include <string.h>

/*
  
							    PF2 GND PF3 		   PC4	PC5 PC6 PC7			 
                                                             |   |   |		            |   |   |   |
							     Rs  RW EN  	 	   DB4 DB5 DB6 DB7    
							  _____________________________________________________________
							 |  _________________________________________________________  |
							 | |							     | |
							 | | 	TM4C123GH6PM					     | | 
							 | |	Mini Project	II				     | | 
							 | |________________________________________________________ | |
							 |_____________________________________________________________|
							 
							 
*/

void delayms(uint32_t delay) 
{
	int i = 0 ; 
	SYSCTL -> RCGCTIMER |= 0x01 ; 
	TIMER0 -> CTL = 0x00 ; 
	TIMER0 -> CFG = 0x04 ; 
	TIMER0 -> TAMR = 0x02 ; 
	TIMER0 -> TAILR = 50000 - 1 ;
	TIMER0 -> ICR = 0x01 ; 
  TIMER0 -> CTL = 0x01 ;
	
	for(i=0 ; i<delay ; i++)
	{
		//i++ ; 
		while(!(TIMER0->RIS & 0x01)) ; 
		TIMER0 -> ICR = 0x01 ; 
	}
}

void cmd (char command)
{
	uint8_t a = (command>>4) & 0x0f ; 
	uint8_t b = (command & 0x0f) ;
	
	GPIOF -> DATA &= ~(1<<2) ; 
	GPIOF -> DATA |= (1<<3) ;
	GPIOC -> DATA = a<<4 ; 
	delayms(5) ; 
	GPIOF -> DATA &= ~(1<<3) ;
	
	GPIOF -> DATA |= (1<<3) ;
	GPIOC -> DATA = b<<4 ;  
	delayms(5) ;  
	GPIOF -> DATA &= ~(1<<3) ;
		
}

void lcd_data (char data)
{
	//uint8_t a = data & 0x0f ;
	uint8_t a = (data>>4) & 0x0f ; 
	uint8_t b = (data & 0x0f) ;
	 
	GPIOF -> DATA |= (1<<3) | (1<<2) ;
	GPIOC -> DATA = a<<4 ;  
	delayms(5) ;  
	GPIOF -> DATA &= ~(1<<3) ;
	
	GPIOF -> DATA |= (1<<3) ;
	GPIOC -> DATA = b<<4 ;  
	delayms(5) ; 
	GPIOF -> DATA &= ~(1<<3) ;
	GPIOF -> DATA &= ~(1<<2) ;	
}

void init_lcd()
{
	SYSCTL -> RCGCGPIO |= (1<<2) | (1<<5) ; 						/* PORTF & C CLK enable */ 
	
	GPIOC  -> DIR |= (1<<4) | (1<<5) | (1<<6) | (1<<7) ; 	// 0xF0 ; 
  GPIOC	 -> DEN |= (1<<4) | (1<<5) | (1<<6) | (1<<7) ;
	
	GPIOF  -> DIR |= (1<<2) | (1<<3) ; 
	GPIOF  -> DEN |= (1<<2) | (1<<3) ; 
	
	cmd(0x02) ; 
	cmd(0x28) ; 
	cmd(0x0E) ; 
	cmd(0x06) ; 
	cmd(0x01) ; 
	cmd(0x80) ;
	cmd(0x01) ; 
  delayms(10) ; 
}

void lcd(char *string)
{
	while(*string)
		lcd_data(*string++) ; 
}

void lcd(int m) 
{
	uint8_t i=0, sign=0,r,p  ;
	char str[100] ;	
	if(m==0)
	{ str[i] = '0' ;
		i++ ;
		str[i]= '\0' ;
	}
	if(m<0)
	{ m=-m ;
		sign=1 ;
	}
	while(m!=0)
	{
		r=m%10 ;
		str[i++] = r+ '0' ;
		m=m/10 ;
	}
	if(sign==1)
	str[i++] = '-' ;
	str[i++] = '\0' ;
	p=strlen(str) ; 
	for(int i=p-1 ; i>=0 ; i--)
	lcd_data(str[i]) ;
}


void lcd(double d)
{
	int i = int(d) ; 
	double mantissa = d-i ;
	if((d<0) && (i==0)) 
		lcd_data('-') ; 
	lcd(i) ; 
	lcd_data('.') ; 
	 if(d>0)	
  lcd((int)(mantissa*100000)) ; 
  else if(d<0) 
 	lcd((int)(-mantissa*100000)) ;	
}

