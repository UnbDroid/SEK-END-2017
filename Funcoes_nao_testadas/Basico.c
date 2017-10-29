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
/* Funcao do rodrigo que funciona para quando os sensores de cor estao no cérebro principal 
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
int identifica_cor()
{
	// Assumimos aqui que o robô ja esta em cima de uma esquina
	const int DELAY = 30, max = 10;
	int CORES[6] = {VERMELHO, VERDE, AZUL, BRANCO, PRETO, FORA};
	int cor_left[7], cor_right[7];
	int i, j, re_left, re_right, sum;
	int RED[7][2] = {
		{0,   0}, 	// NONE
		{200, 230}, // PRETO
		{240, 310}, // AZUL
		{350, 420}, // VERDE
		{380, 420}, // FORA
		{520, 550}, // VERMELHO
		{555, 620}, // BRANCO
	};
	int GREEN[7][2] = {
		{0,   0}, 	// NONE
		{130, 180}, // PRETO
		{220, 310}, // AZUL
		{370, 460}, // VERDE
		{340, 390}, // FORA
		{320, 400}, // VERMELHO
		{490, 560}, // BRANCO
	};
	int BLUE[7][2] = {
		{0,   0}, 	// NONE
		{120, 210}, // PRETO
		{260, 340}, // AZUL
		{300, 410}, // VERDE
		{310, 380}, // FORA
		{310, 400}, // VERMELHO
		{470, 550}, // BRANCO
	};
	while(1)
	{
		for(i = 0; i < 3; i++) // Esse for percorre vermelho, verde e azul.
		{
			set_sensor_color(COR_ESQUERDA, CORES[i]);
			sum = 0;
			for(j = 0; j < max; j++)
				sum += get_value_color(COR_ESQUERDA);
			cor_left[CORES[i]] 	= sum/max;
			Wait(DELAY);
			set_sensor_color(COR_DIREITA,  CORES[i]);
			sum = 0;
			for(j = 0; j < max; j++)
				sum += get_value_color(COR_DIREITA);
			cor_right[CORES[i]] = sum/max;
			Wait(DELAY);	
		}
		re_left		= -1; // valor lixo
		re_right	= -1;
		for(i = 0; i < 6; i++)
		{
			if(RED[CORES[i]][0]  <= cor_left[CORES[i]] && cor_left[CORES[i]] <= RED[CORES[i]][1])
			{
				if(BLUE[CORES[i]][0]  <= cor_left[CORES[i]] && cor_left[CORES[i]] <= BLUE[CORES[i]][1])
				{
					if(GREEN[CORES[i]][0]  <= cor_left[CORES[i]] && cor_left[CORES[i]] <= GREEN[CORES[i]][1])
					{
						re_left = CORES[i];
					}
				}
			}
			if(RED[CORES[i]][0]  <= cor_right[CORES[i]] && cor_right[CORES[i]] <= RED[CORES[i]][1])
			{
				if(BLUE[CORES[i]][0]  <= cor_right[CORES[i]] && cor_right[CORES[i]] <= BLUE[CORES[i]][1])
				{
					if(GREEN[CORES[i]][0]  <= cor_right[CORES[i]] && cor_right[CORES[i]] <= GREEN[CORES[i]][1])
					{
						re_right = CORES[i];
					}
				}
			}
		}
		if(re_left == re_right)
		{
			if(re_left == )
			{
				PlayTone(880, 500);
			}
			return re_left;	
		}
	}

}

int reto(int passageiros) // Essa funcao e executada ate retornar preto
{
	/* Essa funcao manda ir reto ate achar uma esquina, fim de linha ou algum passageiro(se o robo nao estiver cheio) */
	const int n = 20;
	int cor_left, cor_right, us_left, us_right;
	int contador = 0; /* Contador para ocntar a quanto tempo um sensor esta fora da pista para no fim saber se achou bloco preto */
	OnRevSync(AMBOS_MOTORES, VELOCIDADE_MEDIA, 0);
	while(1) // Loop principal, indica que ainda esta na regiao branca
	{
		cor_left	= get_value_color(COR_ESQUERDA);
		cor_right	= get_value_color(COR_DIREITA);
		us_left		= US_esquerda();
		us_right	= US_direita();

		cor_left 	= trata_cor(cor_left); // Devolve se e preto ou branco 
		cor_right 	= trata_cor(cor_right);

		if(US_achou_algo(us_left, us_right) && passageiros < 4) /* Se achou algum boneco e e possivel pegar mais */
		{
			Off(AMBOS_MOTORES);
			return -2; /* Codigo que achou boneco */
		}
		else if(cor_left == BRANCO && cor_right == BRANCO)
		{
			contador = 0;									// O contador para evitar que entre em um loop infinito(nas esquinas talvez)
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
			if(cor_left == PRETO && cor_right == PRETO) // Se chegou em uma esquina
			{
				while(1) // Aqui ele comeca a ajeitar, para caso chegue torto, ele fique certo no final
				{
					Off(AMBOS_MOTORES);
					OnRev(MOTOR_ESQUERDA, (-1)*VELOCIDADE_BAIXA);
					contador = 0;
					while(trata_cor(get_value_color(COR_ESQUERDA)) == PRETO && contador < 3)
					{ // Ajusta a roda esquerda para ficar em cima da linha.
						Wait(100);
						contador += 1;
					}
					Off(MOTOR_ESQUERDA);
					OnRev(MOTOR_DIREITA,  (-1)*VELOCIDADE_BAIXA);
					contador = 0;
					while(trata_cor(get_value_color(COR_DIREITA)) == PRETO && contador < 3)
					{ // Ajusta a roda esquerda para ficar em cima da linha.
						Wait(100);
						contador += 1;
					}
					if(trata_cor(get_value_color(COR_ESQUERDA)) != PRETO && trata_cor(get_value_color(COR_DIREITA)) != PRETO)
					{
						OnRevSync(AMBOS_MOTORES, VELOCIDADE_MEDIA, 0);
						Wait(300);
						Off(AMBOS_MOTORES);
						return PRETO;
					}
				}
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
	int direcao[7] = {NADA, NADA, NADA, NADA, NADA, NADA};
	int passageiros = 0, cor_achada;
	ligar_sensores();
	if(existe_arquivo_info())
	{
		/* Codigo para pegar do arquivo e gravar nas direcoes */
	}
	
	while(1)
	{
		reto(passageiros);
		cor_achada = identifica_cor();
		if(direcao[cor_achada] != NADA)
		{

		}
		else
		{
			/* Aqui vai chutar o caminho */
		}
		Wait(10000);
	}
}	