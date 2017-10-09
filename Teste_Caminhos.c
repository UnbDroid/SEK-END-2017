
#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B /*conexão com o outro cérebro*/
#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_4
#define SENSOR_GYRO IN_4 /*teste*/
#define SENSOR_US_GARRA IN_3 /*teste*/

#define SENSIBILIDADE 0.1
#define OFFSET_SAMPLES 2000

void ligar_sensores()
{
	SetSensorHTGyro(SENSOR_GYRO);
	//SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	//SetSensorUltrasonic(SENSOR_US_DIREITA);
	SetSensorColorRed(SENSOR_COR_DIREITA);
	SetSensorColorRed(SENSOR_COR_ESQUERDA);
	SetSensorUltrasonic(SENSOR_US_GARRA);
}

float getGyroOffset() 
{
    float gyro_sum = 0, i;

    for(i = 0; i < OFFSET_SAMPLES; ++i)
    {
       gyro_sum += SensorHTGyro(SENSOR_GYRO);
    }

    return gyro_sum/OFFSET_SAMPLES;
}

void girar(float degrees) // Algoritimo usado pela sek do ano passado //testada
{
	float angle = 0, gyro = 0;
	unsigned long time = CurrentTick(), prev_time;

  Off(MOTORES);

  degrees = -degrees;

	float offset = getGyroOffset();

	if(degrees > 0) {
	  OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_ALTA);
	  OnRev(MOTOR_DIREITA, -VELOCIDADE_ALTA);
	  while(angle < degrees)
	  {
	  	 prev_time = time;
       time = CurrentTick();
       gyro = SensorHTGyro(SENSOR_GYRO);
       angle += (gyro - offset) * (time - prev_time)/1000.0;
       ClearLine(LCD_LINE1);
       TextOut(0, LCD_LINE1, "ANGLE:");
       NumOut(48, LCD_LINE1, angle);


	  }
	} else {
	  OnFwd(MOTOR_DIREITA, -VELOCIDADE_ALTA);
	  OnRev(MOTOR_ESQUERDA, -VELOCIDADE_ALTA);
	  while(angle > degrees)
	  {
	  	 prev_time = time;
       time = CurrentTick();
       gyro = SensorHTGyro(SENSOR_GYRO);
       angle += (gyro - offset) * (time - prev_time)/1000.0;
       ClearLine(LCD_LINE1);
       TextOut(0, LCD_LINE1, "ANGLE:");
       NumOut(48, LCD_LINE1, angle);
	  }
 }

	Off(MOTORES);
}

#include "Funcoes_testadas/cores.h"

bool intervalo_cor(int cor, int sensor);
void reto(int cor);
bool verificar_direcao(int cor);
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
