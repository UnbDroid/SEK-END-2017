#define MOTOR_ESQUERDA OUT_C
#define MOTOR_DIREITA OUT_A
#define AMBOS_MOTORES OUT_AC

#define SENSOR_GYRO IN_1

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65
#define VELOCIDADE_ALTISSIMA 70

#define SENSIBILIDADE 0.9
#define OFFSET_SAMPLES 2000

#define CORRECAO 0.051

float getGyroOffset()
{
    float gyro_sum = 0, i;

    for(i = 0; i < OFFSET_SAMPLES; ++i)
    {
       gyro_sum += SensorHTGyro(SENSOR_GYRO);
    }

    return gyro_sum/OFFSET_SAMPLES;
}

void girar_sem_tempo(float degrees) // Algoritimo usado pela sek do ano passado //testada
{

	SetSensorHTGyro(SENSOR_GYRO);

	float angle = 0, gyro = 0;
	unsigned long time = CurrentTick(), prev_time;

	Off(AMBOS_MOTORES);

	degrees = -degrees;

	float offset = getGyroOffset();

	if(degrees > 0) {


		while(angle < degrees)
		{
			OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_ALTA);
			OnRev(MOTOR_DIREITA, -VELOCIDADE_ALTA);
			prev_time = time;
			time = CurrentTick();
			gyro = SensorHTGyro(SENSOR_GYRO);
			angle += (gyro - offset) * (time - prev_time)/1000.0;
			//ClearLine(LCD_LINE1);
			//TextOut(0, LCD_LINE1, "ANGLE:");
			//NumOut(48, LCD_LINE1, angle);
			Off(AMBOS_MOTORES);
		}
	} else {


		while(angle > degrees)
		{
			OnFwd(MOTOR_DIREITA, -VELOCIDADE_ALTA);
			OnRev(MOTOR_ESQUERDA, -VELOCIDADE_ALTA);
			prev_time = time;
			time = CurrentTick();
			gyro = SensorHTGyro(SENSOR_GYRO);
			angle += (gyro - offset) * (time - prev_time)/1000.0;
			//ClearLine(LCD_LINE1);
			//TextOut(0, LCD_LINE1, "ANGLE:");
			//NumOut(48, LCD_LINE1, angle);
			Off(AMBOS_MOTORES);
		}
	}

	Off(AMBOS_MOTORES);
}

task main()
{
	girar_sem_tempo(180);
}