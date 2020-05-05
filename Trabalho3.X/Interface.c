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
static double dist_sim[samples];         //used for simulated distance values
static double dist_real[samples];        //used for real distance values
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
    printf("------------------------------------------------\r");
    printf("Option: \r");
}

int resolution(double val){
    int  x = floor(val);                                          // to convert for integer with resolution 1 mm
    return x;
}

void led(double val){
    if(val >= range_min) {
        PORTAbits.RA3 = 1;                                       // If the led is active it is because the distance is within the range
    }
    else if(val <= range_max){
        PORTAbits.RA3 = 1;   
    }else {
        PORTAbits.RA3 = 0;                                       // force the led as '0'
    } 
}

void verification(void){
    int dist;
    dist_real[aux_real] = ReadSensor();                          // read, first, value of distance 
    dist = resolution(dist_real[aux_real]);                      // to convert for integer with resolution 1 mm
    //printf("current distance %f mm",dist);                       // print new value
    while(dist_real[aux_real]<range_min && dist_real[aux_real]>range_max){
        dist_real[aux_real++] = ReadSensor();                    // read value
        if(dist_real[aux_real]<range_min){                       // if distance <range_min then 
            printf(" Out of range %d \n", range_min);
            printf("Current distance %d mm",dist);               // print new value
        }
        else if(dist_real[aux_real]>range_max){                  // if distance <range_max then 
            printf(" Out of range %d \n", range_max); 
            printf("Current distance %d mm",dist);               // print new value
        }
        led(dist_real[aux_real]);
    }
}

/* Interface for user interaction */
void interface(void){
        //variable declarations
    init_Ports();
    char user,user0;
    int i,converted;
    double mean;
    double max;
    
    start_PWM();                                                // start function start PWM
    
    printf("\nPrefere use simulation or real values ? \n");     //choose real values with sensor PT100 or simulate values 
    printf("| 0 : Simulation ? \n");
    printf("| 1 : Real values? \n");
    user0 = getch();
    if(user0 == '0'){
        dist_sim[aux_sim] = init_sim();                         // init the signal to ajust the distance into the range 
    }else if(user0 =='1'){
        printf("Mode : Real values selected\n");
        verification();
    }else{                                                      // char invalid, repeat 
        puts("Invalid entry");
        dist_sim[aux_sim] = 0;
        interface();  
    }
    printMenu();                                               // prints interface 
    
    while(user != '6'){                                        // when user press number 6 the system out
        //active led 
        if(user0 == '1'){    
            led(dist_real[aux_real]);
        }else if(user0 == '0'){
            led(dist_sim[aux_sim]);
        }
        
        //interface
        user = getch(); 
        switch(user){
        case '1':
            if(user0 == '1'){                                  // real values (sensor PT100)
                dist_real[aux_real+1] = ReadSensor();
                if(dist_real[aux_real] < range_min || dist_real[aux_real] > range_max ){
                    printf("Out of range[%d;%d]", range_min,range_max);  
                }  
                else{
                    puts("\n Instant distance : ");
                    converted = resolution(dist_real[aux_real]);
                    printf("%d mm \n",converted); 
                    aux_real = aux_real +1;
                }
            }else if(user0 == '0'){                            // simulate values 
                puts("\n Instant simulate distance  : ");
                printf("%f mm \n ",dist_sim[aux_sim]);
                aux_sim = aux_sim +1;
            }
            break;
        case '2' :                                             // calculate the peak of distance from startup   
            if(user0 == '1'){                                               
                max = dist_real[0];                            // max = the first value of array 
                for(i=1;i<aux_real+1;i++){                     // i < number of positions
                    if(dist_real[i]>max){                      // if atual value of distance is bigger then max
                        max = dist_real[i];                    // max refresh with the new distance
                    }                        
                }
            converted = resolution(max);
            printf("|Peak distance ( from startup ): %d mm |\n", converted);    
            }
            else if(user0 == '0'){                              // for simulate values  
                max = dist_sim[0];                              // max = the first value of array 
                for(i=1;i<aux_sim+1;i++){                       // i < number of positions
                    if(dist_sim[i]>max){                        // if atual value of distance is bigger then max
                        max= dist_sim[i];                       // max refresh with the new distance
                    }
                }
            converted = resolution(max);
            printf("|Peak distance ( from startup ): %d mm |\n", converted);    
            }    
            break;
        case '3':                                                // calculate the mean
            if(user0 == '1'){
                mean = dist_real[0];
                for(i=1;i<aux_real+1;i++){
                    mean = mean + dist_real[i];
                }
                mean = mean / (aux_real +1 );
                printf("|Mean distance ( from startup ): %f mm |\n", mean);    
            }
            else if(user0 == '0'){
                mean = dist_sim[0];
                for(i=1;i<aux_sim;i++){
                    mean = mean + dist_sim[i];
                }
                mean = mean /(aux_sim +1);
                printf("|Mean distance ( from startup ): %f mm |\n", mean);    
            }
            break;
        case '4':
            puts("\nThe main objective of this project is to create circuits, \n that controlled from the user, with the possibility to read the distance sensor value and obtain\n some information after the measurements as maximum and average value. \n For more information are explicit in the report. ");
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