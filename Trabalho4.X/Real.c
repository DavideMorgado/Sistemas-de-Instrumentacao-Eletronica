#include <math.h>
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

/*declare other functions of others files*/
void start_ADC(void);
float ADC_OUT(void);
float transf_function(float val);

double ReadRPM(int x){
   // Variable declarations;
    double rpm_real;

    /*dont necessary realized mean because is already done in ADC_OUT()*/
    //start_ADC();                                        // init conversation           
    //duty = ADC_OUT();                                   // obtain the value from ADC
    // temperature_real = transf_function(0.123242);    // test if the temperature is 60ºC, table R is 123.242 ohm and knowing the current (1mA) we get V = 0.123242
    rpm_real = (x * 1 ) / (420);
    return rpm_real;
}