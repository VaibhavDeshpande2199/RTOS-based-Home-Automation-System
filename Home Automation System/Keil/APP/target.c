

/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			target.c
** Last modified Date:	2004-09-17
** Last Version:		1.0
** Descriptions:		header file of the specific codes for LPC2100 target boards
**						Every project should include a copy of this file, user may modify it as needed
**------------------------------------------------------------------------------------------------------
** Created by:			Chenmingji
** Created date:		2004-02-02
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			Chenmingji
** Modified date:		2004-09-17
** Version:				1.01
** Descriptions:		Renewed the template, added more compiler supports 
**
**------------------------------------------------------------------------------------------------------
** Modified by: 
** Modified date:
** Version:	
** Descriptions: 
**
********************************************************************************************************/

#define IN_TARGET
#include "config.h"
#include<LPC21XX.H>

/*********************************************************************************************************
** Function name:			IRQ_Exception
**
** Descriptions:			interrupt exceptional handler , change it as needed
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void IRQ_Exception(void)
{
    while(1);                   // change it to your code  ???????????
}

/*********************************************************************************************************
** Function name:			FIQ_Exception
**
** Descriptions:			Fast interrupt exceptional handler , change it as needed
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void FIQ_Exception(void)
{
    while(1);                   // change it to your code  ???????????
}
/*********************************************************************************************************
** Function name:			Timer0_Exception
**
** Descriptions:			Timer0 interrupt service function
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void Timer0_Exception(void)
{
    T0IR = 0x01;
    VICVectAddr = 0;            //interrupt close ???????????
    OSTimeTick();
}

/*********************************************************************************************************
** Function name:			Timer0Init
**
** Descriptions:			Initialize the Time0
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void Timer0Init(void)
{
    T0IR = 0xffffffff;
    T0TC = 0;
    T0TCR = 0x01;
    T0MCR = 0x03;
    T0MR0 = (Fpclk / OS_TICKS_PER_SEC);
 }
/*********************************************************************************************************
** Function name:			VICInit
**
** Descriptions:			Initialize the Interrupt Vevtor Controller
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
   void VICInit(void)
{
    extern void IRQ_Handler(void);
    extern void Timer0_Handler(void);

    VICIntEnClr = 0xffffffff;
    VICDefVectAddr = (uint32)IRQ_Handler;

    VICVectAddr0 = (uint32)Timer0_Handler;
    VICVectCntl0 = (0x20 | 0x04);
    VICIntEnable = 1 << 4;
 }

/*********************************************************************************************************
** Function name:			TargetInit
**
** Descriptions:			Initialize the target board; it is called in a necessary place, change it as 
**							needed
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
 void TargetInit(void)
{
    OS_ENTER_CRITICAL();
    srand((uint32) TargetInit);
    VICInit();

    Timer0Init();
    OS_EXIT_CRITICAL();
}

  

void delay() // Delay for flashing interval
 {
 int i;
 for(i=0;i<=1000000;i++);
 }
 
void LED_FLASH()
{
	 IODIR1=0XFF;   // set direction of port pins 1.16 to 1.23 as output

 	 IOSET1=0XFF; // setting pins for third nibble ( 1.20-1.23) as 1
	 delay();
 	 IOCLR1=0xFF; // setting pins for third nibble ( 1.20-1.23) as 0
 	 delay();
	
	//OSTimeDlyHMSM(0,0,1,0);
	
	}

void LED_FLASH_FourthNibble()
{
 //int i;
	 IODIR1=0x00FF0000;			// set direction of port pins 1.16 to 1.23 as output							   

   IOSET1=0x000F0000;  // setting pins for fourth nibble ( 1.16-1.19) as 1
	 delay();
 	 IOCLR1=0x000F0000;  // setting pins for fourth nibble ( 1.16-1.19) as 0 
 	 delay();
	 //OSTimeDlyHMSM(0,0,3,0);
}
 
 /*********************************************************************************************************
** ADC PROGRAM
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
unsigned int ADC_Read(unsigned char channel)
{
	static unsigned int ad1_data;
	AD0CR = 0x01200300 | (1<<channel);	//Set Burst mode to 0, CLK_DIV, Channels, 10bit ADC/11 cycles and start ADC   							
	while(!(AD0GDR & 0x80000000));			   	//Check DONE bit
	ad1_data = (AD0GDR & 0x0000FFC0)>>6;		//Assign ADC result to ad1_data n display in mV		 
	return ad1_data;
}
/*********************************************************************************************************
** Function name:			InitialiseUART0
**
** Descriptions:			Initialize the Uart0
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void InitialiseUART0(uint32 bps)
{  	
    uint16 Fdiv;
    
    PINSEL0 = (PINSEL0 & 0xfffffff0) | 0x05;    /* Select the pins for Uart ?????UART0 */

    U0LCR = 0x80;                               /* Enable to access the frequenc regecter ??????????? */
    Fdiv = (Fpclk / 16) / bps;                  /* Set the baudrate????? */
    U0DLM = Fdiv / 256;							
	U0DLL = Fdiv % 256;						
    U0LCR = 0x03;                               /* Disable to access the frequenc regecter ??????????? */
                                                /* set to 8,1,n ????8,1,n */
	U0IER = 0x00;                               /* Disable interrupt???? */
    U0FCR = 0x00;                               /* initial FIFO ???FIFO */
} 



/*********************************************************************************************************
** LCD PROGRAM
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
/*
 ============================================================================
  Interface		:
 		D0		-	P0.16
		D1		-	P0.17
		D2		-	P0.18
		D3		-	P0.19
		D4		-	P0.20
		D5		-	P0.21
		D6		-	P0.22
		D7		-	P0.23 
		
		EN		-	P0.15
		RS		-	P0.14
		RW		-	P0.20
 ============================================================================
 */



/*********************************************************************************************************
** Function name:			TargetResetInit
**
** Descriptions:			Initialize the target 
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void TargetResetInit(void)
{
#ifdef __DEBUG_RAM    
    MEMMAP = 0x2;                   //remap
#endif

#ifdef __DEBUG_FLASH    
    MEMMAP = 0x1;                   //remap
#endif

#ifdef __IN_CHIP    
    MEMMAP = 0x1;                   //remap
#endif

    PINSEL0 = (PINSEL0 & 0xFFFF0000) | 0x05 | 0x50;

/* ????????? */
/* Set system timers for each component */
    PLLCON = 1;
#if (Fpclk / (Fcclk / 4)) == 1
    VPBDIV = 0;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
    VPBDIV = 2;
#endif
#if (Fpclk / (Fcclk / 4)) == 4
    VPBDIV = 1;
#endif

#if (Fcco / Fcclk) == 2
    PLLCFG = ((Fcclk / Fosc) - 1) | (0 << 5);
#endif
#if (Fcco / Fcclk) == 4
    PLLCFG = ((Fcclk / Fosc) - 1) | (1 << 5);
#endif
#if (Fcco / Fcclk) == 8
    PLLCFG = ((Fcclk / Fosc) - 1) | (2 << 5);
#endif
#if (Fcco / Fcclk) == 16
    PLLCFG = ((Fcclk / Fosc) - 1) | (3 << 5);
#endif
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
    while((PLLSTAT & (1 << 10)) == 0);
    PLLCON = 3;
    PLLFEED = 0xaa;
    PLLFEED = 0x55;

/* ????????? */
/* Set memory accelerater module*/
    MAMCR = 0;
#if Fcclk < 20000000
    MAMTIM = 1;
#else
#if Fcclk < 40000000
    MAMTIM = 2;
#else
    MAMTIM = 3;
#endif
#endif
    MAMCR = 2;

/* ????? */
/* initialize UART*/
    InitialiseUART0(115200);

/* ?????? */
/* initialize RTC*/
    CCR = 1;
    PREINT = Fpclk / 32768 - 1;
    PREFRAC = Fpclk - (Fpclk / 32768) * 32768;
    YEAR = 2003;
    MONTH = 6;
    DOM = 2;
    
/* initialize VIC*/
    VICIntEnClr = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;
    T0IR = 0xffffffff;
    T0TCR = 0X02;
}


