/*
 * Trabalho 0  
 *   - Davide Morgado    84950
 *   - Guilherme Maniezo 84907
 *
 * Para reter mais informações abrir documento README
*/
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 
#include "config_bits.h"

#define timer_freq          250
#define freq_PWM           2000   

void init_Ports(void);
void config_Timer2(int freq);
void config_Timer3(int freq);
void verify_UART(void);
void config_ADC(void);
void config_PWM(void);
void start_PWM(void);
void start_ADC(void);
float ADC_OUT(float res);
int set_PWM(int PWM_VAL);
void test_pwm(void);


int main(int argc, char** argv) {
    //iniciate the functions 
    init_Ports();
    config_Timer2(timer_freq);
    config_Timer3(freq_PWM);
    verify_UART(); 
    config_ADC();
    config_PWM();
    
    // Variable declarations;
    float steps; 
    int i = 0;                                  // - PWM
    float res;
    int j;
    
    start_PWM();                                // start function start PWM
    
    while(1){
        start_ADC();
        int val_adc = ADC_OUT(res);
        int duty_cycle = transf_function(val_adc);
        set_PWM(duty_cycle);
        for( j=0;j<10000;j++);                 // delay
        
        //test_pwm();                          // if we try test pwm with pwm in oscilloscope decomment test_pwm()
        
    }
    return (EXIT_SUCCESS);
}

