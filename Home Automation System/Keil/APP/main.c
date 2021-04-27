//ESOS SCE Component
//Automation System
//Group no: D19
//Group Members : 1) Vaibhav Deshpande
//								2) Ashutosh Sharma
//								3) Vishwajit Ghadge


#include "config.h" 													// Master Header include
#include "stdlib.h" 													// standard lib include
#include <stdint.h>
#include "stdio.h" 														// standard lib include
#include<LPC21xx.H> 													// device header include

#define bit(x) (1<<x)

#define	TaskStkLengh 64												//Define the Task's stack length 

static OS_EVENT *semaphore;   //ECB (Event Control Block) creates pointer to ECB
int flag=0; // used as flag in task-2
unsigned char pass[4] = "5555";   						//Set your Password 
void delay_sys()
{
    unsigned int temp, ct;
    for(ct=0; ct<30; ct++) {
        for(temp=0; temp < 65000; temp++);
    }
}
unsigned int range=0,i;

/* ---------- Keypad ------*/                // Assigned the keypad
#define c1 (IOPIN1&1<<20) // Defining input pins for keypad columns,pin 20,21 & 22
#define c2 (IOPIN1&1<<21)
#define c3 (IOPIN1&1<<22)
 
unsigned char r_loc,c_loc;
unsigned char key[4][3]={"123","456","789","*0#"}; //Creating 2D array for Keypad characters
unsigned char keypad(void);


OS_STK	TaskStk1 [TaskStkLengh];								//Define the Task1 stack
OS_STK	TaskStk2 [TaskStkLengh];								//Define the Task2 stack 
OS_STK	TaskStk3 [TaskStkLengh];								//Define the Task3 stack 

/* ----- Keypad Function ----*/
unsigned char keypad()
{
    IO1PIN &= ~(0xff<<16);
    IO1PIN |= 0xf0<<16;    //rows as output and columns as input
 
    while(c1 && c2 && c3); //checks if key is pressed
    while(!c1 || !c2 || !c3) { // if key is pressed
        if(!c1 && c2 && c3)     c_loc=0;
        else if(c1 && !c2 && c3)    c_loc=1;
        else if(c1 && c2 && !c3)    c_loc=2;
 
        IO1CLR = 1<<16;  //setting row 0 to LOW
        IO1SET = 0x0e<<16; //setting other rows to high
        if(!c1 || !c2 || !c3) {
            r_loc=0;
            break;
        }
 
        IO1CLR = 1<<17;
        IO1SET = 0x0d<<16;
        if(!c1 || !c2 || !c3) {
            r_loc=1;
            break;
        }
        
        IO1CLR = 1<<18;
        IO1SET = 0x0b<<16;
        if(!c1 || !c2 || !c3) {
            r_loc=2;
            break;
        }
 
        IO1CLR = 1<<19;
        IO1SET = 0x07<<16;
        if(!c1 || !c2 || !c3) {
            r_loc=3;
            break;
        }
    }
    while(!c1 || !c2 || !c3);
    return (key[r_loc][c_loc]);
}
//---------------------------------------------------------------------------

/* ----- LCD Functions -------*/
 
void delay_ms(uint16_t j) 				/* Function for delay in milliseconds */
{
    uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<6000; x++);    		/* loop to generate 1 millisecond delay with Cclk = 60MHz */
	}
}

void LCD_CMD(char command)
{
	IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((command & 0xF0)<<8) ); /* Upper nibble of command */
	IO0SET = 0x00000040; /* EN = 1 */
	IO0CLR = 0x00000030; /* RS = 0, RW = 0 */
	delay_ms(5);
	IO0CLR = 0x00000040; /* EN = 0, RS and RW unchanged(i.e. RS = RW = 0)	 */
	delay_ms(5);
	IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((command & 0x0F)<<12) ); /* Lower nibble of command */
	IO0SET = 0x00000040; /* EN = 1 */
	IO0CLR = 0x00000030; /* RS = 0, RW = 0 */
	delay_ms(5);
	IO0CLR = 0x00000040; /* EN = 0, RS and RW unchanged(i.e. RS = RW = 0)	 */
	delay_ms(5);
	
}

void LCD_INIT(void)
{
	IO0DIR = 0x0000FFF0; /* P0.12 to P0.15 LCD Data. P0.4,5,6 as RS RW and EN */
	delay_ms(20);
	LCD_CMD(0x02);  /* Initialize lcd in 4-bit mode */
	LCD_CMD(0x28);  /* 2 lines */
	LCD_CMD(0x0C);   /* Display on cursor off */
	LCD_CMD(0x06);  /* Auto increment cursor */
	LCD_CMD(0x01);   /* Display clear */
	LCD_CMD(0x80);  /* First line first position */
}

void LCD_STRING (char* msg)
{
	uint8_t i=0;
	while(msg[i]!=0)
	{
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((msg[i] & 0xF0)<<8) );
		IO0SET = 0x00000050; /* RS = 1, EN = 1 */
		IO0CLR = 0x00000020; /* RW = 0 */
		delay_ms(2);
		IO0CLR = 0x00000040; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW = 0) */
		delay_ms(5);
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((msg[i] & 0x0F)<<12) );
		IO0SET = 0x00000050; /* RS = 1, EN = 1 */
		IO0CLR = 0x00000020; /* RW = 0 */
		delay_ms(2);
		IO0CLR = 0x00000040; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW = 0) */
		delay_ms(5);
		i++;
	}
}

void LCD_CHAR (char msg)
{
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((msg & 0xF0)<<8) );
		IO0SET = 0x00000050; /* RS = 1, EN = 1 */
		IO0CLR = 0x00000020; /* RW = 0 */
		delay_ms(2);
		IO0CLR = 0x00000040; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW = 0) */
		delay_ms(5);
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((msg & 0x0F)<<12) );
		IO0SET = 0x00000050; /* RS = 1, EN = 1 */
		IO0CLR = 0x00000020; /* RW = 0 */
		delay_ms(2);
		IO0CLR = 0x00000040; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW = 0) */
		delay_ms(5);
}

//----------------------------------------------------------------------------


void Task1(void *pdata) 											// TASK-1
{
  char p;
	unsigned char q[4];
  int count;
	VPBDIV=0x01;                 // PCLK = 60MHz
  IO1DIR |= 0x0f<<16;          // Keypad interfacing at PORT 1
  IO0DIR |= 0xf00fc;           // LCD interfacing at PORT 0
  LCD_INIT();
 
     
        LCD_CMD(0x80);  //First Line Initialization
        LCD_STRING(" Hello");
	      delay_sys(1500);   //1500ms
        //LCD_CMD(0xc5);
				LCD_CMD(0x01); //Clear display screen
				LCD_STRING("Enter Password");
	      delay_sys(1500);
			  LCD_CMD(0x01); //Clear display screen
        for(count=0; count <4; count++) {
            q[count] = keypad();
            LCD_CHAR('*');
        }
        
        if ( (pass[0] == q[0]) &&                  
                (pass[1] == q[1]) && 
                (pass[2] == q[2]) && 
                (pass[3] == q[3]) ) {                      // Checking Password
            LCD_CMD(0x01); //Clear display screen
						LCD_CMD(0xc0); // Row 1 Col 0
            LCD_STRING(" Access Granted");
            delay_sys(1500);
						LCD_CMD(0x01); //Clear display screen
            //stop();
            LCD_CMD(0xc0);
            LCD_STRING("Welcome");
            delay_sys();
						LCD_CMD(0x01); //Clear display screen			
									
									
						semaphore=OSSemCreate(1);               //Creating Semaphore
						if(semaphore!=NULL)  // If password is correct & semaphore is created
						{
						LCD_CMD(0xc0);
						LCD_STRING("Semaphore Created");
						delay_sys();
						LCD_CMD(0x01); //Clear display screen
						OSSemPend(semaphore,1,&p);						//Semaphore Acquired
						LCD_CMD(0xc0);
						LCD_STRING("Sem Acquired");
						delay_sys();
						LCD_CMD(0x01); //Clear display screen
						}
						else // If password is correct but semaphore is not created
						{
						LCD_CMD(0xc0);
						LCD_STRING("Sem Not Created");
						delay_sys();
						LCD_CMD(0x01); //Clear display screen
						}			
            //stop();
        } else {  // If password entered is wrong
					  flag++;
            LCD_CMD(0xc0);
					  LCD_CMD(0x01); //Clear display screen
            LCD_STRING(" Wrong Password");
            delay_sys();
					  LCD_CMD(0x01); //Clear display screen
					  LCD_CMD(0xc0);
            LCD_STRING("Access Denied");
            delay_sys();
					  LCD_CMD(0x01); //Clear display screen
        }
        LCD_CMD(0x01);		
	OSTimeDly(800);
}		

void Task2(void *pdata)  // Task-2 starts
{
	if(flag==0)  // If password entered is correct and semaphore is created 
	{
	pdata=pdata; 
	
	PINSEL2 = 0;
	IODIR1 = 0xFF000000;  
	LCD_CMD(0xc0);
	LCD_STRING("Lights ON");
	
	
	
		IOSET1 = 0xFF000000;
		delay_sys(2500);
		IOCLR1 = 0xFF000000;
		delay_sys(2500);
	OSTimeDly(1000);
	}
}




void Task3(void *pdata)				// Task-3 Starts				 
{
	char val_j[3];
	OSSemPost(semaphore);																	//Semaphore Released
	LCD_INIT();
	LCD_CMD(0x80);
	LCD_STRING("Sem Released");
	delay_sys();
	LCD_CMD(0x01); //Clear display screen
	LCD_CMD(0xc0);
	LCD_STRING("Thankyou!");
	delay_sys();
	LCD_CMD(0x01); //Clear display screen
	OSTimeDly(1500);		
}

int main (void)
{
	
	TargetInit ();
	OSInit();	// Initialize the OS internal data structure for configured services in os_cfg.h 
	
	
	OSTaskCreate (Task1,(void *)0, &TaskStk1[TaskStkLengh - 1], 3);
	OSTaskCreate (Task2,(void *)0, &TaskStk2[TaskStkLengh - 1], 4);	// Task creation for OS 
	OSTaskCreate (Task3,(void *)0, &TaskStk3[TaskStkLengh - 1], 5);
	OSStart (); 																										// Start Multitasking
	return 0;															
}
