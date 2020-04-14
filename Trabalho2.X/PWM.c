#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>

#define SYSCLK      80000000L               // System clock frequency, in Hz
#define PBCLOCK     40000000L               // Peripheral Bus Clock frequency, in Hz
#define freq_PWM    2000   
#define PWM_steps   256      

void config_PWM(void){
   // Set OC3 - chipKIT Pin 6 
    OC3CONbits.OCM = 6;          // OCM = 0b110 : OC1 in PWM mode,  (Output compare mode selected bits)
    OC3CONbits.OCTSEL = 1;       // Timer 3 is clock source of OCM - 1 - select timer 3 / 0 - select timer 2
   // OC3RS = 0;
    OC3CONbits.ON = 1;           // Enable OC3
}

double transf_function(double val){ 
    double m = 0.384481818;
    double R = val / 0.001;      // calculation of the sensor resistance value  (with 1mA from corrent source)
    val =( R - 100 ) / m;        // this formula is explain in report
    return val;
}

void set_PWM(int duty){
     OC3RS = (((PBCLOCK/1)/ freq_PWM) * duty) / PWM_steps;    
}

void start_PWM(void){
    // Start PWM generation
    T3CONbits.TON = 1; // Start the timer
}

void test_pwm(void){
    int i,j;
    for ( i = 0; i<256;i++){
        set_PWM(i);                          // increses until 256 steps
        for( j=0;j<10000;j++);                  // delay
    }
}
