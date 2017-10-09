/* Este codigo é a implementação das funções de giro do robo */

#define SENSOR_GYRO IN_4
#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define AMBOS_MOTORES OUT_AC

#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65

#define OFFSET_SAMPLES 2000

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

	Off(AMBOS_MOTORES);

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

	Off(AMBOS_MOTORES);
}