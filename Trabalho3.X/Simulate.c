#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

float simulate(int value){ 
    float distance_si[100];
    int i = 0;
    distance_si[i++] = value;
    int j,k;
    distance_si[i]++;
    for(j=0;j<150000;j++);
    for(k=0;k<100000;k++); 
    return distance_si[i];
}

int init_sim(void){
    int aux=0;
    int i=0;
    double distance_si[100];
    distance_si[i] = simulate(0);       // for simulate :    float temperature = simulate(0);       
                                        // for use real sensor: float temperature = ReadSensor()
    while(distance_si[i] < 40){
/* Code valid for real simulation, that's why we commented on this code, but we understand that 3 lines would be necessary to increase the temperature
        u = PI_controller(distance_si[i],distance_si[i]+1,0.5,2,0.2);      //send to the function PI_Controller 
        set_PWM(u);     // just for real values 
*/           
        printf("|------------------|");
        printf("| Heating Resistor |");
        printf("... %f C\n", distance_si[i++]);
        distance_si[i] = distance_si(aux);
    }
    if(distance_si[i] >= 40){
        printf("\n|------------------|\n");
        printf("| Inside range|\n");         
    }  
    return distance_si[i];
}