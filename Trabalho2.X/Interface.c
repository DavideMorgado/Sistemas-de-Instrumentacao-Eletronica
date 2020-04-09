#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

char getch(void)
{
    if(U1STAbits.OERR == 1) U1STAbits.OERR =0;  // If OERR == 1 then reset OERR
    while(U1STAbits.URXDA == 0);  // Wait while URXDA == 0
    return U1RXREG;     // Return U1RXREG
}

void putch(char byte2send){
    while(U1STAbits.UTXBF == 1);    // wait while UTXBF == 1
    U1TXREG = byte2send;            // Copy byte2send to the UxTXREG register
}

void ReadSensor(void){
   // Variable declarations;
    float temperature;
    float val_adc;
    int i,number = 1;
    for(i=0;i<number;i++){
        start_ADC();
        val_adc = ADC_OUT();
        temperature = transf_function(val_adc);
        puts("\n Temperatura instantanea : ");
        printf("%d\n",temperature);
    }
}

void interface(void){
    char user;
    char read;
    puts("\n Interface Grafica: \n 0 - Mais informacoes \n 1 - Mostrar temperatura instântanea \n 2 - Configurar valor de temperatura \n 3 - Temperatura máxima obtida \n s - Para sair!\n");
    while(user != 's'){
        //puts ("\nOpção escolhida: ");
        user = getch(); 
        putch(user);
        switch(user){
        case '0':
            puts("\nO presente projeto tem como principal objetivo criar circuitos, que controlados a partir do utilizador, possam aquecer uma resistência num dado intervalo de temperatura e com umas especificações explicitas no relatório. ");
            break;
        case '1':
            ReadSensor();
            break;
        case '2' :
           
            break;
        case '3' :
            puts("\n Estado 3");
            break;
        
        }
    }
}
