#ifndef _lcd_h
#define _lcd_h

#include "TM4C123.h"                    // Device header
#include <string.h>
#include <stdint.h>
#define clr_lcd() cmd(0x01) 

void cmd (char command) ; 
void lcd_data (char data) ; 
void init_lcd() ; 
void lcd(int m) ;
void lcd(char *string) ;
void delayms(uint32_t delay) ; 

void lcd(double d) ; 

#endif 

