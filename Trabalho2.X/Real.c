#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

/*declare other functions of others files*/
void start_ADC(void);
float ADC_OUT(void);
float transf_function(float val);

/* Function ReadSensor is used to obtain the real value of temperature from PT100*/
float ReadSensor(void){
   // Variable declarations;
    double temperature_real;
    double duty;
    /*dont necessary realized mean because is already done in ADC_OUT()*/
    start_ADC();                                 // init conversation           
    duty = ADC_OUT();                            // obtain the value from ADC
    temperature_real = transf_function(duty);     
    return temperature_real;
     
}