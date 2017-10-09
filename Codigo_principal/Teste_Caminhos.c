


/* #define SENSIBILIDADE 0.1 */ /* Nao necessario */

#include "Funcoes_testadas/sensores_e_motores.h"
/*
void ligar_sensores();
*/

#include "Funcoes_testadas/giro.h"
/*
float getGyroOffset();
void girar(float degrees);
*/

#include "Funcoes_testadas/cores.h"
/*
bool intervalo_cor(int cor, int sensor);
void reto(int cor);
bool verificar_direcao(int cor);
*/

#define ESQUERDA 1
#define DIREITA 2
#define FRENTE 3

void voltar()
{
	girar(180);
	reto(BRANCO);
}

int testar_caminho(int cor, int vetor_direcao[])
{
	if (vetor_direcao[AZUL] != ESQUERDA && vetor_direcao[VERMELHO] != ESQUERDA && vetor_direcao[VERDE] != ESQUERDA)
	{
		TextOut(10,10, "teste esquerda");
		Wait(2000);
		ClearScreen();
		girar(90);
		ClearScreen();

		if (verificar_direcao(cor))
		{
			return ESQUERDA;
		}
		voltar();
		girar(90); 
	if (vetor_direcao[AZUL] != FRENTE && vetor_direcao[VERMELHO] != FRENTE && vetor_direcao[VERDE] != FRENTE)
	{
		if (verificar_direcao(cor))
		{
		return FRENTE;
		}
		voltar();
		girar(180);	
	}
	girar(-90);	
	verificar_direcao(cor);
	return DIREITA;
	}
}	


void seguir_direcao(int vetor_direcao[], int cor)
{
	if (vetor_direcao[cor] == ESQUERDA)
	{
		girar(90);
	}
	if (vetor_direcao[cor] == DIREITA)
	{
		girar(-90);
	}
	reto(cor);
}


task main () //por enquato a maior parte está só com a lógica, tem que alterar as funções pra ele conseguir andar certinho e girar
{
	int direcoes[6] = {0, 0, 0, 0, 0, 0}; //achei mais prático criar um vetor de 6 posiçoes e usar as constantes como o valor do índice
	ligar_sensores();

	reto(BRANCO);
	Wait(2000);
	
	if (intervalo_cor(AZUL, SENSOR_COR_DIREITA) && intervalo_cor(AZUL, SENSOR_COR_ESQUERDA))
	{	
		ClearScreen();
		TextOut(10,10, "ACHEI AZUL");
		reto(AZUL);
		if (direcoes [AZUL] == 0)
		{
			direcoes[AZUL] = testar_caminho(AZUL, direcoes);
		} else 
		{
			seguir_direcao(direcoes, AZUL);		
		}
	}
  
  if (intervalo_cor(VERMELHO, SENSOR_COR_DIREITA) && intervalo_cor(VERMELHO, SENSOR_COR_ESQUERDA))
	{
		reto(VERMELHO);
		if (direcoes [VERMELHO] == 0)
		{
			direcoes[VERMELHO] = testar_caminho(VERMELHO, direcoes);
		} else 
		{
			seguir_direcao(direcoes, VERMELHO);		
		}
	}
  
  if (intervalo_cor(VERDE, SENSOR_COR_DIREITA) && intervalo_cor(VERDE, SENSOR_COR_ESQUERDA))
	{
		reto(VERDE);
		if (direcoes [VERDE] == 0)
		{
			direcoes[VERDE] = testar_caminho(VERDE, direcoes);
		} else 
		{
			seguir_direcao(direcoes, VERDE);		
		}
	}
}
