#include "TM4C123.h"                    // Device header
#include "delay.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "UART.h"

/******** STACK EXCHANGE DEFINITIONS ***********/

#define ANSI_COLOR_RED	   "\x1b[31m"		 
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE 	 "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN 	 "\x1b[36m"
#define ANSI_COLOR_RESET 	 "\x1b[0m"

/***********************************************/

/** @NOTE : DETAILED (RESEARCHED) DEFINITIONS **/  

#define ESC 			"\033"
#define ESC_RESET "\033[00m"

/********** BOLD TEXT DEFINITIONS ***********/ 

#define bRED 	   "[1;31;40m"
#define bGREEN   "[1;32;40m"
#define bYELLOW  "[1;33;40m"
#define bBLUE 	 "[1;34;40m"
#define bMAGENTA "[1;35;40m"
#define bCYAN 	 "[1;36;40m"
#define bWHITE 	 "[1;37;40m"

/******** UNDERLINE TEXT DEFINITIONS ********/ 

#define uRED 	   "[4;31;40m"
#define uGREEN   "[4;32;40m"
#define uYELLOW  "[4;33;40m"
#define uBLUE 	 "[4;34;40m"
#define uMAGENTA "[4;35;40m"
#define uCYAN 	 "[4;36;40m"
#define uWHITE 	 "[4;37;40m"

/******** NORMAL TEXT DEFINITIONS **********/ 

#define RED 	   "[2;31;40m"
#define GREEN    "[2;32;40m"
#define YELLOW   "[2;33;40m"
#define BLUE 	 	 "[2;34;40m"
#define MAGENTA  "[2;35;40m"
#define CYAN 	   "[2;36;40m"
#define WHITE 	 "[2;37;40m"
/**************************************************************************

	@LEARN : color escape codes for once and for all 
		
		\033 ==> Escape code to define color : Informs terminal color data follows																		
																					[1;36;40m	==> 1 -> Style 
																					36 -> color (CYAN)  
																					40m -> Background 
																		
	
  @THEORY : \033 is the ASCII value of ESC key on the keyboard, it is generally used to demark color data to any terminal
						\033 == \x1b == \e ====> >0x1B in HEx
						
						@STYLES : 1 ===> BOLD 
											4 ===> UNDERLINE 
											REST ALL IS NORMAL THIN TEXT 
											
											
						@TEXT>COLORS :  	 31 ==> RED
															 32 ==> GREEN
															 33 ==> YELLOW
															 34 ==> BLUE
															 35 ==> PINK/MAGENTA
															 36 ==> CYAN
															 37 ==> WHITE 
						
						@TEXT>BACKGORUND : 40m ==> BLACK BACKGROUND
															 41m ==> RED BACKGROUND
															 42m ==> GREEN BACKGROUND
															 43m ==> YELLOW BACKGROUND
															 44m ==> BLUE BACKGROUND
															 45m ==> PINK BACKGROUND
															 46m ==> CYAN BACKGROUND
															 47m ==> GREY/WHITE BACKGROUND
							 				    
***************************************************************************/ 									
						
int main()
{
	//char str[] = {97,97,97} ; 
	char str1[] = {98,98,98} ;
	char str[20] ; 
	memset(str,0,20) ; 
	int32_t data = 0 ;  
	init_UART1() ; 
	init_UART0() ; 
                                	//while(1)
	                                	//{
			for(int i = 0 ; i < 10 ; i++)
			{
				UART0_Tx("\r\n") ; 
			                                                                	//UART0_Tx(S) ;
				                                                               //UART0_Tx(RED) ; 
				UART0_Tx(ESC bCYAN "HEY" ESC_RESET) ;
                                                                      //UART0_Tx("HEY") ;
                                                                      //UART0_Tx(E) ; 
                                                                      //UART0_Tx("\r\n") ;
                                                                      //UART0_Tx("\r\n\033[1;31;40mhello\033[00m\r\n") ; 
                                                                      //UART0_Tx("\033[1;31;40mHi\033[00m") ; 
				delayms(100) ;
				                      //  
				                            //}
	}
}






	
