#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

#define range_min 100
#define range_max 600

float simulate(int value){ 
    float dist_sim[100];
    int i = 0;
    dist_sim[i++] = value;
    int j,k;
    dist_sim[i]=dist_sim[i]+10 ;
    for(j=0;j<150000;j++);
    for(k=0;k<100000;k++); 
    return dist_sim[i];
}

int init_sim(void){
    int aux=0;
    int i=0;
    double dist_sim[100];
    dist_sim[i] = simulate(50);         
    while(dist_sim[i] < range_min){          
        printf("|--------Simulate increase distance----------|");
        printf("| -------------- |");
        printf("... %f mm\n", dist_sim[i++]);
        dist_sim[i] = simulate(aux);
    }
    if(dist_sim[i] >= range_min){
        printf("\n|------------------|\n");
        printf("| Inside range|\n");         
    }  
    return dist_sim[i];
}