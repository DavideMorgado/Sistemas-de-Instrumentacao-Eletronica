/*
 * Trabalho 0  
 *   - Davide Morgado    84950
 *   - Guilherme Maniezo 84907
 
 Este trabalho possui 11 funções:
 1) init_Ports
 2) config_Timer2
 3) config_Timer3
 4) verify_UART
 5) config_ADC 
 6) config_PWM
 7) start_PWM
 8) start_ADC
 9) ADC_OUT
 10) set_PWM
 11) test_pwm
 
 * As funções config_Timer2, config_Timer3, config_ADC e config_PWM são funções básicas de configuração dos registos responsáveis pelo funcionamento do Timer 2 e Timer 3, da  ADC e do sinal PWM, respetivamente.
 * A função init_Port ajusta os pinos de entrada e de saída do PIC.
 * A função verify_UART assegura a comunicação UART.
 * As funções start_ADC e ADC_OUT são incunbidas de iniciarem o processo na ADC e disponibilizar na saída o seu resultado, respetivamente.
 * A função set_PWM serve para ajustar os parâmetros do pwm.
 * A função test_pwm serviu para realizar um teste no osciloscópio, para verificar se o PWM estava devidamente configurado.
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

