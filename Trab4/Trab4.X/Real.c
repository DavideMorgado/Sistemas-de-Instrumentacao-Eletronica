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

double ConvDegree(int count){
    double degree = (count *360)/420;
    printf("Degree: %d", degree);
    return degree;
}

double ReadRPM(int count){
   // Variable declarations;
    double rpm_real;
    rpm_real = (count * 1 ) / (420);     // count ---- x rpm
                                         // 420   ---- 1 rpm
    printf("RPM: %d", rpm_real);         // print
    return rpm_real;
}
