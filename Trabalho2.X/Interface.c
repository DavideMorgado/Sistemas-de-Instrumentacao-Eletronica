#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

static float temperature[100];

int set_PWM(int PWM_VAL);
int PI_controller(int y, int r, float k, int Kp, int Ti);
float simulate(int value); 
int init(void);

unsigned char getch(void)
{
     if(U1STAbits.OERR == 1){           // if register Receive buffer has overflowed (1) then reset)
        U1STAbits.OERR == 0;
     }
     while(!U1STAbits.URXDA );
     if((U1STAbits.FERR == 1) ||(U1STAbits.PERR == 1)){  // if Framing error and Parity error has been detected for the current character
        U1RXREG = getchar(); 
        return 0;
     }else{
        return U1RXREG = getchar();
     }
     return U1RXREG;
}

void putch(char byte2send){
    while(U1STAbits.UTXBF == 1);    // wait while UTXBF == 1
    U1TXREG = byte2send;            // Copy byte2send to the UxTXREG register
}

float ReadSensor(void){
   // Variable declarations;
    float media=0;
    float val_adc;
    int i,elements = 10;
    for(i=0;i<elements;i++){
        start_ADC();
        val_adc = ADC_OUT();
        temperature[i] = transf_function(val_adc);
        media = media + temperature[i];
    }
    media = media/elements;
    return media;
}

void printMenu(void){
    //Show menu
    printf("\n\n");
    printf("\n\n");
    printf("-------------------------------------------------\n");
    printf("|                     MENU                      |\n");
    printf("-------------------------------------------------\n");
    printf("| 0 : more info                          \t\t|\n");
    printf("| 1 : Show instantaneous temperature     \t|\n");
    printf("| 2 : Increase value of temperature      \t\t|\n");
    printf("| 3 : Decrease value of temperature      \t\t\t|\n");
    printf("| 4 : Peak temperature ( from startup )  \t\t\t\t|\n");
    printf("| 5 : Exit                               \t\t\t\t\t|\n");
    printf("----------------------------------------\n\n\r");
}


void interface(void){
    //variable declarations
    char user,user0;
    float temp;
    int maxs = 0,i,u,aux;
    printf("\nPrefere use simulation or real values ? \n");
    printf("| 0 : Simulation ? \n");
    printf("| 1 : Real values? \n");
    user0 = getch();
    if(user0 == '0'){
        aux = init();                   // init the signal pwn to ajust the temperature into the range 
    }else if(user0 =='1'){
        
    }else{
        puts("Invalid entry");
    }
    printMenu();                        // prints interface 
    while(user != '5'){                 // when user press number 5 the system out
        user = getch(); 
        switch(user){
        case '0':
            puts("\nO presente projeto tem como principal objetivo criar circuitos, que controlados a partir do utilizador, possam aquecer uma resistência num dado intervalo de temperatura e com umas especificações explicitas no relatório. ");
            break;
        case '1':
            temp = ReadSensor();
            puts("\n Temperatura instantanea : ");
            printf("%d\n",temp);  
            break;
        case '2' :
            printf("| Heating Resistor |\n");
            u = PI_controller(aux,aux++,0.5,2,0.2);
            set_PWM(u);
            break;
        case '3' :     
            printf("| Cooling down Resistor|\n");
            u = PI_controller(aux,aux--,0.5,2,0.2);
            set_PWM(u);
            break;
        case '4' : 
            printf("OLA");
            for (i=0;i<1000;i++){
                //if(temperature[i] = maxs){
                    
            //    }
            }
            maxs = 5;
            printf("|Peak temperature ( from startup ): %f ºC |\n", maxs);
            break;
        case '5':
            puts("Bye");
            puts("To continue reset the system");
            exit(0);
            break;
        }
    }
}
