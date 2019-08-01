#include <tm4c123gh6pm.h>
#include <string.h>
#include <stdlib.h>

void init_UART1(void)
{
	SYSCTL->RCGCUART |= (1<<1) ;                   // Enable the UART1 module using the RCGCUART register
  SYSCTL->RCGCGPIO |= (1<<1) ; 
	
	GPIOB->AFSEL |= (1<<1)|(1<<0); 
	GPIOB->PCTL  |= (1<<0)|(1<<4);
	GPIOB->DEN 	 |= (1<<0)|(1<<1);
	
	UART1->CTL &= ~(1<<0);
	UART1->IBRD = 325;                                    // Write the integer portion of the BRD to the UARTIRD register
  UART1->FBRD = 33;                                     // Write the fractional portion of the BRD to the UARTFBRD register
  UART1->LCRH = (0x3<<5);                               // 8-bit, no parity, 1 stop bit
  
  UART1->CC = 0x0;
  
  UART1->CTL = (1<<0)|(1<<8)|(1<<9);                    // UART1 Enable, Transmit Enable, Recieve Enable
}


void init_UART0(void)
{
	SYSCTL->RCGCUART |= (1<<0);                    // Enable the UART0 and UART1 module using the RCGCUART register
  SYSCTL->RCGCGPIO |= (1<<0);                    // Enables the Port A and Port B GPIO via the RCGCGPIO register
  
  GPIOA->AFSEL |= (1<<1)|(1<<0);                         // enables the alternate function on pin PA0 & PA1
  //GPIOB->AFSEL |= (1<<1)|(1<<0);                         // enables the alternate function on pin PB0 & PB1
  

  GPIOA->PCTL = (1<<0)|(1<<4);                          // Configure the GPIOPCTL register to select UART0 in PA0 and PA1
  //GPIOB->PCTL = (1<<0)|(1<<4);                          // Configure the GPIOPCTL register to select UART1 in PB0 and PB1
  
  GPIOA->DEN |= (1<<0)|(1<<1);                           // Enable the digital functionality in PA0 and PA1   
  //GPIOB->DEN |= (1<<0)|(1<<1);                           // Enable the digital functionality in PB0 and PB1 
  
  UART0->CTL &= ~(1<<0);                                // Disable UART0 by clearing UARTEN bit in the UARTCTL register
  //UART1->CTL &= ~(1<<0);                                // Disable UART1 by clearing UARTEN bit in the UARTCTL register
  
  
  UART0->IBRD = 325;                                    // Write the integer portion of the BRD to the UARTIRD register     
  //UART1->IBRD = 325;                                    // Write the integer portion of the BRD to the UARTIRD register
  
  UART0->FBRD = 33;                                     // Write the fractional portion of the BRD to the UARTFBRD register
  //UART1->FBRD = 33;                                     // Write the fractional portion of the BRD to the UARTFBRD register
  
  UART0->LCRH = (0x3<<5);                               // 8-bit, no parity, 1 stop bit
  //UART1->LCRH = (0x3<<5);                               // 8-bit, no parity, 1 stop bit
  
  
  
  UART0->CC = 0x0;                                      // Configure the UART clock source as system clock
  //UART1->CC = 0x0;
  
  UART0->CTL = (1<<0)|(1<<8)|(1<<9);                    // UART0 Enable, Transmit Enable, Recieve Enable
  //UART1->CTL = (1<<0)|(1<<8)|(1<<9);                    // UART1 Enable, Transmit Enable, Recieve Enable

}


void UART0_Tx(char c)                                                  // function to print user's input.
{
  while((UART0->FR &(1<<5)) != 0);                                      // waits the program till the previous transmission was completed
  UART0->DR =c;                                                         // value of UART0 DATA REGISTER is equal to c
}

void UART0_Tx(int32_t m)  
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
	UART0_Tx(str[i]) ;
}

void UART0_Tx(float d)
{
	int i = int(d) ; 
	float mantissa = d-i ;
	if((d<0) && (i==0)) 
	UART0_Tx('-') ; 
	UART0_Tx(i) ; 
	UART0_Tx('.') ; 
	 if(d>0)	
  UART0_Tx((int)(mantissa*100000)) ; 
  else if(d<0) 
 	UART0_Tx((int)(-mantissa*100000)) ;	
}

void UART0_Tx(char * string)                                         // function to print a predefined string
{
  while(*string)
  {
    UART0_Tx(*(string++));                                             // printChar is called with every increment in string pointer
  }
}

void UART1_Tx(char c)                                                  // function to print user's input.
{
  while((UART1->FR &(1<<5)) != 0);                                      // waits the program till the previous transmission was completed
  UART1->DR =c;                                                         // value of UART0 DATA REGISTER is equal to c
}

void UART1_Tx(int32_t m)  
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
	UART1_Tx(str[i]) ;
}

void UART1_Tx(float d)
{
	int i = int(d) ; 
	float mantissa = d-i ;
	if((d<0) && (i==0)) 
	UART1_Tx('-') ; 
	UART1_Tx(i) ; 
	UART1_Tx('.') ; 
	 if(d>0)	
  UART1_Tx((int)(mantissa*100000)) ; 
  else if(d<0) 
 	UART1_Tx((int)(-mantissa*100000)) ;	
}

void UART1_Tx(char * string)                                         // function to print a predefined string
{
  while(*string)
  {
    UART1_Tx(*(string++));                                             // printChar is called with every increment in string pointer
  }
}
