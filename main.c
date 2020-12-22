#include <LPC17xx.H>                    	
#include <stdio.h>
#include <math.h>
#include "lcd.h"
#include "string.h"
#include "sys.h"
#include "hardware_conf.h"
#include "ov7670.h"	

u8 buf1[32];
u16 Camera_Data = 0,i=0;
extern u8 VsyncCnt;

/*
*********************************************************************************************************
* Description: 	Delay function
* Arguments  : 	The time of ms
* Returns    : 	None
*********************************************************************************************************
*/

static __inline void Delay1us(unsigned int delay1msdata)
{
unsigned int i,j,k;
for(i=0;i<delay1msdata;i++)
for(j=0;j<1;j++)
for(k=0;k<10;k++)
;
}

static __inline void Camera_RESET(void)
{
FIFO_WRST =0;
DelayNS(1);
FIFO_WRST =1;
DelayNS(1);
FIFO_RRST =0;
DelayNS(1);
FIFO_RCLK =0;	
DelayNS(1);
FIFO_RCLK =1;
DelayNS(1);
FIFO_RCLK =0;	
DelayNS(1);
FIFO_RCLK =1;
DelayNS(200);
FIFO_RRST =1;
}
static __inline void Camera_Display(void)
{
int x=0,y=0;

for(y=0;y<321;y++)
{
for(x = 0; x <240;)	//320*240	
{
FIFO_RCLK =0;
DelayNS(1);
FIFO_RCLK =1;
DelayNS(1);
Camera_Data = (FIFO_DATA_PIN<<8)&0xff00 ;	           

FIFO_RCLK =0;
DelayNS(1);
FIFO_RCLK =1;
DelayNS(1);

Camera_Data |= (FIFO_DATA_PIN)&0x00ff ;

LCD_SetCursor(x,y);		
LCD_WriteRAM_Prepare();	
LCD_WR_DATA(Camera_Data); 
x=x+1;
}
}

__nop();
FIFO_RRST =0;														
__nop();
for(i=0;i<128;i++)		
{
FIFO_RCLK =0;				
__nop();
FIFO_RCLK =1;
__nop();
}
FIFO_RRST =1;
__nop();
FIFO_WRST =0;
__nop();

DelayNS(200);		
FIFO_WRST =1;			
__nop();
VsyncCnt = 0;				   
__nop();
}
/*
*********************************************************************************************************
* Description: 	Main function
* Arguments  : 	None
* Returns    : 	None
*********************************************************************************************************
*/
int main (void)                       
{
SystemInit();
LCD_Init();
LCD_Clear(GRED);
LCD_ShowString(40,35,200,16,16,"Z A S M A Z");	
LCD_ShowString(70,55,200,16,16,"    1397     ");	
showimage(95 ,100);													

Camera_Configuration();		
Camera_RESET();


while (1)
{
if(OV7660_init()) break;

}
LCD_ShowString(40,75,200,16,16,"OV7670 Successfully Connected");			
Delay1us(10000000);



OV7660_config_window(272,0,240,320);

while (1)
{

if(VsyncCnt == 2)
{			
Camera_Display();			
}
}
}
