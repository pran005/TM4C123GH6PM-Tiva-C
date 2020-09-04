/*      Author  : Pranjal
				Project : Distributed Data Acquisition
				Driver  : SSI0_v1
				Specific to 1.8" TFT (SPI) based on ST7735
																																										TFT 
																																								____________
				               PA6-----------------------------------------------------|DC					|
											 PA7-----------------------------------------------------|RST					|
																																							 |						|
				SSI0 																																	 |						|	
		  __________																															 |						|	
		 | SSI0_FSS |- PA3 - CS----------------------------------------------------|CS					|
		 | SSI0_CLK |- PA2 - CLK---------------------------------------------------|CLK					|
		 | SSI0_Tx  |- PA5 - SDA---------------------------------------------------|SDA					|
		 | SSI0_Rx  |- PA4 (unused for ST7735 in v1, configure if needed)					 |____________|
	   |__________|
		 
*/


#include "TM4C123.h"                    // Device header
#include "tft_st7735.h" 


static uint8_t ColStart, RowStart; // some displays need this changed

static int16_t _width = ST7735_TFTWIDTH;
static int16_t _height = ST7735_TFTHEIGHT;
uint32_t Start_X=0; 
uint32_t Start_Y=0; 
uint16_t StTextColor = YELLOW;
static uint8_t ColStart, RowStart; 
int32_t PlotMax,PlotMin,X;        
int32_t PlotRange; 
int TimeIndex;               
int32_t PlotMax, PlotMin, PlotRange;  
uint16_t PlotBGColor; 


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

void init_SSI0(void)
{
	
	SYSCTL -> RCGCSSI |= (1<<0) ; 					 														    //Enable SSI0 Clock	
	SYSCTL -> RCGCGPIO |= (1<<0) ;									  					   			   //Enable GPIOA Clock
	while (!(SYSCTL -> PRGPIO & 0x01)) ; 											  				  // wait for GPIOA to be peripheral ready 

	GPIOA -> AFSEL |= (1<<2) | (1<<3) | (1<<5) ; 	 
	GPIOA -> PCTL |= (1<<9) | (1<<13) | (1<<21) ; 					  				 // Pin Mux selection for SSI0  
	GPIOA -> AMSEL &= ~(0x2C) ; 													  			  	// Disable Analog mode select for 2,3,5  
	GPIOA -> DEN |= 0x2C ;										  				  			   	 // Digital Enable for 2,3,5 
	
	
	SSI0 -> CR1 &= ~(1<<1) ; 															  			// Disable SSI0 
	SSI0 -> CR1 &= ~(1<<2) ; 					   					 				   		 // Master of Puppets
	
	SSI0 -> CC = (SSI0 -> CC & ~(0X0F)) + 0x00 ;		   		     // SYSCLK => Avoid modifying reserved bits  
	SSI0 -> CPSR = (SSI0 -> CPSR & ~(0xFF))+ 0x0A ;			  		// 5MHz CLK = 5000000/10 ; BR=SysClk/(CPSDVSR * (1 + SCR))
  
	SSI0 -> CR0 = 0x00000007 ; 												  		// SCR = 0 ; SPHA = 0 ; SPOL = 0 ; Sample on rising , shift on falling ; Freescale SPI Mode 
	SSI0 -> CR1 |= (1<<1) ; 													 		 // Enable SSI  
}


void static tft_cmd(uint8_t cmd) 
{
  while((SSI0->SR & (0x10))==0x10);
  GPIOA -> DATA  &= CMD ;
  SSI0->DR = cmd;                        
  while((SSI0->SR & (0x10))==0x10);
}


void static tft_data(uint8_t data) 
{
  while((SSI0->SR & (1<<1))==0)	;  								// wait till Tx FIFO FULL 
  GPIOA -> DATA |= DAT ;
  SSI0->DR = data;                        
	while((SSI0->SR & (1<<4))==0x10) ;	
		
}


void config_tft(void)
{
	SYSCTL -> RCGCSSI |= (1<<0) ; 					 														    //Enable SSI0 Clock	
	SYSCTL -> RCGCGPIO |= (1<<0) ;									  					   			   //Enable GPIOA Clock
	while (!(SYSCTL -> PRGPIO & 0x01)) ; 											  				  // wait for GPIOA to be peripheral ready 
	
	GPIOA->DIR |= 0xC8;             
  GPIOA->AFSEL &= ~0xC8;          
  GPIOA->DEN |= 0xC8; 
	GPIOA->AMSEL&= ~0xC8; 
	
	GPIOA -> DATA &= ~CS ; 
	GPIOA -> DATA |= RST ; 
	delayms(500) ; 
	GPIOA -> DATA &= ~RST ; 
	delayms(500) ; 
	GPIOA -> DATA |= RST ; 
	delayms(500) ; 
	
	init_SSI0() ; 
	
	
	GPIOA -> DATA &= ~CS ; 									// CS=0     
  GPIOA -> DATA &= ~RST ;   						 // RST=0 

  delayms(10);      

  GPIOA -> DATA |= RST ;      	     // RST=1
  delayms(10);   

   
   tft_cmd(0x11); 

   delayms(120);      

   tft_cmd(0x3A); 							//Set Color mode
   tft_data(0x05); 						 //16 bits color 
	 tft_cmd(0x36);							//Set Memory Data Access Control 
	 tft_data(0x14);					 //
	 tft_cmd(0x29);						//Display on

}


void static setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

  tft_cmd(ST7735_CASET); 									// Column addr set
  tft_data(0x00);
  tft_data(x0+ColStart);     			 			// XSTART
  tft_data(0x00);
  tft_data(x1+ColStart);		     			// XEND

  tft_cmd(ST7735_RASET); 						// Row addr set
  tft_data(0x00);
  tft_data(y0+RowStart);    	 	  // YSTART
  tft_data(0x00);
  tft_data(y1+RowStart);     		// YEND

  tft_cmd(ST7735_RAMWR); 			// write to RAM
}


void tft_Fill_rectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
  uint8_t hi = color >> 8, lo = color;

  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  setAddrWindow(x, y, x+w-1, y+h-1);

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      tft_data(hi);
      tft_data(lo);
    }
  }
}


void tft_SetCursor(uint32_t newX, uint32_t newY)
	{
  if((newX > 20) || (newY > 15)){       
    return;                           
  }
  Start_X = newX;
  Start_Y = newY;
}

void ST7735_FillScreen(uint16_t color) 
{
  tft_Fill_rectangle(0, 0, _width, _height, color);  
}
void init_tft(void)
{
		config_tft();
	 tft_SetCursor(0,0);
	 StTextColor = YELLOW;
   ST7735_FillScreen(BLACK);                 // set screen to black

}
void static tft_WriteColor(uint16_t color) 
{
  tft_data((uint8_t)(color >> 8));
  tft_data((uint8_t)color);
}

void tft_DrawPixel(int16_t x, int16_t y, uint16_t color) 
{

  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

  setAddrWindow(x,y,x,y);

  tft_WriteColor(color);
}

void tft_DrawVLine(int16_t x, int16_t y, int16_t h, uint16_t color)	
{
  uint8_t hi = color >> 8, lo = color;

  if((x >= _width) || (y >= _height)) return;
  if((y+h-1) >= _height) h = _height-y;
  setAddrWindow(x, y, x, y+h-1);

  while (h--) {
    tft_data(hi);
    tft_data(lo);
  }
}

void tft_DrawHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  uint8_t hi = color >> 8, lo = color;

  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  setAddrWindow(x, y, x+w-1, y);

  while (w--) {
    tft_data(hi);
    tft_data(lo);
  }
}
void tft_DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size)
{
  uint8_t line; 
  int32_t i, j;
  if((x >= _width)            || 
     (y >= _height)           || 
     ((x + 5 * size - 1) < 0) || 
     ((y + 8 * size - 1) < 0))   
    return;

  for (i=0; i<6; i++ ) {
    if (i == 5)
      line = 0x0;
    else
      line = Font[(c*5)+i];
    for (j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) 
          tft_DrawPixel(x+i, y+j, textColor);
        else {  
          tft_Fill_rectangle(x+(i*size), y+(j*size), size, size, textColor);
        }
      } else if (bgColor != textColor) {
        if (size == 1) 
          tft_DrawPixel(x+i, y+j, bgColor);
        else {  
          tft_Fill_rectangle(x+i*size, y+j*size, size, size, bgColor);
        }
      }
      line >>= 1;
    }
  }
}

void tft_DrawChar(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size)
{
  uint8_t line; 
  int32_t col, row, i, j;
  if(((x + 5*size - 1) >= _width)  || 
     ((y + 8*size - 1) >= _height) || 
     ((x + 5*size - 1) < 0)        || 
     ((y + 8*size - 1) < 0)){        
    return;
  }

  setAddrWindow(x, y, x+6*size-1, y+8*size-1);

  line = 0x01;        
  for(row=0; row<8; row=row+1){
    for(i=0; i<size; i=i+1){
      for(col=0; col<5; col=col+1){
        if(Font[(c*5)+col]&line){
          for(j=0; j<size; j=j+1){
            tft_WriteColor(textColor);
          }
        } else{
          for(j=0; j<size; j=j+1){
            tft_WriteColor(bgColor);
          }
        }
      }
      for(j=0; j<size; j=j+1){
        tft_WriteColor(bgColor);
      }
    }
    line = line<<1;   // move up to the next row
  }
}
uint16_t tft_Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
}

uint32_t tft_DrawString(uint16_t x, uint16_t y, char *pt, int16_t textColor)
{
  uint32_t count = 0;
  if(y>15) return 0;
  while(*pt){
    tft_DrawCharS(x*6, y*10, *pt, textColor, BLACK, 1);
    pt++;
    x = x+1;
    if(x>20) return count;  
    count++;
  }
  return count;  
}


void tft_Plo_clr(int32_t PlotMin, int32_t PlotMax){
  tft_Fill_rectangle(0, 32, 128, 128, tft_Color565(228,228,228));
  if(PlotMax>PlotMin){
    PlotMax = PlotMax;
    PlotMin = PlotMin;
    PlotRange = PlotMax-PlotMin;
  } else{
    PlotMax = PlotMin;
    PlotMin = PlotMax;
    PlotRange = PlotMax-PlotMin;
  }
  X = 0;
}

void tft_PlotStaticPoint(int32_t y)
{
		int32_t j;
  if(y<PlotMin) y=PlotMin;
  if(y>PlotMax) y=PlotMax;
  j = 32+(127*(PlotMax-y))/PlotRange;
  if(j<32) j = 32;
  if(j>159) j = 159;
  tft_DrawPixel(X,   j,  BLUE);
  tft_DrawPixel(X+1, j,  BLUE);
  tft_DrawPixel(X,   j+1,BLUE);
  tft_DrawPixel(X+1, j+1,BLUE);
}

void tft_PlotPoint(int32_t data1, uint16_t color1)
{
  data1 = ((data1 - PlotMin)*100)/PlotRange;
  if(data1 > 90){
    data1 = 90;
    color1 = RED;
  }
  if(data1 < 0){
    data1 = 0;
    color1 = RED;
  }
  tft_DrawPixel(TimeIndex + 11, 116 - data1, color1);
  tft_DrawPixel(TimeIndex + 11, 115 - data1, color1);
}
void tft_PlotIncrement(void)
{  
  TimeIndex = TimeIndex + 1;
  if(TimeIndex > 99){
    TimeIndex = 0;
  }
  tft_DrawVLine(TimeIndex + 11, 17, 100, PlotBGColor);
}

int32_t lastj=0;

void tft_PlotLine(int32_t y)
{
	int32_t i,j;
  if(y<PlotMin) y=PlotMin;
  if(y>PlotMax) y=PlotMax;
 
  j = 32+(127*(PlotMax-y))/PlotRange;
  if(j < 32) j = 32;
  if(j > 159) j = 159;
  if(lastj < 32) lastj = j;
  if(lastj > 159) lastj = j;
  if(lastj < j){
    for(i = lastj+1; i<=j ; i++){
      tft_DrawPixel(X,   i,   BLUE) ;
      tft_DrawPixel(X+1, i,   BLUE) ;
    }
  }else if(lastj > j){
    for(i = j; i<lastj ; i++){
      tft_DrawPixel(X,   i,   BLUE) ;
      tft_DrawPixel(X+1, i,   BLUE) ;
    }
  }else{
    tft_DrawPixel(X,   j,   BLUE) ;
    tft_DrawPixel(X+1, j,   BLUE) ;
  }
  lastj = j;
}

void tft_PlotBar(int32_t y)
{
int32_t j;
  if(y<PlotMin) y=PlotMin;
  if(y>PlotMax) y=PlotMax;

  j = 32+(127*(PlotMax-y))/PlotRange;
  tft_DrawVLine(X, j, 159-j,BLACK);

}

void tft_PlotNext(void)
{
  if(X==127){
    X = 0;
  } else{
    X++;
  }
}



void tft_drawaxes(uint16_t axisColor, uint16_t bgColor, char *xLabel,char *yLabel1, uint16_t label1Color, char *yLabel2, uint16_t label2Color,int32_t PlotMax, int32_t PlotMin)
{
  int i;
  PlotMax = PlotMax;
  PlotMin = PlotMin;
  PlotRange = PlotMax - PlotMin;
  TimeIndex = 0;
  PlotBGColor = bgColor;
  tft_Fill_rectangle(0, 0, 128, 160, bgColor);
  tft_DrawHLine(10, 140, 101, axisColor);
  tft_DrawVLine(10, 17, 124, axisColor);
  for(i=20; i<=110; i=i+10){
    tft_DrawPixel(i, 141, axisColor);
  }
  for(i=17; i<120; i=i+10){
    tft_DrawPixel(9, i, axisColor);
  }
  i = 50;
  while((*xLabel) && (i < 100)){
    tft_DrawChar(i, 145, *xLabel, axisColor, bgColor, 1);
    i = i + 6;
    xLabel++;
  }
  if(*yLabel2){ // two labels
    i = 26;
    while((*yLabel2) && (i < 50)){
      tft_DrawChar(0, i, *yLabel2, label2Color, bgColor, 1);
      i = i + 8;
      yLabel2++;
    }
    i = 82;
  }else{ // one label
    i = 42;
  }
  while((*yLabel1) && (i < 120)){
   tft_DrawChar(0, i, *yLabel1, label1Color, bgColor, 1);
    i = i + 8;
    yLabel1++;
  }
}

