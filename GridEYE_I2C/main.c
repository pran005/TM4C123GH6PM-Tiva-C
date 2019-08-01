#include <tm4c123gh6pm.h>

#define RED (1<<1) 
#define BLUE (1<<2) 
#define GREEN (1<<3) 


void setSlaveAddress(uint8_t slaveAddress)
{
  //MSA register consists of eight bits: 7 address bits and 1 Receive/Send bit, which determines if the next operation is Receive(1) or Transmit(0)
  //slaveAddress = 0b _001.0000
  I2C0->MSA = (slaveAddress<<1); //Specify the slave address of the master
}

void setRW(uint8_t mode)
{
  //NEXT operation is to Transmit
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
	
	if( (I2C0->MCS & (1<<2) ) != 0)
		GPIOF -> DATA = BLUE ; 
	
  if( (I2C0->MCS & (1<<1) ) != 0)
  {
    if( (I2C0->MCS & (1<<4)) == 1){
			GPIOF-> DATA = RED ; }
    else
    {
      I2C0->MCS = (1<<2);
      while( (I2C0->MCS & (1<<0)) != 0);
    }
  }        
}

int main()
{
	SYSCTL -> RCGCI2C |= (1<<0)  ; 
	SYSCTL -> RCGCGPIO|= (1<<1)  ;
	
	SYSCTL -> RCGCGPIO |= 0x20 ; 
	GPIOF -> DIR |= RED | GREEN | BLUE ; 
	GPIOF -> DEN |= RED | GREEN | BLUE ; 
	
	GPIOB -> AFSEL |= (1<<2) | (1<<3) ; 
	GPIOB -> DEN   |= (1<<2) | (1<<3) ; 
  GPIOB -> ODR   |= (1<<3) ;
	
	I2C0 -> MCR = (1<<4) ;  																			// Enable Master Mode
	I2C0 -> MTPR = 24 ; 																					 // Standard mode 100KHz
	//I2C0 -> MSA  = 0xD2 ; 
	//I2C0 -> MDR  = 0x41 ;
	//I2C0 -> MCS  = 0x07 ;
	setSlaveAddress(0x69) ; 
	setRW(0) ;
	
	while(1) 
	{
		//GPIOF -> DATA |= GREEN ;
		while((I2C0->MCS & 0x01)==1) 
				GPIOF -> DATA = BLUE ;
		GPIOF -> DATA = ~BLUE ;
		writeByte(0x41, (1<<0)|(1<<1)); //BYTE TWO: conditions = RUN and START
  //send IODIR value to make GPIO pins outputs
		writeByte(0x41, (1<<0)|(1<<2));

		
	
	}


}



