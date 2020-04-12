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

static double integral_part;

int PI_controller(int y, int r, double k, double Kp, double Ti)
{
    if( r  < 40){
        r = 40;
        printf(" Out of range [40;70]");
    }else if ( r > 70){
        r = 70;
        printf(" Out of range [40;70]");
    }else {}
    int e = r - y;                              //  
    integral_part = integral_part + e*k;        //  esta variavel tem que ser global juntamente com o sinal u
    int u = Kp*(e + (1/Ti)*integral_part);  
    return u;
}