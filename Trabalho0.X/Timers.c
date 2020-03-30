#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>

#define SYSCLK  80000000L               // System clock frequency, in Hz
#define PBCLOCK 40000000L               // Peripheral Bus Clock frequency, in Hz
                  
#define Presc_val           8          // biggest value is chosen to obtain smaller frequencies  
#define TPS_256             3          // TCKPS code for 256 pre-scaler  
                                       // 000=> 1 , 001=> 2, 010=> 4, 011=> 8 , ... 111=> 256

void config_Timer2(int freq){
    T2CONbits.ON = 0;                   //Stop timer
    T2CONbits.TGATE = 0;                
    T2CONbits.TCS = 0;                
    IFS0bits.T2IF = 0;                  //Reset interrupt flag
    IEC0bits.T2IE = 0;                  //Enable T2 interrupts, 0 -> polling
    // Timer period configuration
    T2CONbits.TCKPS = TPS_256;          //Select pre-scaler
                                        //Divide by 256 pre-scaler - Timer 2 contains this prescaler: 1,2,4,8,16,36,64,256 (correspond to the number 7)
    T2CONbits.T32 = 0;                  // 16 bit timer operation      
    TMR2 = 0; 
    PR2 = PBCLOCK/(Presc_val*freq) - 1; // defines timer frequency equal to Timer2_freq                                  
    T2CONbits.TON = 1;                  // Start the timer
}

void config_Timer3(int freq){
    T3CONbits.ON = 0;                   //Stop timer              
    IFS0bits.T3IF = 0;                  //Reset interrupt flag
    // Timer period configuration
    T3CONbits.TCKPS = 1;                //Select pre-scaler
    //T3CONbits.T32 = 0;                // 16 bit timer operation      
    TMR3 = 0;
    PR3 = PBCLOCK/(1*freq) - 1;         // defines timer frequency with prescalet with value 1                             
}
