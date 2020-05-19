/*
 * Projeto 4 - Controlo do Motor DC
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
#define SYSCLK        80000000L               // System clock frequency, in Hz
#define PBCLOCK       40000000L               // Peripheral Bus Clock frequency, in Hz
#define baud             115200

/*declare other functions of others files*/
void init_Ports(void);
void config_Timer2(int freq);
void config_Timer3(int freq);
void verify_UART(void);
void config_ADC(void);
void config_PWM(void);
void interface(void);
int UartInit(uint64_t pbclock, uint32_t br);

int main(int argc, char** argv) {
    init_Ports();                           // init ports 
    config_Timer2(timer_freq);              // config timer 2 for ADC
    config_Timer3(freq_PWM);                // config timer 3 for PWM
    verify_UART();                          // check the connection in the uart (when the uart sets the LED 5 lights)
    config_ADC();
    UartInit(PBCLOCK, baud);    
    config_PWM();
    
    while(1){  
        interface();
    }
    return (EXIT_SUCCESS);
}

