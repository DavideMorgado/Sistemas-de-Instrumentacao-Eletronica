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
#define range_min 10
#define range_max 50
/*global variables*/
static double integral_part;
static double u;

double PI_controller(double y, double r, double k, double Kp, double Ti)
{
    /*Variables*/
    double e;
    /* RPM in range [range_min,range_max] rpm*/
    if( r < range_min){
        r = range_min;
    }else if ( r > range_max){
        r = range_max;
    }else{
    }
    e = r - y;                                  //
    integral_part = integral_part + e*k;        //  esta variavel tem que ser global juntamente com o sinal u
    u = Kp * (e + (1/Ti)*integral_part);  
    return u;
}