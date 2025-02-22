#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

#define SYSCLK  80000000L               // System clock frequency, in Hz
#define PBCLOCK 40000000L               // Peripheral Bus Clock frequency, in Hz

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
    AD1CON1bits.SSRC = 7;        // Internal counter ends sampling and starts conversion
    AD1CON1bits.CLRASAM = 1;     // Stop conversion when 1st A/D converter interrupt is generated and clears ASAM bit automatically
    AD1CON1bits.FORM = 0;        // Integer 16 bit output format
    AD1CON2bits.VCFG = 0;        // VR+=AVdd; VR-=AVss
    AD1CON2bits.SMPI = 0;        // Number (+1) of consecutive conversions, stored in ADC1BUF0...ADCBUF{SMPI}
    AD1CON3bits.ADRC = 1;        // ADC uses internal RC clock
    AD1CON3bits.SAMC = 16;       // Sample time is 16TAD ( TAD = 100ns)
    // Set AN0 as input
    AD1CHSbits.CH0SA = 0;        // Select AN0 as input for A/D converter
    TRISBbits.TRISB0 = 1;        // Set AN0 to input mode
    AD1PCFGbits.PCFG0 = 0;       // Set AN0 to analog mode
    // Enable module
    AD1CON1bits.ON = 1;          // Enable A/D module (This must be the ***last instruction of configuration phase***)
}
void start_ADC(void){
    // Get one sample
    IFS1bits.AD1IF = 0;             // Reset interrupt flag
    AD1CON1bits.ASAM = 1;           // Start conversion
    while (IFS1bits.AD1IF == 0);    // Wait fo EOC
}

float ADC_OUT(void){
      // Sampled voltage  - ADC
    int i;
    int mean = 0;
    int elements = 10;
    float res;
    //create a filter digital
    for(i=0;i<elements;i++){
        mean = mean + (ADC1BUF0 * 3.3) / 1023;  // Convert to 0..3.3V 
    }
    res = mean / elements;
    
    // Output result
    //printf("Voltage: %f \n\r",res);
    //printf("Temp:%f \n",(res-2.7315)/.01); // For a LM335 directly connected
    return res;
}

