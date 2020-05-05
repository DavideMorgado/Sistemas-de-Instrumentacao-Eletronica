# Sistemas-de-Instrumentacao-Eletronica
# Projetos realizados, realizado por:
#   - Davide Morgado   , MIEET,    84950
#   - Guilherme Maniezo, MIEET,    84907

Projeto 3 - Medição de Distância

* Possui  ficheiros:
 - ADC.C
 - Interface.c
 - main.c
 - Ports.c
 - PWM.c
 - Real.c
 - Simulate.c
 - Timers.c
 - uart.c

Funções importantes para interpretação do código:
* ReadSensor() -  Obtêm-se valores provenientes da ADC 
* verification()- Serve para verificar se a distância está na gama pretendida
* getch() - Lê um caracter
* init_sim() e simulate() - Funções desenvolvidas para simular a aquisação virtual de temperatura
* transfer_funct(), conv_dist(), percentage() e resolution() - servem para converter o valor proveniente da ADC no inverso da distância, converter o inverso na distância, converter o valor lido de distancia em percentagem e escolher a resolução de 1mm
* led() - para ligar o led quando está dentro da range
*** Para informação mais detalhada, ler o relatório associado, ProjetoA.pdf 
***
