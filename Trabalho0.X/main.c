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

#define SYSCLK  80000000L               // System clock frequency, in Hz
#define PBCLOCK 40000000L               // Peripheral Bus Clock frequency, in Hz
                         
#define Presc_val           8        // biggest value is chosen to obtain smaller frequencies 
                                       
#define timer_freq          250
#define TPS_256             3          // TCKPS code for 256 pre-scaler
                                       // 000=> 1 , 001=> 2, 010=> 4, 011=> 8 , ... 111=> 256

#define PWM_steps           255        
#define freq_PWM            2000

void init_Ports(void){
	/* Configure RA3 and RC1 (led 4 and led 5) as Output */
    TRISAbits.TRISA3 = 0;
    TRISCbits.TRISC1 = 0;
  
	/*define initial values for RA3 and RC1*/
    PORTAbits.RA3 = 0;
    PORTCbits.RC1 = 1;
    
    // Set RD0 as digital output - PWM
    TRISDbits.TRISD0 = 0;
    PORTDbits.RD0 = 1;
}

void config_Timer2(int freq){
    T2CONbits.ON = 0;                   //Stop timer
    T2CONbits.TGATE = 0;                
    T2CONbits.TCS = 0;                
    IFS0bits.T2IF = 0;                  //Reset interrupt flag
    IEC0bits.T2IE = 0;                  //Enable T2 interrupts, 0 -> polling
    // Timer period configuration
    T2CONbits.TCKPS = TPS_256;          //Select pre-scaler
                                        //Divide by 256 pre-scaler - Timer 2 contains this prescaler: 1,2,4,8,16,32,64,256 (correspond to the number 7)
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

void verify_UART(void){
    // Init UART and redirect tdin/stdot/stderr to UART
    if(UartInit(PBCLOCK, 115200) != UART_SUCCESS) {
        PORTAbits.RA3 = 1;              // If Led active error initializing UART
        while(1);
    }
    __XC_UART = 1;                      /* Redirect stdin/stdout/stderr to UART1*/ 
    // Disable JTAG interface as it uses a few ADC ports
    DDPCONbits.JTAGEN = 0;   
}

void config_ADC(void){
    // Initialize ADC module
    // Polling mode, AN0 as input
    // Generic part 
    AD1CON1bits.SSRC = 7;           // Internal counter ends sampling and starts conversion
    AD1CON1bits.CLRASAM = 1;        //Stop conversion when 1st A/D converter interrupt is generated and clears ASAM bit automatically
    AD1CON1bits.FORM = 0;           // Integer 16 bit output format
    AD1CON2bits.VCFG = 0;           // VR+=AVdd; VR-=AVss
    AD1CON2bits.SMPI = 0;           // Number (+1) of consecutive conversions, stored in ADC1BUF0...ADCBUF{SMPI}
    AD1CON3bits.ADRC = 1;           // ADC uses internal RC clock
    AD1CON3bits.SAMC = 16;          // Sample time is 16TAD ( TAD = 100ns)
    // Set AN0 as input
    AD1CHSbits.CH0SA = 0;           // Select AN0 as input for A/D converter
    TRISBbits.TRISB0 = 1;           // Set AN0 to input mode
    AD1PCFGbits.PCFG0 = 0;          // Set AN0 to analog mode
    // Enable module
    AD1CON1bits.ON = 1;             // Enable A/D module (This must be the ***last instruction of configuration phase***)
}

void config_PWM(void){
   // Set OC3 - chipKIT Pin 6 
    OC3CONbits.OCM = 6;             // OCM = 0b110 : OC1 in PWM mode,  (Output compare mode selected bits)
    OC3CONbits.OCTSEL = 1;          // Timer 3 is clock source of OCM - 1 - select timer 3 / 0 - select timer 2
   // OC3RS = 0;
    OC3CONbits.ON = 1;              // Enable OC3
}

void start_PWM(void){
    // Start PWM generation
    T3CONbits.TON = 1;              // Start the timer
}

void start_ADC(void){
    // Get one sample
    IFS1bits.AD1IF = 0;             // Reset interrupt flag
    AD1CON1bits.ASAM = 1;           // Start conversion
    while (IFS1bits.AD1IF == 0);    // Wait fo EOC
}

float ADC_OUT(float res){
    // Sampled voltage  - ADC
    int i;
    int mean = 0;
    int elements = 10;
    //create a filter digital
    for(i=0;i<elements;i++){
        mean = mean + (ADC1BUF0 * 3.3) / 1023;  // Convert to 0..3.3V 
    }
    res = mean / elements;
    
    // Output result
    printf("Voltage: %f \n\r",res);
    //printf("Temp:%f \n",(res-2.7315)/.01); // For a LM335 directly connected
    return res;
}

int transf_function(int val){ 
    val = val * 255 / 1023;
    return val;
}

void set_PWM(int duty){
    OC3RS = (((PBCLOCK/1)/ freq_PWM) * duty) / PWM_steps;     // 255, because we need convert 0 ... 255
}


void test_pwm(void){
    int i,j;
    for ( i = 0; i<256;i++){
        set_PWM(i);                          // increses until 256 steps
        for( j=0;j<10000;j++);               // delay
    }
}

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
