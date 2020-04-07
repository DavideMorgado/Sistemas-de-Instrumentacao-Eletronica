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
    PORTCbits.RC1 = 1;
    
    // Set RD0 as digital output - PWM
    TRISDbits.TRISD0 = 0;
    PORTDbits.RD0 = 1;
}
