#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

float simulate(int value){ 
    float temperature_si[100];
    int i = 0;
    temperature_si[i++] = value;
    int j,k;
    temperature_si[i]++;
    for(j=0;j<150000;j++);
    for(k=0;k<100000;k++); 
    return temperature_si[i];
}

int init_sim(void){
    int aux=0;
    int i=0;
    double temperature_si[100];
    temperature_si[i] = simulate(0);       // for simulate :    float temperature = simulate(0);       
                                        // for use real sensor: float temperature = ReadSensor()
    while(temperature_si[i] <= 40){
/* Code valid for real simulation, that's why we commented on this code, but we understand that 3 lines would be necessary to increase the temperature
        u = PI_controller(temperature_si[i],temperature_si[i]+1,0.5,2,0.2);      //send to the function PI_Controller 
        set_PWM(u);     // just for real values 
*/           
        printf("|------------------|");
        printf("| Heating Resistor |");
        printf("... %f ºC\n", temperature_si[i++]);
        temperature_si[i] = simulate(aux);
    }
    if(temperature_si[i] >= 40){
        printf("\n|------------------|\n");
        printf("| Inside range|\n");         
    }  
    return temperature_si[i];
}