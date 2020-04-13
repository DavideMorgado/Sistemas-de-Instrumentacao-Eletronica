/*
 * Projeto 2-Controle de Processo Térmico
 *   - Davide Morgado    84950
 *   - Guilherme Maniezo 84907
 *
 * Para reter mais informações sobre o presente projeto, basta abrir o documento README
*/

#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 
#include "config_bits.h"

#define timer_freq          250
#define freq_PWM           2000   
#define SYSCLK  80000000L               // System clock frequency, in Hz
#define PBCLOCK 40000000L               // Peripheral Bus Clock frequency, in Hz
#define baud  115200

/* iniciate the functions */
void init_Ports(void);
void config_Timer2(int freq);
void config_Timer3(int freq);
void verify_UART(void);
void config_ADC(void);
void config_PWM(void);
void start_PWM(void);
void start_ADC(void);
float ADC_OUT(void);
int set_PWM(int PWM_VAL);
void test_pwm(void);
int GetChar(uint8_t *byte);
int UartInit(uint64_t pbclock, uint32_t br);
char getch(void);
char putch(void);
void interface(void);


int main(int argc, char** argv) {
    init_Ports();
    config_Timer2(timer_freq);
    config_Timer3(freq_PWM);
    verify_UART(); 
    config_ADC();
    UartInit(PBCLOCK, baud);    
    
    start_PWM();                                // start function start PWM
    interface();
    while(1){  

    }
    return (EXIT_SUCCESS);
}

