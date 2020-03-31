# Sistemas-de-Instrumentacao-Eletronica
# Trabalho 0, realizado por:
#   - Davide Morgado,    84950
#   - Guilherme Maniezo, 84907
 
Este trabalho possui 12 funções:
 1) init_Ports
 2) config_Timer2
 3) config_Timer3
 4) verify_UART
 5) config_ADC 
 6) config_PWM
 7) start_PWM
 8) start_ADC
 9) ADC_OUT
 10) set_PWM
 11) transf_function
 12) test_pwm
 
 * As funções config_Timer2, config_Timer3, config_ADC e config_PWM são funções básicas de configuração dos registos responsáveis pelo funcionamento do Timer 2 e Timer 3, da  ADC e do sinal PWM, respetivamente.
 * A função init_Port ajusta os pinos de entrada e de saída do PIC.
 * A função verify_UART assegura a comunicação UART.
 * As funções start_ADC e ADC_OUT são incunbidas de iniciarem o processo na ADC e disponibilizar na saída o seu resultado, respetivamente.
 * A função set_PWM serve para ajustar os parâmetros do pwm.
 * A função test_pwm serviu para realizar um teste no osciloscópio, para verificar se o PWM estava devidamente configurado.
 * A função transfer_function faz a conversão de [0,3.3] para [0,255]
 