#include <math.h>
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

#define samples 400
#define range_max 50
#define range_min 10

/*global variables*/
static double speed_si[samples];         //used for simulated temperature values
static double speed_real[samples];        //used for real temperature values
static int aux_real = 0;
static int aux_sim = 0;
static double u_real=0; 
static double u_sim= 0;
static int x,y;
static int rotation = 0;

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
    printf("| 0 : Stop system                           \t\t|\n");
    printf("| 1 : Show instantaneous speed              \t\t|\n");
    printf("| 2 : Increase value of speed               \t\t|\n");
    printf("| 3 : Decrease value of speed               \t\t|\n");
    printf("| 4 : Peak speed ( from startup )           \t\t|\n");
    printf("| 5 : Mean speed ( from startup )           \t\t|\n");
    printf("| 6 : Show angular velocity                 \t\t|\n");
    printf("| 7 : Info about the system                 \t\t|\n");
    printf("| 8 : Repeat menu print                     \t\t|\n");
    printf("| 9 : Exit                                  \t\t|\n");
    printf("---------------------------------------------\n\n\r");
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

void verification(int x){
        double speed_desired;
        speed_real[aux_real] = ReadSensor();              // read, first, value of speed 
        x = floor(speed_real[aux_real]);                  // to convert for integer with resolution 1 c
        printf("current speed %d C",x);                     // print new value
        if(x == 0){                                             // x == 0 when we want heet or cool the resistor, in inicial case
            while(speed_real[aux_real]<range_min){        // if <range_min then 
                printf(" Out of range [40;70]\n");
                u_real = PI_controller(speed_real[aux_real],speed_real[aux_real]+1,0.5,2,0.2);      //send to the function PI_Controller 
                set_PWM(u_real);                                // send signal u for pwm, to adjust the temperature
                aux_real = aux_real +1;                         // increase index
                speed_real[aux_real] = ReadSensor();      // read new value for new index
                x = floor(speed_real[aux_real]);          // to convert for integer with resolution 1 ºc
                printf("Current temperature %d C",x);             // print new value
                PORTAbits.RA3 = 0;                              // If the led is desative it is because the temperature is out the range
            }
            while(speed_real[aux_real]>range_max){               // while temperature <range_max then 
                printf(" Out of range [40;70]\n");
                u_real = PI_controller(speed_real[aux_real],speed_real[aux_real]-1,0.5,2,0.2);      //send to the function PI_Controller 
                set_PWM(u_real);                                // send signal u for pwm, to adjust the temperature
                aux_real = aux_real +1;                         // increase index
                speed_real[aux_real] = ReadSensor();      // read new value for new index
                 x = floor(speed_real[aux_real]);         // to convert for integer with resolution 1 ºc
                printf("Current temperature %d C",x);             // print new value
                PORTAbits.RA3 = 0;                              // If the led is desative it is because the temperature is out the range
            }
            PORTAbits.RA3 = 1;                                  // If the led is active it is because the temperature is within the range
        }else if (x == 1){                                      // x == 1 when we want heet the resistor  
            speed_desired = speed_real[aux_real]+1;
            while(speed_real[aux_real]<speed_desired){
                printf("| Heating Resistor |\n");
                u_real = PI_controller(speed_real[aux_real],speed_desired,0.5,2,0.2);
                set_PWM(u_real);
                u_real = u_real +1;                             // increase index
                speed_real[aux_real] = ReadSensor();      // read new value for new index
            }
        }else if( x == 2){                                      // x == 2 when we want cool the resistor  
            speed_desired = speed_real[aux_real]-1;
            while(speed_real[aux_real]>speed_desired){
                printf("| Cool Resistor |\n");
                u_real = PI_controller(speed_real[aux_real],speed_desired,0.5,2,0.2);
                set_PWM(u_real);
                u_real = u_real +1;                             // increase index
                speed_real[aux_real] = ReadSensor();      // read new value for new index
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
        speed_si[aux_sim] = init_sim();                  // init the signal pwn to ajust the temperature into the range 
        if(speed_si[aux_sim] > 40) {
            PORTAbits.RA3 = 1;                                  // If the led is active it is because the temperature is within the range
        }
    }else if(user0 =='1'){
        verification(0);
    }else{                                                      // char invalid, repeat 
        puts("Invalid entry");
        speed_si[aux_sim] = 0;
        interface();  
    }
    printMenu();                                               // prints interface 
    
    while(user != '8'){                                        // when user press number 5 the system out
                //active led 
        if(user0 == '1'){    
            led(speed_real[aux_real]);
        }else if(user0 == '0'){
            led(speed_si[aux_sim]);
        }
        
        //interface
        user = getch(); 
        switch(user){
        case '0':                                              // stop system
            if(user0 == '1'){ 
               set_PWM(0);
            }else if(user0 == '0'){
               speed_si[aux_sim] = 0;
            }
            break;
        case '1':
            if(user0 == '1'){                                  // real values (sensor PT100)
                speed_real[aux_real+1] = ReadSensor();
                puts("\n Instant speed : ");
                x = floor(speed_real[aux_real]);         // to convert for integer with resolution 1 ºc
                printf("%d C \n",x);  
            }else if(user0 == '0'){                            // simulate values 
                puts("\n Instant simulate speed  : ");
                printf("%1f C \n ",speed_si[aux_sim]);  
            }
            break;
        case '2' :                                         
            if(user0 == '1'){                                  // real values (sensor PT100)
                verification(1);
            }else if(user0 == '0'){                            // simulate values 
                printf("|Increase speed|");
                //u_sim = PI_controller(temperature_sim[aux_sim],temperature_sim[aux_sim]+1,2,1,0.2);
                //set_PWM(u_sim);
                aux_sim = aux_sim +1;                                         //refresh the index (aux_sim)
                if(speed_si[aux_sim-1]+1 >range_max ){
                    speed_si[aux_sim] = speed_si[aux_sim-1];    // dont refresh
                    printf(" Out of range [40;70]\n");
                }else{ 
                    speed_si[aux_sim] = speed_si[aux_sim-1]+1; //refresh the array
                    PORTAbits.RA3 = 1;                        // If the led is active it is because the temperature is within the range
                }
                printf("New speed: %f",speed_si[aux_sim]);
            }
            break;
        case '3' :     
            if(user0 == '1'){                               // real values (sensor PT100)
                verification(2);                                
            }else if(user0 == '0'){                         // simulate values 
                printf("| Decrease Speed |");
                //u_sim = PI_controller(temperature_sim[aux_sim],temperature_sim[aux_sim]-1,0.5,2,0.2);
                //set_PWM(u_sim);
                aux_sim = aux_sim +1;                                             //refresh the index (aux_sim)
                if(speed_si[aux_sim-1]-1 <range_min ){
                    speed_si[aux_sim] = speed_si[aux_sim-1];        // dont refresh
                    printf(" Out of range [40;70]\n");
                }else{
                    speed_si[aux_sim] = speed_si[aux_sim-1]-1;      //refresh the array
                    PORTAbits.RA3 = 1;                      // If the led is active it is because the temperature is within the range
                }
                printf("New speed: %f",speed_si[aux_sim]);
            }
            break;  
        case '4' :                                          // calculate the peak of temperature from startup   
            if(user0 == '1'){                                               
                max = speed_real[0];                  // max = the first value of array 
                for(i=1;i<aux_real+1;i++){                  // i < number of positions
                    if(speed_real[i]>max){            // if atual value of temperature is bigger then max
                        max = speed_real[i];          // max refresh with the new temperature
                    }                        
                }
            printf("|Peak speed ( from startup ): %d C |\n", max);    
            }else if(user0 == '0'){                         //for simulate values  
                max = speed_si[0];                   // max = the first value of array 
                for(i=1;i<aux_sim+1;i++){                   // i < number of positions
                    if(speed_si[i]>max){             // if atual value of temperature is bigger then max
                        max= speed_si[i];            // max refresh with the new temperature
                    }
                }
            printf("|Peak speed ( from startup ): %d C |\n", max);    
            }    
            break;
        case '5':                                           // calculate the mean
            if(user0 == '1'){
                mean = speed_real[0];
                for(i=1;i<aux_real+1;i++){
                    mean = mean + speed_real[i];
                }
                mean = mean / (aux_real +1 );
                printf("|Mean speed ( from startup ): %f C |\n", mean);    
            }
            else if(user0 == '0'){
                mean = speed_si[0];
                for(i=1;i<aux_sim;i++){
                    mean = mean + speed_si[i];
                }
                mean = mean /(aux_sim +1);
                printf("|Mean speed ( from startup ): %f C |\n", mean);    
            }
            break;
        case '7':
            puts("\nThe main objective of this project is to create circuits, \n that controlled from the user, can heat a resistance \n in a given temperature range and with specifications \n explicit in the report. ");
            break;
        case '8':
            printMenu();
            break;
        case '9':
            puts("--Finished--");
            puts("To continue reset the system");
            exit(0);
            break;
        }
    }
}
