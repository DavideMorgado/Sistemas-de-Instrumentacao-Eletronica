#include <math.h>
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 
#define freq_PWM    30000
double ConvDegree(int count){
    double degree = (count *360)/420;   
    if(degree >= 360){
        degree = 0;
    }else if(degree <= -360){
        degree = 0;
    }
    printf("Degree: %d", degree);
    return degree;
}

double ReadRPM(int time){
   // Variable declarations;
    double rpm_real;
    rpm_real = (60 * freq_PWM) / (time * 420);     // count ---- x rpm
                                                   // 420   ---- 1 rpm
                                                    // * freq PWM
    printf("RPM: %d", rpm_real);         // print
    return rpm_real;
}
