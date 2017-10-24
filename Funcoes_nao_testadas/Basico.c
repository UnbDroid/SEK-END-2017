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

#define RETO 0
#define ESQUERDA 1
#define DIREITA -1
#define NADA 2


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
/* Funcao do rodrigo que funciona para quando os sensores de cor estao no principal 
int sensor_cor(int sensor)
{
	unsigned int rawData[];
	int red = 0, green = 1 , blue = 2, none = 3, result = ReadSensorColorRaw(sensor, rawData);

	if(rawData[red] < REDUP_R && rawData[red] > REDDOWN_R && rawData[green] < REDUP_G && rawData[green] > REDDOWN_G && rawData[blue] < REDUP_B && rawData[blue] > REDDOWN_B && rawData[none] < REDUP_N && rawData[none] > REDDOWN_N)
	{
		return VERMELHO;
	}

	if(rawData[red] < BLUEUP_R && rawData[red] > BLUEDOWN_R && rawData[green] < BLUEUP_G && rawData[green] > BLUEDOWN_G && rawData[blue] < BLUEUP_B && rawData[blue] > BLUEDOWN_B && rawData[none] < BLUEUP_N && rawData[none] > BLUEDOWN_N)
	{
		return AZUL;
	}
	
	if(rawData[red] < GREENUP_R && rawData[red] > GREENDOWN_R && rawData[green] < GREENUP_G && rawData[green] > GREENDOWN_G && rawData[blue] < GREENUP_B && rawData[blue] > GREENDOWN_B && rawData[none] < GREENUP_N && rawData[none] > GREENDOWN_N)
	{
		if (Sensor(sensor) == VERDE)
		{
				return VERDE;
		}
	}
	
	if(rawData[red] < WHITEUP_R && rawData[red] > WHITEDOWN_R && rawData[green] < WHITEUP_G && rawData[green] > WHITEDOWN_G && rawData[blue] < WHITEUP_B && rawData[blue] > WHITEDOWN_B && rawData[none] < WHITEUP_N && rawData[none] > WHITEDOWN_N)
	{
		return BRANCO;
	}

	return PRETO;
}*/



int US_esquerda()
{
	return 0;
}
int US_direita()
{
	return 0;
}
int US_achou_algo(int us_left, int us_right)
{
	return 0;
}

int trata_cor(int color)
{
	if(552 <= color && color <= 620)
		return BRANCO;
	else if(color < 500)
		return PRETO;
}

int reto(int passageiros)
{
	/* Essa funcao manda ir reto ate achar uma esquina, fim de linha ou algum passageiro(se o robo nao estiver cheio) */
	const int n = 20;
	int cor_left, cor_right, us_left, us_right;
	int contador = 0; /* Contador para ocntar a quanto tempo um sensor esta fora da pista para no fim saber se achou bloco preto */
	OnRevSync(AMBOS_MOTORES, VELOCIDADE_MEDIA, 0);
	while(1)
	{
		cor_left	= get_value_color(COR_ESQUERDA);
		cor_right	= get_value_color(COR_DIREITA);
		us_left		= US_esquerda();
		us_right	= US_direita();

		cor_left 	= trata_cor(cor_left);
		cor_right 	= trata_cor(cor_right);

		if(US_achou_algo(us_left, us_right) && passageiros < 4) /* Se achou algum boneco e e possivel pegar mais */
		{
			Off(AMBOS_MOTORES);
			return -2; /* Codigo que achou boneco */
		}
		else if(cor_left == BRANCO && cor_right == BRANCO)
		{
			contador = 0;
			OnRevSync(AMBOS_MOTORES, VELOCIDADE_MEDIA, 0);
		}
		else /* Entra aqui caso nao esteja no caminho certo ou ache alguma esquina */
		{
			contador += 1;
			/* 	Aqui esta o caso em que algum dos sensores nao ve branco 
				Entao temos duas possibilidades: Quando esta saindo fora da pista e quando esta no quadrado preto
				1) Para o caso de quando esta fora da pista, os dois sensores nao vao reconhecer preto, mas somente um
				2) Para o caso do quadrado preto, entao os dois sensores veem preto.

				As correcoes sao respectivamente:
				1) colocar uma potencia maior na roda que esta mais perto de cair
				2) retornar o valor indicando que achou um quadrado preto

				Contudo, existe a possibilidade do robo chegar torto e entao reconhecer como se estivesse caindo.
				Nesse caso, ele iria colocar uma potencia grande na roda que ja esta na frente e entao ficaria em circulos.
				Assim, introduzimos um contador: Se ele mantem a mesma opcao durante muito tempo, o contador chega a um limiar
				e faz a correcao necessaria
			*/
			if(cor_left == PRETO && cor_right == PRETO)
			{
				Off(AMBOS_MOTORES);
				return PRETO; /* Codigo do preto */
			}
			if(cor_left == PRETO && cor_right != PRETO)  
			{
				OnRev(MOTOR_ESQUERDA, VELOCIDADE_MEDIA);
				OnRev(MOTOR_DIREITA,  VELOCIDADE_BAIXA);
			}
			else if(cor_left != PRETO && cor_right == PRETO)
			{
				OnRev(MOTOR_ESQUERDA, VELOCIDADE_BAIXA);
				OnRev(MOTOR_DIREITA,  VELOCIDADE_MEDIA);
			}
		}
		Wait(100);
	}
}
int existe_arquivo_info()
{
	return 0;
}

task main()
{
	int direcao[6] = {NADA, NADA, NADA, NADA, NADA, NADA};
	int passageiros = 0;
	ligar_sensores();
	if(existe_arquivo_info())
	{
		/* Codigo para pegar do arquivo e gravar nas direcoes */
	}
	
	while(1)
	{
		reto(passageiros); // Essa funcao recebe o passageiros para saber se deve parar caso ache boneco ou nao
		/*if(cor_achada == PRETO)
			TextOut(10, 10, "Preto");
		else if(cor_achada == AZUL)
			TextOut(10, 10, "Azul");
		else if(cor_achada == VERMELHO)
			TextOut(10, 10, "Vermelho");
		else if(cor_achada == VERDE)
			TextOut(10, 10, "Verde");
		else
			TextOut(10, 10, "None!");*/
		Wait(2000);
	}
}	