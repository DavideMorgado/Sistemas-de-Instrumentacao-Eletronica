#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>

void init_Ports(void){
	/* Configure RA3 and RC1 (led 4 and led 5) as Output */
    TRISAbits.TRISA3 = 0;
    TRISCbits.TRISC1 = 0;
    
	/*define initial values for RA3 and RC1*/
    PORTAbits.RA3 = 0;
    PORTCbits.RC1 = 0;
    
    // Set RD1 as digital output - PWM - correspond OC2
    TRISDbits.TRISD1 = 0;
    PORTDbits.RD1 = 1;
    
    // Set RD2 as digital output - PWM - correspond OC3
    TRISDbits.TRISD2 = 0;
    PORTDbits.RD2 = 1;

}
