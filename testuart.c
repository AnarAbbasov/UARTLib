/*
 * File:   testuart.c
 * Author: anar
 *
 * Created on December 16, 2023, 5:04 PM
 */

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#include <xc.h>
#include <string.h>
#define _XTAL_FREQ 4000000
                   

#define BAUDRATE 9600

void InitUART(void)
{
    TRISB2=0;
    TRISB1=1;
    
    BRGH=1;
    TX9D=0;
    SPBRG=((_XTAL_FREQ/16)/BAUDRATE);
    SYNC=0;
    SPEN=1;
    CREN=1;
    SREN=0;
    TXIE=0;
    RCIE=1;
    TX9=0;
    RX9=0;
    TXEN=0;
    
    TXEN=1;

    
}



void SendByteSerially(unsigned char Byte)
{
    while (!TXIF);
    TXREG=Byte;
  
      
      
}


void SendString(char * str)
{
    for (int i=0;i<=strlen(str);i++)
            SendByteSerially(str[i]);
            
    
        
}

typedef struct CMD{

    char cmd[5];
    int position;
} commanddata;

commanddata cmdata;
void __interrupt() isr(void)
{
  if(RCIF)
  {
   RB5=1;
    if(cmdata.position>5){
       cmdata.position=0;}
    else{
   cmdata.cmd[cmdata.position]=RCREG;
  
   cmdata.position++;
    }
  
  SendByteSerially(RCREG);
  
     RB5=0;
  
   
  }
}




void main(void) {
   TRISB=0;
  InitUART();
  cmdata.position=0;
   RB5=0;
    GIE=1;
  PEIE=1;
  SendString("XUY OS V 0.1.2\n");
	while( 1 )
    {
      //  RB5=0;
   
         __delay_ms(1000);
         if (cmdata.position==5)
            SendByteSerially('F');
      
   
     
       SendString(cmdata.cmd);
        SendByteSerially('\n');
   //RB5=1;
     __delay_ms(1000);
  
     //__delay_ms(1000);
   
    
    }
		
    return;
}
