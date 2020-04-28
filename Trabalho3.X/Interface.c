#include <math.h>
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

#define samples 400
#define range_max 600
#define range_min 100

/*global variables*/
static double dist_sim[samples];         //used for simulated temperature values
static double dist_real[samples];        //used for real temperature values
static int aux_real = 0;
static int aux_sim = 0;
static double u_real=0; 
static double u_sim= 0;
static int x,y;
/*declare other functions of others files*/
int set_PWM(int PWM_VAL);
double PI_controller(double y, double r, double k, double Kp, double Ti);
float simulate(int value); 
int init_sim(void);
float ReadSensor(void);
void start_PWM(void);
void init_Ports(void);

/*Used for obtain a char */
unsigned char getch(void)
{
    if(U1STAbits.OERR == 1){                            // if register Receive buffer has overflowed (1) then reset
        U1STAbits.OERR == 0;
    }
    while(!U1STAbits.URXDA);                            // Wait while !URXDA 
    if((U1STAbits.FERR == 1) ||(U1STAbits.PERR == 1)){  // if Framing error and Parity error has been detected for the current character
        U1RXREG = getchar(); 
        return 0;
    }else{
        return U1RXREG = getchar();
    }
    return U1RXREG;                                     //return register with last char pressed
}

/*Used for transmite a char*/
void putch(char byte2send){
    while(U1STAbits.UTXBF == 1);                        // wait while UTXBF == 1
    U1TXREG = byte2send;                                // Copy byte2send to the UxTXREG register
}

/*Used for print a Menu*/
void printMenu(void){
    //Show menu
    printf("\n\n");
    printf("\n\n");
    printf("-------------------------------------------------\n");
    printf("|                     MENU                      |\n");
    printf("-------------------------------------------------\n");
    printf("| 1 : Show instantaneous distance           \t|\n");
    printf("| 2 : Peak distance ( from startup )     \t\t|\n");
    printf("| 3 : Mean distance ( from startup )     \t\t|\n");
    printf("| 4 : Info about the system                 \t\t|\n");
    printf("| 5 : Repeat menu print                     \t\t|\n");
    printf("| 6 : Exit                                  \t\t|\n");
    printf("------------------------------------------------\n\n\r");
}

void verification(int x){
        double temp_desired;
        dist_real[aux_real] = ReadSensor();              // read, first, value of temperature 
        x = floor(temperature_real[aux_real]);                  // to convert for integer with resolution 1 c
        printf("current distance %d C",x);                   // print new value
        if(x == 0){                                             // x == 0 when we want heet or cool the resistor, in inicial case
            while(dist_real[aux_real]<range_min){        // if <range_min then 
                printf(" Out of range [;]\n");
                u_real = PI_controller(dist_real[aux_real],dist_real[aux_real]+1,0.5,2,0.2);      //send to the function PI_Controller 
                set_PWM(u_real);                                // send signal u for pwm, to adjust the temperature
                aux_real = aux_real +1;                         // increase index
                dist_real[aux_real] = ReadSensor();      // read new value for new index
                x = floor(temperature_real[aux_real]);          // to convert for integer with resolution 1 ºc
                printf("Current temperature %d C",x);           // print new value
                PORTAbits.RA3 = 0;                              // If the led is desative it is because the temperature is out the range
            }
            while(dist_real[aux_real]>range_max){               // while temperature <range_max then 
                printf(" Out of range [40;70]\n");
                u_real = PI_controller(dist_real[aux_real],dist_real[aux_real]-1,0.5,2,0.2);      //send to the function PI_Controller 
                set_PWM(u_real);                                // send signal u for pwm, to adjust the temperature
                aux_real = aux_real +1;                         // increase index
                dist_real[aux_real] = ReadSensor();      // read new value for new index
                 x = floor(temperature_real[aux_real]);         // to convert for integer with resolution 1 ºc
                printf("Current temperature %d C",x);             // print new value
                PORTAbits.RA3 = 0;                              // If the led is desative it is because the temperature is out the range
            }
            PORTAbits.RA3 = 1;                                  // If the led is active it is because the temperature is within the range
        }else if (x == 1){                                      // x == 1 when we want heet the resistor  
            temp_desired = dist_real[aux_real]+1;
            while(dist_real[aux_real]<temp_desired){
                printf("| Heating Resistor |\n");
                u_real = PI_controller(dist_real[aux_real],temp_desired,0.5,2,0.2);
                set_PWM(u_real);
                u_real = u_real +1;                             // increase index
                dist_real[aux_real] = ReadSensor();      // read new value for new index
            }
        }else if( x == 2){                                      // x == 2 when we want cool the resistor  
            temp_desired = dist_real[aux_real]-1;
            while(dist_real[aux_real]>temp_desired){
                printf("| Cool Resistor |\n");
                u_real = PI_controller(dist_real[aux_real],temp_desired,0.5,2,0.2);
                set_PWM(u_real);
                u_real = u_real +1;                             // increase index
                dist_real[aux_real] = ReadSensor();      // read new value for new index
            }
        }
}

/* Interface for user interaction */
void interface(void){
        //variable declarations
    init_Ports();
    char user,user0;
    int i,max;
    double mean;

    start_PWM();                                                // start function start PWM
    
    printf("\nPrefere use simulation or real values ? \n");     //choose real values with sensor PT100 or simulate values 
    printf("| 0 : Simulation ? \n");
    printf("| 1 : Real values? \n");
    user0 = getch();
    if(user0 == '0'){
        dist_sim[aux_sim] = init_sim();                  // init the signal pwn to ajust the temperature into the range 
        if(dist_sim[aux_sim] > range_min) {
            PORTAbits.RA3 = 1;                                  // If the led is active it is because the temperature is within the range
        }
    }else if(user0 =='1'){
        //verification(0);
        printf("Out of range");
    }else{                                                      // char invalid, repeat 
        puts("Invalid entry");
        dist_sim[aux_sim] = 0;
        interface();  
    }
    printMenu();                                               // prints interface 
    
    while(user != '6'){                                        // when user press number 6 the system out
        user = getch(); 
        switch(user){
        case '1':
            if(user0 == '1'){                                  // real values (sensor PT100)
                dist_sim[aux_real+1] = ReadSensor();
                puts("\n Instant temperature : ");
                x = floor(temperature_real[aux_real]);         // to convert for integer with resolution 1 ºc
                printf("%d C \n",x);  
            }else if(user0 == '0'){                            // simulate values 
                puts("\n Instant simulate temperature  : ");
                printf("%1f C \n ",dist_sim[aux_sim]);  
            }
            break;
        case '2' :                                             // calculate the peak of temperature from startup   
            if(user0 == '1'){                                               
                max = dist_real[0];                             // max = the first value of array 
                for(i=1;i<aux_real+1;i++){                     // i < number of positions
                    if(dist_real[i]>max){                       // if atual value of temperature is bigger then max
                        max = dist_real[i];                     // max refresh with the new temperature
                    }                        
                }
            printf("|Peak distance ( from startup ): %d mm |\n", max);    
            }
            else if(user0 == '0'){                               //for simulate values  
                max = dist_sim[0];                               // max = the first value of array 
                for(i=1;i<aux_sim+1;i++){                        // i < number of positions
                    if(dist_sim[i]>max){                        // if atual value of temperature is bigger then max
                        max= dist_sim[i];            // max refresh with the new temperature
                    }
                }
            printf("|Peak distance ( from startup ): %d mm |\n", max);    
            }    
            break;
        case '3':                                           // calculate the mean
            if(user0 == '1'){
                mean = dist_real[0];
                for(i=1;i<aux_real+1;i++){
                    mean = mean + dist_real[i];
                }
                mean = mean / (aux_real +1 );
                printf("|Mean temperature ( from startup ): %f C |\n", mean);    
            }
            else if(user0 == '0'){
                mean = dist_sim[0];
                for(i=1;i<aux_sim;i++){
                    mean = mean + dist_sim[i];
                }
                mean = mean /(aux_sim +1);
                printf("|Mean temperature ( from startup ): %f C |\n", mean);    
            }
            break;
        case '4':
            puts("\nThe main objective of this project is to create circuits, \n that controlled from the user, \n explicit in the report. ");
            break;
         case '5':
            printMenu();
            break;
        case '6':
            puts("--Finished--");
            puts("To continue reset the system");
            exit(0);
            break;
        }
    }
}