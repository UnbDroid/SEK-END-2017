#define MOTOR_ESQUERDA OUT_C
#define MOTOR_DIREITA OUT_A
#define AMBOS_MOTORES OUT_AC
#define SENSOR_GYRO IN_1 /*teste*/

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 45
#define VELOCIDADE_ALTA 65
#define POT_MAX 90

#define CORRECAO 0.01
#define RAZAO 1.286

void ligar_sensores()
{
	SetSensorHTGyro(SENSOR_GYRO);
}



void retinho(int velocidadeR)
{
	float erro;
	int velo1 = 0, velo2 = 0, velocidadeL = velocidadeR * RAZAO;

	if(velocidadeL > POT_MAX) velocidadeL = POT_MAX;
	if(velocidadeL < -POT_MAX) velocidadeL = -POT_MAX;
	if(velocidadeR > POT_MAX) velocidadeR = POT_MAX;
	if(velocidadeR < -POT_MAX) velocidadeR = -POT_MAX;
	OnFwd(MOTOR_DIREITA, -velocidadeR);
	OnFwd(MOTOR_ESQUERDA, -velocidadeL);
	Wait(100);
	while(erro > -0.5 && erro < 0.5){
		erro = SensorHTGyro(SENSOR_GYRO);
	}
	velo1 = velocidadeR - CORRECAO * erro;
	velo2 = velocidadeL + CORRECAO * erro;
	if (velo1 > POT_MAX) velo1 = POT_MAX;
	else if (velo1 < -POT_MAX) velo1 = -POT_MAX;
	if (velo2 > POT_MAX) velo2 = POT_MAX;
	else if (velo2 < -POT_MAX) velo2 = -POT_MAX;
	Off(AMBOS_MOTORES);
	OnFwd(MOTOR_DIREITA, -velo1);
	OnFwd(MOTOR_ESQUERDA, -velo2);
}

task main()
{
	ligar_sensores();
	while(1)
	{
		retinho(VELOCIDADE_ALTA);
	}
}