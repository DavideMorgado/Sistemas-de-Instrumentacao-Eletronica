#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

float ReadSensor(void){
   // Variable declarations;
    double temperature_real[100];
    double media=0;
    double val_adc;
    int i,elements = 10;
    for(i=0;i<elements;i++){
        start_ADC();
        val_adc = ADC_OUT();
        temperature_real[i] = transf_function(val_adc);
        media = media + temperature_real[i];
    }
    media = media/elements;
    return media;
}


