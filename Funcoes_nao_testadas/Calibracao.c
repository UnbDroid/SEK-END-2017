#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define AMBOS_MOTORES OUT_AC

#include "Funcoes_testadas/comunicacao.h"

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65

#define PRETO 1
#define AZUL 2
#define VERDE 3
#define AMARELO 4
#define VERMELHO 5
#define BRANCO 6
#define FORA 4	// 

#define RETO 0
#define ESQUERDA 1
#define DIREITA -1
#define NADA 2
#define VOLTAR -2

void ligar_sensores() //Funcoes_testadas
{
	SetSensorHTGyro(SENSOR_GYRO);
	//SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	//SetSensorUltrasonic(SENSOR_US_DIREITA);
	//SetSensorColorFull(SENSOR_COR_DIREITA);
	//SetSensorColorFull(SENSOR_COR_ESQUERDA);
	set_sensor_color(COR_ESQUERDA, VERMELHO);
	Wait(50);
	set_sensor_color(COR_DIREITA,  VERMELHO);

	
}


/* Fim das funcoes do takashi */

double raiz(double value)
{
	int i;
	double est = 1;
	for(i = 0; i < 20; i++)
	{
		est = est - (est*est-value)/(2*est);
	}
	return est;
}

task main()
{
	int left, right;
	double left_med, right_med, left_var, right_var;
	int i = 0, max = 10;
	char cores[3] = {VERMELHO, VERDE, AZUL};
	
	ligar_sensores();

	BTCheck(BT_CONN); // Checa a conexao

	SetSensorTouch(TOUCH); // Liga o sensor touch

	Wait(100);
	RemoteSetInputMode(BT_CONN, COR_ESQUERDA, SENSOR_TYPE_COLORRED, SENSOR_MODE_RAW); // Liga o sensor esquerdo de cor
	Wait(25);
	RemoteSetInputMode(BT_CONN, COR_DIREITA, SENSOR_TYPE_COLORRED, SENSOR_MODE_RAW);  // Liga o sensor direito de cor
	while(true)
	{
		if(Sensor(TOUCH) == 1)
		{
			i += 1;
			i %= 3;
			set_sensor_color(COR_ESQUERDA, cores[i]);
			Wait(25);
			set_sensor_color(COR_DIREITA, cores[i]);
			while(Sensor(TOUCH) == 1)
			{
				Wait(100);
			}
		}
		left_med  = 0;
		right_med = 0;
		left_var  = 0;
		right_var = 0;
		for(i = 0; i < max; i++)
		{
			left   = get_value_color(COR_ESQUERDA);
			right  = get_value_color(COR_DIREITA);
			left_med  += left;
			right_med += right;
			left_var  += left*left;
			right_var += right*right;
		}
		left_med  /= max;
		right_med /= max;
		left_var  /= max;
		right_var /= max;
		left_var  -= left_med;
		right_var -= right_var;
		left_var  = raiz(left_var);
		right_var = raiz(right_var);
		ClearScreen();
		NumOut(40, LCD_LINE4, left_med);
		NumOut(70, LCD_LINE4, right_med);
		NumOut(40, LCD_LINE6, left_var);
		NumOut(70, LCD_LINE6, right_var);
		Wait(100);
	}
}
