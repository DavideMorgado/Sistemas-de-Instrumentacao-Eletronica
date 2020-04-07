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

/* iniciate the functions */
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
    
    
    return (EXIT_SUCCESS);
}

