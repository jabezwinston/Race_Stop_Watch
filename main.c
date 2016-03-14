/* 
	Author:Jabez Winston C
	14-01-2016
	Simple Race Stop Watch
	
*/
	
//Uses six 7-segment displays in multiplexed form,uses timer1 interrupt to refresh them 
#include<reg52.h>

void Timer_Init();
void timer_100ms();
void Interrupts_Configure();
void display(unsigned long int,bit);	//Display time at 'Current Lap' / 'Prev. Lap'  of 7-segment display
void delay_small();

unsigned long int t=0,t2=0;
bit full=0;
unsigned char sw;
char seven_seg_code[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
					    //Equivalent values for 0, 1, 2, 3, 4, 5, 6, 8, 9 in 7 segment display

void main()
{
	Timer_Init();
	Interrupts_Configure();
	while(1);						//Wait indefinitely for any interrupt to occur
}


void timer_100ms() interrupt 1        //Invoked every 50 ms
{
	full=~full;
	if(full)
		t++;				//Incremented by 1 every 100ms
}

void Timer_Init()
{
	TMOD=0x11;		//Timer0 in mode 1 and Timer1 in mode 1 
	
	TH0=0x4B;
	TL0=0xFE;
	
	TH1=0xDC;
	TL1=0x00;
	TR1=1;			//Start Timer1
}

void Interrupts_Configure()
{
	EA=1;		//Enable Interrupts
	ET0=1;		//Enable Timer0 interrupt
	ET1=1;		//Enable Timer1 interrupt
	EX0=1;		//Enable External Interrupt 0
	EX1=1;		//Enable External Interrupt 1
	IT0=1;		//Make External interrupt 0 as edge triggered
	IT1=1;		//Make External interrupt 1 as edge triggered
}

void Timer_Start_Stop() interrupt 0    //External interrupt 0 (P3.2)
{
	TR0=~TR0;		//Start Timer if Stopped , Stop Timer if running when START button is pushed
}

void Timer_Lap() interrupt 2	//External interrupt 0 (P3.3)
{
	t2=t;						//Save Laptime
	t=0;						//Reset timer

}
void Refresh() interrupt 3		//Timer1 Interrupt 
{
	TF1=0;						//Clear Timer Overflow Flag
	TR1=0;						//Stop Timer1
	TH1=0x00;
	TL1=0x00;
	display(t,0);				//Display running time in 'Current lap' portion of 7 seg
	display(t2,1);				//Display running time in 'Prev lap' portion of 7 seg
	TR1=1;
	
}

 void display(unsigned int t_copy,bit s)
{
	unsigned char j,n;
	
	if(!s)
	for(j=0;t_copy!=0;j++)
	{
	n=t_copy%10;									//Break a number to individual  digits
	P2=(1<<j);										//Switch from one 7 seg display to other
		
		if(j==1)
			P1=~((1<<7)+seven_seg_code[n]);			// (1<<7) for display number with decimal point 
		else
			P1=~seven_seg_code[n];					
		
	delay_small();
	t_copy=t_copy/10;								//Break a number to individual  digits
	}
	
	else
		
	for(j=3;t_copy!=0;j++)
	{
	n=t_copy%10;									//Break a number to individual  digits
	P2=(1<<j);										//Switch from one 7 seg display to other
		
		if(j==4)
			P1=~((1<<7)+seven_seg_code[n]);			// (1<<7) for decimal point
		else
			P1=~seven_seg_code[n];
		
	delay_small();
	t_copy=t_copy/10;								//Break a number to individual  digits
	}

	
}

void delay_small()
 {
	 int k;
	 for(k=0;k<1000;k++);
 }