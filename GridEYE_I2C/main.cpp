#include <tm4c123gh6pm.h>
#include <math.h>
#include "lcd.h" 
#include "UART.h"

#define RED (1<<1) 
#define BLUE (1<<2) 
#define GREEN (1<<3) 



void setSlaveAddress(uint8_t slaveAddress)
{
  I2C0->MSA = (slaveAddress<<1); //Specify the slave address of the master
}

void setRW(uint8_t mode)
{
 
  if(mode == 0)
  {
    I2C0->MSA &= ~(1<<0); //Clear bit, transmitting data
  }
  else
  {
    I2C0->MSA |= (1<<0); //set bit, receiving data
  }
}

void writeByte(uint8_t dataByte, uint8_t conditions)
{
  //This byte contains the data transferred during a transaction
  I2C0->MDR = dataByte;
  I2C0->MCS = conditions;
  
  while( (I2C0->MCS & (1<<0)) != 0);
	
  if( (I2C0->MCS & (1<<1) ) != 0)
  {
    if( (I2C0->MCS & (1<<4)) == 1){
			}
    else
    {
      I2C0->MCS = (1<<2);
      while( (I2C0->MCS & (1<<0)) != 0);
    }
  }        
}

uint8_t getByte(uint8_t conditions)
{
  //This byte contains the data transferred during a transaction
  //I2C0->MDR = dataByte;
  I2C0->MCS = conditions;
  
  while( (I2C0->MCS & (1<<0)) != 0);
	
  if( (I2C0->MCS & (1<<1) ) != 0)
  {
    if( (I2C0->MCS & (1<<2)) != 0)
			{
			lcd((char*)"Error") ; 
				}
    /*else
    {
      I2C0->MCS = (1<<2);
      while( (I2C0->MCS & (1<<0)) != 0);
    }*/
  }	
	uint8_t data = I2C0->MDR ;
	return data; 
	//clr_lcd() ; 
	//lcd(data) ;
	
  /*if( (I2C0->MCS & (1<<1) ) != 0)
  {
    if( (I2C0->MCS & (1<<4)) == 1){
			}
    else
    {
      I2C0->MCS = (1<<2);
      while( (I2C0->MCS & (1<<0)) != 0);
    }
  } */       
}


int main()
{
	SYSCTL -> RCGCI2C |= (1<<0)  ; 
	SYSCTL -> RCGCGPIO|= (1<<1)  ;
	
	SYSCTL -> RCGCGPIO |= 0x20 ; 
	GPIOF -> DIR |= RED | GREEN | BLUE ; 
	GPIOF -> DEN |= RED | GREEN | BLUE ; 
	init_lcd() ; 
	
	GPIOB -> AFSEL |= (1<<2) | (1<<3) ; 
	GPIOB -> DEN   |= (1<<2) | (1<<3) ; 
  GPIOB -> ODR   |= (1<<3) ;
	
	I2C0 -> MCR = (1<<4) ;  																			// Enable Master Mode
	I2C0 -> MTPR = 24 ; 																					 // Standard mode 100KHz
	//I2C0 -> MSA  = 0xD2 ; 
	//I2C0 -> MDR  = 0x41 ;
	//I2C0 -> MCS  = 0x07 ;
	//setSlaveAddress(0x69) ; 
	//setRW(0) ;
	init_UART0() ; 
	delayms(100) ; 
		
	
	while(1) 
	{
	
		uint16_t data[64] ; 
		//while((I2C0->MCS & 0x01)==1) 
		/*
		setSlaveAddress(0x69) ; 
		setRW(0) ;	
		writeByte(0x81, (1<<0)|(1<<1)); //BYTE TWO: conditions = RUN and START
		//writeByte(0x41, (1<<0)|(1<<2)); //BYTE TWO: conditions = RUN and START
		
		setSlaveAddress(0x69) ; 
		setRW(1) ;	
		uint16_t data = (getByte((1<<0)|(1<<2)|(1<<1))) << 8 ;
 

		setSlaveAddress(0x69) ; 
		setRW(0) ;	
		writeByte(0x80, (1<<0)|(1<<1)); //BYTE TWO: conditions = RUN and START
		
		setSlaveAddress(0x69) ; 
		setRW(1) ;	
		data = data | (getByte((1<<0)|(1<<2)|(1<<1))) ;
		*/
		
		int8_t count = 0  ;
		
		
		while(count<64)
		{
			
		setSlaveAddress(0x69) ; 
		setRW(0) ;	
		writeByte((0x81+(2*count)), (1<<0)|(1<<1)); //BYTE TWO: conditions = RUN and START
		//writeByte(0x41, (1<<0)|(1<<2)); //BYTE TWO: conditions = RUN and START
		
		setSlaveAddress(0x69) ; 
		setRW(1) ;	
		data[count] = (getByte((1<<0)|(1<<2)|(1<<1))) << 8 ;
 

		setSlaveAddress(0x69) ; 
		setRW(0) ;	
		writeByte((0x80+2*(count)), (1<<0)|(1<<1)); //BYTE TWO: conditions = RUN and START
		
		setSlaveAddress(0x69) ; 
		setRW(1) ;	
		data[count] = data[count] | (getByte((1<<0)|(1<<2)|(1<<1))) ;	
		
			
		float temp=0 ; 
		double cnt = 0 ; 
		while(data[count])
			{		
				if((data[count] & 0x01)) 
				temp += 0.25*pow(2,cnt); 
	 
				data[count] = data[count]>>1 ; 
				++cnt ; 
			}
				
		if(count==0)
			UART0_Tx((char *)"\r\n{") ; 
			
		UART0_Tx(temp) ; 
		UART0_Tx((char * )"C, ") ;
		
		count++ ; 
		if(count%10==0)
			UART0_Tx((char *)"\r\n") ;
		if(count==64)
			UART0_Tx((char *)"}\r\n") ; 
		}
		
		
		//clr_lcd() ; 
		//lcd(data) ; 
		delayms(1000) ; 
	}


}



