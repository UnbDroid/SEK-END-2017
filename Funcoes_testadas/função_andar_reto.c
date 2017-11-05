#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define AMBOS_MOTORES OUT_AC
#define SENSOR_GYRO IN_1 /*teste*/

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 45
#define VELOCIDADE_ALTA 65

#define CORRECAO 0.051

void ligar_sensores()
{
	SetSensorHTGyro(SENSOR_GYRO);
}



void retinho(int velocidade) // se dif == 1: realizar função enquanto não for "cor", se n, enquanto for "cor"
{
	float gyro1, gyro2, erro, velo1 = velocidade, velo2 = velocidade;
	gyro1 = SensorHTGyro(SENSOR_GYRO);
	OnFwdSync(AMBOS_MOTORES, -velocidade, -12);
	Wait(50);
	while(gyro2 >= -0.5 && gyro2 <= 0.5){
		gyro2 = SensorHTGyro(SENSOR_GYRO);
	}
	erro = gyro2 - gyro1;
	velo1 = velocidade + CORRECAO * erro;
	velo2 = velocidade - CORRECAO * erro;
	if (velo1 > 90) velo1 = 90;
	else if (velo1 < -90) velo1 = -90;
	if (velo2 > 90) velo2 = 90;
	else if (velo2 < -90) velo2 = -90;
	OnRev(MOTOR_DIREITA, velo1);
	OnRev(MOTOR_ESQUERDA, velo2);
	Wait(150);
}

task main()
{
	ligar_sensores();
	while(1)
	{
		retinho(VELOCIDADE_ALTA);
	}
}