#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

double temperature_sim[100];
double temperature_real[100];

int set_PWM(int PWM_VAL);
int PI_controller(int y, int r, float k, int Kp, int Ti);
float simulate(int value); 
int init_sim(void);
float ReadSensor(void);

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
    int max=0,i,aux_real=0, aux_sim=0;
    int x;
    int u_real, u_sim;
    //choose real values with sensor PT100 or simulate values 
    printf("\nPrefere use simulation or real values ? \n");
    printf("| 0 : Simulation ? \n");
    printf("| 1 : Real values? \n");
    user0 = getch();
    if(user0 == '0'){
        //aux_sim = init_sim();                   // init the signal pwn to ajust the temperature into the range 
        temperature_sim[aux_sim] = init_sim();      
    }else if(user0 =='1'){
        temperature_real[aux_real] = 0;
        while(temperature_real[aux_real]>70){
            temperature_real[aux_real+1] = ReadSensor();
            printf("Temperature atual",temperature_real[aux_real]);
            u_real = PI_controller(temperature_real[aux_real],temperature_real[aux_real]-1,0.5,2,0.2);
            set_PWM(u_real);
        }
        while(temperature_real[aux_real]<40.0){
            temperature_real[aux_real+1] = ReadSensor();
            printf("Temperature atual",temperature_real[aux_real]);
            u_real = PI_controller(temperature_real[aux_real],temperature_real[aux_real]+1,0.5,2,0.2);
            set_PWM(u_real);
        }
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
            if(user0 == '1'){                               // real values (sensor PT100)
                temperature_real[aux_real+1] = ReadSensor();
                puts("\n instant temperature : ");
                printf("%f\n",temperature_real[aux_real]);  
            }else if(user0 == '0'){                         // simulate values 
                puts("\n Instant simulate temperature  : ");
                printf("%f \n",temperature_sim[aux_sim]);  
            }
            break;
        case '2' :                                           // real values (sensor PT100)
            if(user0 == '1'){
                printf("| Heating Resistor |\n");
                u_real = PI_controller(temperature_real[aux_real],temperature_real[aux_real]+1,0.5,2,0.2);
                set_PWM(u_real);
                printf("New temperature: %f",temperature_real[aux_real]++);
            }else if(user0 == '0'){                          // simulate values 
                printf("| Heating Resistor|\n");
                u_sim = PI_controller(temperature_sim[aux_sim],temperature_sim[aux_sim]+1,0.5,2,0.2);
                set_PWM(u_sim);
                aux_sim = aux_sim +1;
                temperature_sim[aux_sim] = temperature_sim[aux_sim-1]+1;
                printf("New temperature: %f",temperature_sim[aux_sim]);
            }
            break;
        case '3' :     
            if(user0 == '1'){
                printf("| Heating Resistor |\n");
                u_real = PI_controller(temperature_real[aux_real],temperature_real[aux_real]-1,2,0.1,0.1);
                set_PWM(u_real);
                printf("New temperature: %f",temperature_real[aux_real]--);
                //printf("sinal u: %f",u_real);
            }else if(user0 == '0'){                          // simulate values 
                printf("| Heating Resistor |\n");
                u_real = PI_controller(temperature_sim[aux_sim],temperature_sim[aux_sim]-1,0.5,2,0.2);
                set_PWM(u_sim);
                printf("New temperature: %f",temperature_sim[aux_sim]--);
                //printf("sinal u: %f",u_real);
            }
            break;
        case '4' : 
            if(user0 == '1'){
                for (i=0;i<aux_real;i++){
                    if(temperature_real[aux_real] > max){
                        max = temperature_real[aux_real];
                    }
                }
            }else if(user0 == '0'){           
                for(x=0;x<40;){
                   
                    if(max < temperature_sim[x]){
                        max = temperature_sim[x];
                    }
                    x++;
                    
                }
            }
            
            printf("|Peak temperature ( from startup ): %f ºC |\n", max);
            break;
        case '5':
            puts("Bye");
            puts("To continue reset the system");
            exit(0);
            break;
        }
    }
}
