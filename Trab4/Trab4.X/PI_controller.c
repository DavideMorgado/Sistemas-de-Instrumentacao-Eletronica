/* Controlador PI.
 * Entradas :   Sinal a ser controlado - y(t)
 *              Valor pretendido - r(t)
 *              Intervalo de amostragem - k
 *              Constante Kp 
 *              Constante Ti
 *              
 * Saída :      Sinal de controlo - u(t)
 * 
 */ 
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h" 

/*global variables*/
static double integral_part;
static double u;

double PI_controller(double y, double r, double k, double Kp, double Ti)
{
    /*Variables*/
    double e;
    if( r < 10){
        r = 10;
    }else if ( r > 50){
        r = 50;
    }else{
    }
    e = r - y;                                  //
    integral_part = integral_part + e*k;        //  esta variavel tem que ser global juntamente com o sinal u
    u = Kp * (e + (1/Ti)*integral_part);  
    return u;
}