#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

#define samples 400
#define range_max 70
#define range_min 40

/*global variables*/
static double temperature_sim[samples];         //used for simulated temperature values
static double temperature_real[samples];        //used for real temperature values
static int aux_real = 0;
static int aux_sim = 0;
static double u_real=0; 
static double u_sim= 0;
  
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
    printf("| 1 : Show instantaneous temperature        \t\t|\n");
    printf("| 2 : Increase value of temperature         \t\t|\n");
    printf("| 3 : Decrease value of temperature         \t\t|\n");
    printf("| 4 : Peak temperature ( from startup )     \t\t|\n");
    printf("| 5 : Mean temperature ( from startup )     \t\t|\n");
    printf("| 6 : Info about the system                 \t\t|\n");
    printf("| 7 : Repeat menu print                     \t\t|\n");
    printf("| 8 : Exit                                  \t\t|\n");
    printf("------------------------------------------------\n\n\r");
}

void verification(int x){
        double temp_desired;
        temperature_real[aux_real] = ReadSensor();              // read, first, value of temperature 
        if(x == 0){                                             // x == 0 when we want heet or cool the resistor, in inicial case
            while(temperature_real[aux_real]<range_min){             // if <range_min then 
                printf(" Out of range [40;70]\n");
                u_real = PI_controller(temperature_real[aux_real],temperature_real[aux_real]+1,0.5,2,0.2);      //send to the function PI_Controller 
                set_PWM(u_real);                                // send signal u for pwm, to adjust the temperature
                aux_real = aux_real +1;                         // increase index
                temperature_real[aux_real] = ReadSensor();      // read new value for new index
                printf("Temperature atual",temperature_real[aux_real]);         // print new value
                PORTAbits.RA3 = 0;                              // If the led is desative it is because the temperature is out the range
            }
            while(temperature_real[aux_real]>range_max){               // while temperature <range_max then 
                printf(" Out of range [40;70]\n");
                u_real = PI_controller(temperature_real[aux_real],temperature_real[aux_real]-1,0.5,2,0.2);      //send to the function PI_Controller 
                set_PWM(u_real);                                // send signal u for pwm, to adjust the temperature
                aux_real = aux_real +1;                         // increase index
                temperature_real[aux_real] = ReadSensor();      // read new value for new index
                printf("Temperature atual",temperature_real[aux_real]);         // print new value
                 PORTAbits.RA3 = 0;                             // If the led is desative it is because the temperature is out the range
            }
            PORTAbits.RA3 = 1;                                  // If the led is active it is because the temperature is within the range
        }else if (x == 1){                                      // x == 1 when we want heet the resistor  
            temp_desired = temperature_real[aux_real]+1;
            while(temperature_real[aux_real]<temp_desired){
                printf("| Heating Resistor |\n");
                u_real = PI_controller(temperature_real[aux_real],temp_desired,0.5,2,0.2);
                set_PWM(u_real);
                u_real = u_real +1;                             // increase index
                temperature_real[aux_real] = ReadSensor();      // read new value for new index
            }
        }else if( x == 2){                                      // x == 2 when we want cool the resistor  
            temp_desired = temperature_real[aux_real]-1;
            while(temperature_real[aux_real]>temp_desired){
                printf("| Cool Resistor |\n");
                u_real = PI_controller(temperature_real[aux_real],temp_desired,0.5,2,0.2);
                set_PWM(u_real);
                u_real = u_real +1;                             // increase index
                temperature_real[aux_real] = ReadSensor();      // read new value for new index
            }
        }
}

/* Interface for user interaction */
void interface(void){
        //variable declarations
    init_Ports();
    char user,user0;
    int i;
    double max, mean;

    start_PWM();                                                // start function start PWM
    
    printf("\nPrefere use simulation or real values ? \n");     //choose real values with sensor PT100 or simulate values 
    printf("| 0 : Simulation ? \n");
    printf("| 1 : Real values? \n");
    user0 = getch();
    if(user0 == '0'){
        temperature_sim[aux_sim] = init_sim();                  // init the signal pwn to ajust the temperature into the range 
        if(temperature_sim[aux_sim] > 40) {
            PORTAbits.RA3 = 1;                                  // If the led is active it is because the temperature is within the range
        }
    }else if(user0 =='1'){
        verification(0);
    }else{                                                      // char invalid, repeat 
        puts("Invalid entry");
        temperature_sim[aux_sim] = 0;
        interface();  
    }
    printMenu();                                               // prints interface 
    while(user != '8'){                                        // when user press number 5 the system out
        user = getch(); 
        switch(user){
        case '0':                                              // stop system
            if(user0 == '1'){ 
               set_PWM(0);
            }else if(user0 == '0'){
               temperature_sim[aux_sim] = 0;
            }
            break;
        case '1':
            if(user0 == '1'){                                  // real values (sensor PT100)
                temperature_real[aux_real+1] = ReadSensor();
                puts("\n Instant temperature : ");
                printf("%f\n",temperature_real[aux_real]);  
            }else if(user0 == '0'){                            // simulate values 
                puts("\n Instant simulate temperature  : ");
                printf("%f \n",temperature_sim[aux_sim]);  
            }
            break;
        case '2' :                                         
            if(user0 == '1'){                                  // real values (sensor PT100)
                verification(1);
            }else if(user0 == '0'){                            // simulate values 
                printf("| Heating Resistor|");
                //u_sim = PI_controller(temperature_sim[aux_sim],temperature_sim[aux_sim]+1,2,1,0.2);
                //set_PWM(u_sim);
                aux_sim = aux_sim +1;                                         //refresh the index (aux_sim)
                if(temperature_sim[aux_sim-1]+1 >range_max ){
                    temperature_sim[aux_sim] = temperature_sim[aux_sim-1];    // dont refresh
                    printf(" Out of range [40;70]\n");
                }else{ 
                    temperature_sim[aux_sim] = temperature_sim[aux_sim-1]+1; //refresh the array
                    PORTAbits.RA3 = 1;                        // If the led is active it is because the temperature is within the range
                }
                printf("New temperature: %f",temperature_sim[aux_sim]);
            }
            break;
        case '3' :     
            if(user0 == '1'){                               // real values (sensor PT100)
                verification(2);                                
            }else if(user0 == '0'){                         // simulate values 
                printf("| Coll Resistor |");
                //u_sim = PI_controller(temperature_sim[aux_sim],temperature_sim[aux_sim]-1,0.5,2,0.2);
                //set_PWM(u_sim);
                aux_sim = aux_sim +1;                                             //refresh the index (aux_sim)
                if(temperature_sim[aux_sim-1]-1 <range_min ){
                    temperature_sim[aux_sim] = temperature_sim[aux_sim-1];        // dont refresh
                    printf(" Out of range [40;70]\n");
                }else{
                    temperature_sim[aux_sim] = temperature_sim[aux_sim-1]-1;      //refresh the array
                    PORTAbits.RA3 = 1;                      // If the led is active it is because the temperature is within the range
                }
                printf("New temperature: %f",temperature_sim[aux_sim]);
            }
            break;  
        case '4' :                                          // calculate the peak of temperature from startup   
            if(user0 == '1'){                                               
                max = temperature_real[0];                  // max = the first value of array 
                for(i=1;i<aux_real+1;i++){                  // i < number of positions
                    if(temperature_real[i]>max){            // if atual value of temperature is bigger then max
                        max = temperature_real[i];          // max refresh with the new temperature
                    }                        
                }
            printf("|Peak temperature ( from startup ): %f ºC |\n", max);    
            }else if(user0 == '0'){                         //for simulate values  
                max = temperature_sim[0];                   // max = the first value of array 
                for(i=1;i<aux_sim+1;i++){                   // i < number of positions
                    if(temperature_sim[i]>max){             // if atual value of temperature is bigger then max
                        max= temperature_sim[i];            // max refresh with the new temperature
                    }
                }
            printf("|Peak temperature ( from startup ): %f ºC |\n", max);    
            }    
            break;
        case '5':                                           // calculate the mean
            if(user0 == '1'){
                mean = temperature_real[0];
                for(i=1;i<aux_real+1;i++){
                    mean = mean + temperature_real[i];
                }
                mean = mean / (aux_real +1 );
                printf("|Peak temperature ( from startup ): %f ºC |\n", mean);    
            }
            else if(user0 == '0'){
                mean = temperature_sim[0];
                for(i=1;i<aux_sim;i++){
                    mean = mean + temperature_sim[i];
                }
                mean = mean /(aux_sim +1);
                printf("|Peak temperature ( from startup ): %f ºC |\n", mean);    
            }
            break;
        case '6':
            puts("\nThe main objective of this project is to create circuits, \n that controlled from the user, can heat a resistance \n in a given temperature range and with specifications \n explicit in the report. ");
            break;
         case '7':
            printMenu();
            break;
        case '8':
            puts("--Finished--");
            puts("To continue reset the system");
            exit(0);
            break;
        }
    }
}
