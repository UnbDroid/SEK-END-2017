#define MOTOR_ESQUERDA OUT_C
#define MOTOR_DIREITA OUT_C
#define AMBOS_MOTORES OUT_AC

#define SENSOR_COR_ESQUERDA IN_1
#define SENSOR_COR_DIREITA IN_2
// #define SENSOR_US_ESQUERDA IN_3
// #define SENSOR_US_DIREITA IN_4
#define SENSOR_GYRO IN_4 /*teste*/

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 30
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

#define WHITEUP_B 650
#define WHITEDOWN_B 450
#define BLUEUP_B 450
#define	BLUEDOWN_B 250
#define	BLACKUP_B 250
#define BLACKDOWN_B 100
#define	REDUP_B 450
#define REDDOWN_B 250
#define GREENUP_B 450
#define GREENDOWN_B 250
#define WHITEUP_G 650
#define WHITEDOWN_G 450
#define BLUEUP_G 350
#define	BLUEDOWN_G 200
#define	BLACKUP_G 300
#define BLACKDOWN_G 100
#define	REDUP_G 450
#define REDDOWN_G 250
#define GREENUP_G 550
#define GREENDOWN_G 300
#define WHITEUP_R 750
#define WHITEDOWN_R 550
#define BLUEUP_R 350
#define	BLUEDOWN_R 200
#define	BLACKUP_R 350
#define BLACKDOWN_R 150
#define	REDUP_R 700
#define REDDOWN_R 475
#define GREENUP_R 475
#define GREENDOWN_R 350
#define WHITEUP_N 450
#define WHITEDOWN_N 150
#define BLUEUP_N 200
#define	BLUEDOWN_N 0
#define	BLACKUP_N 200
#define BLACKDOWN_N 0
#define	REDUP_N 400
#define REDDOWN_N 200
#define GREENUP_N 300
#define GREENDOWN_N 100

void ligar_sensores() //testada
{
	SetSensorHTGyro(SENSOR_GYRO);
	//SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	//SetSensorUltrasonic(SENSOR_US_DIREITA);
	SetSensorColorFull(SENSOR_COR_DIREITA);
	SetSensorColorFull(SENSOR_COR_ESQUERDA);
	
}

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
}

int cor_esquerda()
{
	return sensor_cor(SENSOR_COR_ESQUERDA);
}
int cor_direita()
{
	return sensor_cor(SENSOR_COR_DIREITA);
}

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

int reto(int passageiros)
{
	/* Essa funcao manda ir reto ate achar uma esquina, fim de linha ou algum passageiro(se o robo nao estiver cheio) */
	int cor_left, cor_right, us_left, us_right;
	int contador = 0; /* Contador para ocntar a quanto tempo um sensor esta fora da pista para no fim saber se achou bloco preto */
	int seguranca = 0; /* Para o caso de dar bug */
	OnRev(MOTOR_ESQUERDA, VELOCIDADE_MEDIA);
	OnRev(MOTOR_DIREITA,  VELOCIDADE_MEDIA);
				
	while(1)
	{
		cor_left	= cor_esquerda();
		cor_right	= cor_direita();
		us_left		= US_esquerda();
		us_right	= US_direita();
		if(cor_left == PRETO)
			TextOut(10, LCD_LINE1, "Preto");
		else if(cor_left == AZUL)
			TextOut(10, LCD_LINE1, "Azul");
		else if(cor_left == VERMELHO)
			TextOut(10, LCD_LINE1, "Vermelho");
		else if(cor_left == VERDE)
			TextOut(10, LCD_LINE1, "Verde");
		else if(cor_left == AMARELO)
			TextOut(10, LCD_LINE1, "Amarelo");
		else if(cor_left == BRANCO)
			TextOut(10, LCD_LINE1, "Branco");
		else
			TextOut(10, LCD_LINE1, "None!");
		if(cor_right == PRETO)
			TextOut(10, LCD_LINE3, "Preto");
		else if(cor_right == AZUL)
			TextOut(10, LCD_LINE3, "Azul");
		else if(cor_right == VERMELHO)
			TextOut(10, LCD_LINE3, "Vermelho");
		else if(cor_right == VERDE)
			TextOut(10, LCD_LINE3, "Verde");
		else if(cor_right == AMARELO)
			TextOut(10, LCD_LINE3, "Amarelo");
		else if(cor_right == BRANCO)
			TextOut(10, LCD_LINE3, "Branco");
		else
			TextOut(10, LCD_LINE3, "None!");
		
		if(US_achou_algo(us_left, us_right) && passageiros < 4) /* Se achou algum boneco e e possivel pegar mais */
		{
			Off(AMBOS_MOTORES);
			return -2; /* Codigo que achou boneco */
		}
		else if(cor_left == BRANCO && cor_right == BRANCO)
		{
			OnRev(MOTOR_ESQUERDA, 70);
			OnRev(MOTOR_DIREITA,  70);
		}
		else if(cor_left != BRANCO || cor_right != BRANCO) /* Entra aqui caso nao esteja no caminho certo ou ache alguma esquina */
		{
			/* Aqui esta o caso em que algum dos sensores nao ve branco 
				Entao temos duas possibilidades: Quando esta saindo fora da pista e quando esta no quadrado preto
				1) Para o caso de quando esta fora da pista, os dois sensores nao vao reconhecer preto, mas somente um
				2) Para o caso do quadrado preto, entao os dois sensores veem preto.

				As correcoes sao respectivamente:
				1) colocar uma potencia maior na roda que esta mais perto de cair
				2) retornar o valor indicando que achou um quadrado preto

				Contudo, existe a possibilidade do robo chegar torto e entao reconhecer como se estivesse caindo.
				Nesse caso, ele iria colocar uma potencia grande na roda que ja esta na frente e entao ficaria em circulos.
				Assim, introduzimos um contador: Se ele mantem a mesma opcao durante muito tempo, o contador chega a um limiar.
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
				contador += 1;
			}
			else if(cor_right == PRETO && cor_left != PRETO)
			{
				OnRev(MOTOR_ESQUERDA, VELOCIDADE_BAIXA);
				OnRev(MOTOR_DIREITA,  VELOCIDADE_MEDIA);
				contador += 1;
			}
			else
			{
				contador = 0;
				/* Aqui algum dos sensores achou vermelho, azul ou verde */
				if(cor_left != BRANCO && cor_right == BRANCO)
				{
					OnRev(MOTOR_ESQUERDA, VELOCIDADE_BAIXA);
					OnRev(MOTOR_DIREITA,  0);
				}
				else if(cor_left == BRANCO && cor_right != BRANCO) /* Que e o mesmo que else if(cor_right != BRANCO)*/
				{
					OnRev(MOTOR_ESQUERDA, 0);
					OnRev(MOTOR_DIREITA,  VELOCIDADE_BAIXA);
				}
				seguranca = 0;
				do
				{
					/* Vai ficar dentro desse loop ate consertar a cor*/
					Wait(100);
					cor_left	= cor_esquerda();
					cor_right	= cor_direita();
					// seguranca  += 1; /* Faz com que nao fique mais de 2 segundos neste loop*/
				}while(cor_left != cor_right && seguranca < 20);
				if(cor_left == cor_right)
				{
					Off(AMBOS_MOTORES);
					return cor_left; /* Codigo da cor lida */
				}
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
	int chute = 0, cor_achada;
	int passageiros = 0;
	ligar_sensores();
	if(existe_arquivo_info())
	{
		/* Codigo para pegar do arquivo e gravar nas direcoes */
	}
	
	while(1)
	{
		cor_achada = reto(passageiros);
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
		NumOut(48, LCD_LINE5, cor_achada);
		Wait(2000);
	}
}	