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
static double speed_si[samples];          //used for simulated temperature values
static double speed_real[samples];        //used for real temperature values
static int aux_real = 0;
static int aux_sim = 0;
static double u_real=0; 
static double u_sim= 0;
static int x,y;
static int direction = 1;                 // initial diretion is positive 
static int count = 0,user_rpm,degree,cnt_time;
static double pos_angular,act_rpm;
/*declare other functions of others files*/
int set_PWM(int rpm);
double PI_controller(double y, double r, double k, double Kp, double Ti);
float simulate(int value); 
int init_sim(char rpm);
double ReadRPM(int time);
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
    printf("| 6 : Show angular positon                  \t\t|\n");
    printf("| 7 : Change direction                      \t\t|\n");
    printf("| 8 : Info about the system and repeat menu \t\t|\n");
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
    }else if(val <= range_max){
        PORTAbits.RA3 = 1;   
    }else {
        PORTAbits.RA3 = 0;                                       // force the led as '0'
    } 
}

 void __ISR (_EXTERNAL_1_VECTOR, IPL5SRS) ExtISR(void){          //check channel B
    PORTDbits.RD0 = 1;                                           //force value = 1 to can test and present results
    if(PORTDbits.RD0){
        direction = 1;
        count = count +1; 
    }else{
        direction = -1;
        count = count -1;
    }
    act_rpm = ReadRPM(cnt_time);
    cnt_time = 0;
    degree = ConvDegree(count);
    printf("RPM: %f\n", act_rpm);                               // print
    printf("Degree: %f\n", degree);
    speed_real[aux_real+1] = 10;//act_rpm;
    aux_real++;
    IFS0bits.INT1IF = 0;                                        // Reset interrupt flag
}

void __ISR (_TIMER_4_VECTOR, IPL7AUTO) T4Interrupt(void){
    cnt_time++;
    IFS0bits.INT2IF = 0;                                        // Reset interrupt flag
}

/* Interface for user interaction */
void interface(void){   
    init_Ports();
    /* Enable Interrupt Exceptions */
    // set the CP0 status IE bit high to turn on interrupts globally
    __builtin_enable_interrupts();
      //variable declarations
    char user,user0;
    int i,max;
    double mean;

    start_PWM();                                                // start function start PWM
    
    printf("\nPrefere use simulation or real values ? \n");     //choose real values with sensor PT100 or simulate values 
    printf("| 0 : Simulation ? \n");
    printf("| 1 : Real values? \n");
    
    speed_real[aux_real] = 10;
    aux_real++;
    user0 = getch(); 
    if(user0 == '0'){
        speed_si[aux_sim] = init_sim(0);                       // init the signal pwn to ajust the temperature into the range 
        if(speed_si[aux_sim] > range_min) {
            PORTAbits.RA3 = 1;                                 // If the led is active it is because the temperature is within the range
        }
    }else if(user0 =='1'){

        set_PWM(speed_real[aux_real]);
    }
    printMenu();                                               // prints interface 
    user = 'x';
    while(user != '9'){                                        // when user press number 5 the system out
            //active led 
        if(user0 == '1'){    
            led(speed_real[aux_real]);
        }else if(user0 == '0'){
            led(speed_si[aux_sim]);
        }
            //interface
        user = getch(); 
        switch(user){
        case '0':                                               // stop system
            if(user0 == '1'){ 
               printf("0 - STOP System\n");
               set_PWM(0);                                      // stop motor
            }else if(user0 == '0'){
               printf("0 - STOP System\n");
               speed_si[aux_sim] = 0;
            }
            break;
        case '1':
            if(user0 == '1'){                                   // real values - increse rpm
                printf("| 1 - Read RPM |\n");
                speed_real[aux_real] = act_rpm;
                //x = floor(speed_real[aux_real]);              // to convert for integer with resolution 
                printf("\n Instant %f rpm \n",speed_real[aux_real]);  
                printf("\n Instant %f rpm \n",act_rpm);  
                printf("\n Instant %d rpm \n",act_rpm);  

            }else if(user0 == '0'){                             // simulate values 
                printf("| 1 - Read RPM |\n");
                puts("\n Instant simulate speed  : ");
                printf("%1f rpm \n ",speed_si[aux_sim]);  
            }
            break;
        case '2' :                                          
            if(user0 == '1'){                                   // real values  - increase rpm (+1)
                speed_real[aux_real+1] = speed_real[aux_real]+1;
                aux_real++;
                printf("| 2 - Increase RPM ( +1 ) |\n");
                set_PWM(speed_real[aux_real]+1);
                if(speed_real[aux_real-1]+1 >range_max ){
                    speed_real[aux_real] = speed_real[aux_real-1];    // dont refresh
                    printf(" Out of range [%d;%d]\n",range_min,range_max);
                }
            }else if(user0 == '0'){                             // simulate values 
                printf("| 2 - Increase RPM ( +1 ) |\n");
                aux_sim = aux_sim +1;                                         //refresh the index (aux_sim)
                if(speed_si[aux_sim-1]+1 >range_max ){
                    speed_si[aux_sim] = speed_si[aux_sim-1];    // dont refresh
                    printf(" Out of range [%d;%d]\n",range_min,range_max);
                }else{ 
                    speed_si[aux_sim] = speed_si[aux_sim-1]+1; //refresh the array
                    PORTAbits.RA3 = 1;                         // If the led is active it is because the temperature is within the range
                }
                printf("New value of RPMs: %f",speed_si[aux_sim]);
            }
            break;
        case '3' :     
            if(user0 == '1'){                                    // real values 
                speed_real[aux_real+1] = speed_real[aux_real]-1;
                aux_real++;
                printf("| 2 - Decrease RPM ( -1 ) |\n");
                set_PWM(speed_real[aux_real]-1);
                if(speed_real[aux_real-1]+1 <range_min ){
                    speed_real[aux_real] = speed_real[aux_real-1];    // dont refresh
                    printf(" Out of range [%d;%d]\n",range_min,range_max);
                }
            }else if(user0 == '0'){                              // simulate values 
                printf("| 2 - Decrease RPM |\n");
                aux_sim = aux_sim +1;                            //refresh the index (aux_sim)
                if(speed_si[aux_sim-1]-1 <range_min ){
                    speed_si[aux_sim] = speed_si[aux_sim-1];     // dont refresh
                    printf(" Out of range [%d;%d]\n",range_min,range_max);
                }else{
                    speed_si[aux_sim] = speed_si[aux_sim-1]-1;   //refresh the array
                    PORTAbits.RA3 = 1;                           // If the led is active it is because the temperature is within the range
                }
                printf("New value of RPMs: \n%f",speed_si[aux_sim]);
            }
            break;  
        case '4' :                                          // calculate the peak of temperature from startup   
            if(user0 == '1'){                                               
                max = speed_real[0];                        // max = the first value of array 
                for(i=1;i<aux_real+1;i++){                  // i < number of positions
                    if(speed_real[i]>max){                  // if atual value of temperature is bigger then max
                        max = speed_real[i];                // max refresh with the new temperature
                    }                        
                }
            printf("| 4 - Peak speed ( from startup ): %d rpm |\n", max);    
            }else if(user0 == '0'){                         //for simulate values  
                max = speed_si[0];                          // max = the first value of array 
                for(i=1;i<aux_sim+1;i++){                   // i < number of positions
                    if(speed_si[i]>max){                    // if atual value of temperature is bigger then max
                        max= speed_si[i];                       // max refresh with the new temperature
                    }
                }
            printf("| 4 - Peak speed ( from startup ): %d rpm |\n", max);    
            }    
            break;
        case '5':                                           // calculate the mean
            if(user0 == '1'){
                mean = speed_real[0];
                for(i=1;i<aux_real+1;i++){
                    mean = mean + speed_real[i];
                }
                mean = mean / (aux_real +1 );
                printf("| 5 - Mean speed ( from startup ): %f rpm |\n", mean);    
            }
            else if(user0 == '0'){
                mean = speed_si[0];
                for(i=1;i<aux_sim;i++){
                    mean = mean + speed_si[i];
                }
                mean = mean /(aux_sim +1);
                printf("| 5 - Mean speed ( from startup ): %f rpm |\n", mean);    
            }
            break;
        case '6':   
            pos_angular = 0.5/ count; 
            printf("| 6 - Angular Position: %f |\n", pos_angular);
            break;
        case '7':
            direction = -direction;
            printf("| 7 - Direction changed |\n Now is: %d \n",direction);
            break;
        case '8':
            printf("| 8 - More info and repeat print menu |\n\n");
            puts("The main objective of this project is to create circuits, \n that controlled from the user, can increase or decrease RPM of motor \n More explicitation in the report or Readme.md .\n\n ");
            printMenu();
            break;
        case '9':
            puts("| 9 - Exit | \n");
            puts("--Finished--");
            puts(" To continue reset the system ");
            exit(0);
            break;
        }
    }
}
