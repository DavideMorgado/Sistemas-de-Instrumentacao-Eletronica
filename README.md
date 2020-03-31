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

De modo a que o código fosse reutilizável decidimos dividir as funções por ficheiros, assim como:
	* ADC
	* Ports
	* PWM
	* Timers
Em cada ficheiro encontram-se as respetivas funções.

Descrição sucinta das funções:
 * As funções config_Timer2, config_Timer3, config_ADC e config_PWM são funções básicas de configuração dos registos responsáveis pelo funcionamento do Timer 2 e Timer 3, da  ADC e do sinal PWM, respetivamente.
 * A função init_Port ajusta os pinos de entrada e de saída do PIC.
 * A função verify_UART assegura a comunicação UART.
 * As funções start_ADC e ADC_OUT são incunbidas de iniciarem o processo na ADC e disponibilizar na saída o seu resultado, respetivamente.
 * A função set_PWM serve para ajustar os parâmetros do pwm.
 * A função test_pwm serviu para realizar um teste no osciloscópio, para verificar se o PWM estava devidamente configurado.
 * A função transfer_function faz a conversão de [0,3.3] para [0,255]

A automatização do código permite alterar facilmente valores como de frequências, passos do PWM, entre outros
 * Para alterar o valor da frequência do Timer 2 basta definir outro valor na variável timer_freq
 * Para alterar o valor da frequência do Timer 3, correspondente ao PWM, basta definir outro valor na variável freq_PWM
 * Para alterar o valor de steps do PWM, basta definir outro valor na variável PWM_steps
	Nota: estas variáveis estão definidias no inicio do código

* Por fim, dispomos de um ficheiro main.c, em que contêm todas as funções nesse mesmo ficheiro e para compilar basta includir esse ficheiro e o ficheiro uart.c
* Dispomos do ficheiro mainv2.c que contêm a inclusão das funções que se encontram em ficheiros separados  ( descritos em cima ) . Para compilar basta incluir o ficheiro mainv2.c, ADC.c, Ports.c, PWM.c, Timers.c e uart.c


 