#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define AMBOS_MOTORES OUT_AC

#define SENSOR_COR_ESQUERDA IN_1
#define SENSOR_COR_DIREITA IN_2

#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65

#define PRETO 1
#define AZUL 2
#define VERDE 3
#define AMARELO 4
#define VERMELHO 5
#define BRANCO 6

#define WHITEUP 630
#define WHITEDOWN 575
#define BLUEUP 340
#define	BLUEDOWN 300
#define	BLACKUP 220
#define BLACKDOWN 160
#define	REDUP 575
#define REDDOWN 545
#define GREENUP 330 /* valor ficticio, so para compilar */
#define GREENDOWN 440 /* valor ficticio, so para compilar */

bool intervalo_cor(int cor, int sensor)
{
	if (cor == BRANCO)
	{
		if (SensorRaw(sensor) >= WHITEDOWN && SensorRaw(sensor) <= WHITEUP)
			return true;
	}
	if (cor == AZUL)
	{
		if (SensorRaw(sensor) >= BLUEDOWN && SensorRaw(sensor) <= BLUEUP)
			return true;
	}
	if (cor == PRETO)
	{
		if (SensorRaw(sensor) >= BLACKDOWN && SensorRaw(sensor) <= BLACKUP)
			return true;
	}
	if (cor == VERMELHO)
	{
		if (SensorRaw(sensor) >= REDDOWN && SensorRaw(sensor) <= REDUP)
			return true;
	}
	return false;
}

void reto(int cor)
{
	while (intervalo_cor(cor, SENSOR_COR_ESQUERDA) || intervalo_cor(cor, SENSOR_COR_DIREITA))
	{
		OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 5);
		while(!intervalo_cor(cor, SENSOR_COR_ESQUERDA) && intervalo_cor(cor, SENSOR_COR_DIREITA))
		{
			OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_ALTA);
			OnRev(MOTOR_DIREITA, - VELOCIDADE_MEDIA);
			ClearScreen();
			TextOut(50,50, "E:P");
			NumOut(0,0, Sensor(SENSOR_COR_ESQUERDA));
			NumOut(0,40, Sensor(SENSOR_COR_DIREITA));
		}
		while(!intervalo_cor(cor, SENSOR_COR_DIREITA) && intervalo_cor(cor, SENSOR_COR_ESQUERDA))
		{
			OnFwd(MOTOR_DIREITA, -VELOCIDADE_ALTA);
			OnRev(MOTOR_ESQUERDA, - VELOCIDADE_MEDIA);
			ClearScreen();
			TextOut(50,50, "D:P");
			NumOut(0,0, Sensor(SENSOR_COR_ESQUERDA));
			NumOut(0,40, Sensor(SENSOR_COR_DIREITA));
		}
		ClearScreen();
		NumOut(0,0, Sensor(SENSOR_COR_ESQUERDA));
		NumOut(0,40, Sensor(SENSOR_COR_DIREITA));
	}
	Off(AMBOS_MOTORES);
	
	//alterei o laço para dentro da função recenbendo a cor como argumento
}

bool verificar_direcao(int cor)
{
	reto(cor);
	reto(BRANCO);

	if (!intervalo_cor(PRETO, SENSOR_COR_DIREITA) && !intervalo_cor(PRETO, SENSOR_COR_ESQUERDA))
	{
		TextOut(10,10, "caminho certo");
		Wait(2000);
		return true;
	}
	TextOut(10, 10, "Caminho errado");
	Wait(2000);
	return false;
}

