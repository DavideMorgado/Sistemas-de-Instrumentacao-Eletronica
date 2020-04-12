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
int PI_controller(int y, int r, float k, int Kp, int Ti)
{
    if( r  < 40){
        r = 40;
        printf(" Out of range [40;70]");
    }else if ( r > 70){
        r = 70;
        printf(" Out of range [40;70]");
    }
    int e = r - y;                                  //  
    int i, integral_part;
    for (i = 0;i<k;){
        integral_part = integral_part + e*k(i);        //  esta variavel tem que ser global juntamente com o sinal u
        i = i +0.1;
    }
    int u = Kp*(e + (1/Ti)*integral_part);  
    return u;
}