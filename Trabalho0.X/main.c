/*
 *  Trabalho 0   
 */

#include "config_bits.h"

#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "../CKCommon/UART/uart.h" 

#define SYSCLK  80000000L // System clock frequency, in Hz
#define PBCLOCK 40000000L // Peripheral Bus Clock frequency, in Hz

void init(void)
{
	/* Configure RA3 and RC1 (led 4 and led 5) as Output */
    TRISAbits.TRISA3 = 0;
    TRISCbits.TRISC1 = 0;
  
	/*define initial values for RA3 and RC1*/
    PORTAbits.RA3 = 0;
    PORTCbits.RC1 = 0;

}

void initTimer(){
      // Set timer
    T2CONbits.ON = 0;    // Stop timer
    IFS0bits.T2IF=0;     // Reset interrupt flag
    IPC2bits.T2IP=5;     //set interrupt priority (1..7) *** Make sure it matches iplx in isr declaration ***
    IEC0bits.T2IE = 0;   // Disable T2 interrupts -> 0 corresponds polling
    // Timer period configuration
    T2CONbits.TCKPS = 7; //Divide by 256 pre-scaler - Timer 2 contains this prescaler: 1,2,4,8,16,36,64,256 (correspond to the number 7)
    T2CONbits.T32 = 0;   // 16 bit timer operation
    PR2=0xFFFF;          // Period set to approx 0.42 secs (40MHz/256 = 156kHz; 1/156k*65000 ~ 0.41)
    TMR2=0;
      
    T2CONbits.TON=1;     // Start the timer

}

void initADC(){
    
    // Initialize ADC module
    // Polling mode, AN0 as input
    // Generic part
    AD1CON1bits.SSRC = 7; // Internal counter ends sampling and starts conversion
    AD1CON1bits.CLRASAM = 1; //Stop conversion when 1st A/D converter interrupt is generated and clears ASAM bit automatically
    AD1CON1bits.FORM = 0; // Integer 16 bit output format
    AD1CON2bits.VCFG = 0; // VR+=AVdd; VR-=AVss
    AD1CON2bits.SMPI = 0; // Number (+1) of consecutive conversions, stored in ADC1BUF0...ADCBUF{SMPI}
    AD1CON3bits.ADRC = 1; // ADC uses internal RC clock
    AD1CON3bits.SAMC = 16; // Sample time is 16TAD ( TAD = 100ns)
    // Set AN0 as input
    AD1CHSbits.CH0SA = 0; // Select AN0 as input for A/D converter
    TRISBbits.TRISB0 = 1; // Set AN0 to input mode
    AD1PCFGbits.PCFG0 = 0; // Set AN0 to analog mode
    // Enable module
    AD1CON1bits.ON = 1; // Enable A/D module (This must be the ***last instruction of configuration phase***)

}

/* 
 */
int main(int argc, char** argv) {
    init();
     
     // Variable declarations;
    float res; // Sampled volatge

    // Init UART and redirect tdin/stdot/stderr to UART
    if(UartInit(PBCLOCK, 115200) != UART_SUCCESS) {
        PORTAbits.RA3 = 1; // If Led active error initializing UART
        while(1);
    }
    __XC_UART = 1; /* Redirect stdin/stdout/stderr to UART1*/
    
    // Disable JTAG interface as it uses a few ADC ports
    DDPCONbits.JTAGEN = 0;

    // Loop
    while (1){
	  while(IFS0bits.T2IF == 0);      //Preso até ter a flag a 1              <-
               IFS0bits.T2IF = 0;          //Baixar a bandeira do timer            <-       
          PORTAbits.RA3 = ! PORTAbits.RA3; 

	 // Get one sample
        IFS1bits.AD1IF = 0; // Reset interrupt flag
        AD1CON1bits.ASAM = 1; // Start conversion
        while (IFS1bits.AD1IF == 0); // Wait fo EOC

        // Convert to 0..3.3V 
        res = (ADC1BUF0 * 3.3) / 1023;

        // Output result
        printf("Voltage: %f\n\r",res);
        //printf("Temp:%f",(res-2.7315)/.01); /   

    }
     
    return (EXIT_SUCCESS);
}


