#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define MOTORES Out_AC
#define MOTOR_GARRA OUT_B
#define SENSOR_COR_ESQUERDA IN_1
#define SENSOR_COR_DIREITA IN_2
#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_4
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_ALTA 65
#define PRETO 1
#define VERDE 3
#define BRANCO 6
#define AZUL 2
#define VERMELHO 5
#define AMARELO 4
#define SENSIBILIDADE 0.1

void reto()
{
	OnFwdSync(MOTORES, VELOCIDADE_BAIXA, 0);
}

void virar_esquerda()
{
	OnFwd(MOTOR_DIREITA, VELOCIDADE_BAIXA);
	OnRev(MOTOR_ESQUERDA, VELOCIDADE_BAIXA);
}

void virar_direita()
{
	OnFwd(MOTOR_ESQUERDA, VELOCIDADE_BAIXA);
	OnRev(MOTOR_DIREITA, VELOCIDADE_BAIXA);
}

void re()
{
	OnRevSync(MOTORES, VELOCIDADE_BAIXA, 0);
}

void agarrar()
{
	int posicao_inicial = MotorRotationCount(MOTOR_GARRA);
	while (MotorRotationCount(MOTOR_GARRA) - posicao_inicial < 270) //270 seria 1/4 de volta, suponho que seja o suficiente pra jogar o boneco (falta teste, btw)
	{
		OnFwd(MOTOR_GARRA, VELOCIDADE_BAIXA);
	}

	int posicao_secundaria = MotorRotationCount(MOTOR_GARRA);
	while (MotorRotationCount(MOTOR_GARRA) - posicao_secundaria < 270)
	{
		OnRev(MOTOR_GARRA, VELOCIDADE_BAIXA);
	}

}

float ultrassom_esquerda_filtrado()
{
	SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	float aux = SensorUS(SENSOR_US_ESQUERDA);
	float valor = aux;
	for (int i = 0; i < 5; ++i)
	{
		valor = valor * SENSIBILIDADE + SensorUS(SENSOR_US_ESQUERDA)*(1-SENSIBILIDADE); //Algoritimo passado pelo B.Andreguetti da aula de SisMed
	}

	return valor;
}


task main ()
{

	int help = 66;
	SetSensorUltrasonic(SENSOR_US_ESQUERDA);

	while (true)
	{
		help = SensorUS(SENSOR_US_ESQUERDA);
		ClearScreen();
		NumOut (0, LCD_LINE3, help);
		Wait(50);
	}

}
