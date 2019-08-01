/*****************************POINTERS TO TOGGLE GPIOS**********************************/

/* 														  Tiva TM4C123G																					*/

//BASE ADDR   , OFFSET 
//0x400F.E000 , 0x608 -> RCGC
//0x4002.5000 , 0x400 -> GPIODIR 
//0x4002.5000 , 0x51C -> GPIODEN , data enable
//0x4002.5000 , 0x000 -> GPIODATA 

int main()
{
	unsigned int *ptr ; 
	int i = 0 ;
	
	ptr = (unsigned int *)0x400FE608U ; 
	*ptr = 0x20U ; 
	
	ptr = (unsigned int *)0x40025400U ;
	*ptr = 0x0EU ;   
	ptr = (unsigned int *)0x4002551CU ;
	*ptr = 0x0EU ;  
	ptr = (unsigned int *)0x400253FCU ;
	while(1)
	{
	*ptr = 0x0EU ;
	
	for(i=0;i<2000000;i++) ;
	*ptr = 0x00U ;
	for(i=0;i<2000000;i++) ;
	}
	
	
}
