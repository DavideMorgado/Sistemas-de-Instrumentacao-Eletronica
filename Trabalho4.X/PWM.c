#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>

#define SYSCLK      80000000L               // System clock frequency, in Hz
#define PBCLOCK     40000000L               // Peripheral Bus Clock frequency, in Hz
#define freq_PWM    2000   
#define PWM_steps   1000      
int updateOC = 0;
int i = 0;
int rpm_ant [1000];
static double k = 0.5,Kp=2,Ti = 0.2;
double PI_controller(double y, double r, double k, double Kp, double Ti);

void config_PWM(void){
   // Set OC3 - chipKIT Pin 6 
    OC3CONbits.OCM = 6;          // OCM = 0b110 : OC1 in PWM mode,  (Output compare mode selected bits)
    OC3CONbits.OCTSEL = 1;       // Timer 3 is clock source of OCM - 1 - select timer 3 / 0 - select timer 2
   // OC3RS = 0;
    OC3CONbits.ON = 1;           // Enable OC3
    
     // Set OC2 - chipKIT Pin 5
    OC3CONbits.OCM = 6;          // OCM = 0b110 : OC1 in PWM mode,  (Output compare mode selected bits)
    OC3CONbits.OCTSEL = 1;       // Timer 3 is clock source of OCM - 1 - select timer 3 / 0 - select timer 2
   // OC3RS = 0;
    OC3CONbits.ON = 1;           // Enable OC3
}
/*
double transf_function(double val){ 
    double m = 0.384481818;
    double R = val / 0.001;      // calculation of the sensor resistance value  (with 1mA from corrent source)
    val =( R - 100 ) / m;        // this formula is explain in report
    return val;
}
*/

void set_PWM(int rpm_actual){
    rpm_ant[i+1]= rpm_actual;
    updateOC = PI_controller(rpm_ant[i], rpm_actual, k, Kp, Ti);
    i++;
    if(updateOC<0){
        OC1RS = 0;
    }else{
        if(rpm_actual == 0){
            OC2RS = 500;        //A1
            OC3RS = 500;        //A2
        }else if(rpm_actual > 0){
            OC2RS = updateOC;
            OC3RS = 0;
        }else if(rpm_actual < 0){
            OC2RS = 0;
            OC3RS = updateOC;
        }
        printf("OC2RS = %d, resolution = %d \n",OC2RS,PR2);
        printf("OC3RS = %d, resolution = %d \n",OC3RS,PR3);
    }    
}

void start_PWM(void){
    // Start PWM generation
    T3CONbits.TON = 1; // Start the timer
}