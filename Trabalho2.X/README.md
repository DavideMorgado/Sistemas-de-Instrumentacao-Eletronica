# Sistemas-de-Instrumentacao-Eletronica
# Projetos realizados, realizado por:
#   - Davide Morgado   , MIEET,    84950
#   - Guilherme Maniezo, MIEET,    84907

Projeto 2 - Controle de Processo Térmico 

* Possui 10 ficheiros:
 - ADC.C
 - Interface.c
 - main.c
 - PI_controller.c
 - Ports.c
 - PWM.c
 - Real.c
 - Simulate.c
 - Timers.c
 - uart.c

Funções importantes para interpretação do código:
* ReadSensor() -  Obtêm-se valores provenientes da ADC 
* verification(int x)- De acordo com o valor de x vai controlar o sistema por intermédio do controlador proporcional integral, enviar o sinal do controlador para a entrada do pwm  e verificar o novo valor de temperatura. 
* getch() - Lê um caracter
* PI_controller(...) - Ajusta um valor temperatura num novo valor de temperatura desejado, de um modo proporcional e integral
* init_sim() e simulate() - Funções desenvolvidas para simular a aquisação virtual de temperatura

*** Para informação mais detalhada, ler o relatório associado, ProjetoA.pdf ***
