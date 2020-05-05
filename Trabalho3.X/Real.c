#include <math.h>
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 
#define steps 256
#define range_min 100
#define range_max 600

/*declare other functions of others files*/
void  start_ADC(void);
float ADC_OUT(void);
float transf_function(float val);

double percentage(double val){                          //to convert the distance in percentage
    double res = val *100 / (range_max-range_min);
    return res;
}

/* Function ReadSensor is used to obtain the real value of temperature from PT100*/
double ReadSensor(void){
   // Variable declarations;
    double distance;
    double duty;
    /*dont necessary realized mean because is already done in ADC_OUT()*/
    start_ADC();                                        // init conversation           
    duty = ADC_OUT();                                   // obtain the value from ADC
    distance = transf_function(duty);
    double duty_cycle =percentage(distance);
    set_PWM(duty_cycle);
    return distance;
}