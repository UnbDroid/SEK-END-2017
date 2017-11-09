#define MOTOR_ESQUERDA OUT_C
#define MOTOR_DIREITA OUT_A
#define AMBOS_MOTORES OUT_AC

#define SENSOR_GYRO IN_1

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65
#define VELOCIDADE_ALTISSIMA 80

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

void distancia_reto(int low_speed, int high_speed, int distancia){//função do kaynã
	
	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	OnRev(AMBOS_MOTORES, high_speed);
	do{
		if (count_A - MotorRotationCount(MOTOR_ESQUERDA) > count_C - MotorRotationCount(MOTOR_DIREITA))
		{
			OnRev(MOTOR_ESQUERDA, low_speed);
			until ((count_C - MotorRotationCount(MOTOR_DIREITA)) >  (count_A - MotorRotationCount(MOTOR_ESQUERDA)) || ((count_A - MotorRotationCount(MOTOR_ESQUERDA))*6*PI/360 <= distancia));
			OnRev(MOTOR_ESQUERDA, high_speed);
		}
		else
		{
			OnRev(MOTOR_DIREITA, low_speed);
			until ( (count_A - MotorRotationCount(MOTOR_ESQUERDA)) > (count_C - MotorRotationCount(MOTOR_DIREITA)) || ((count_C - MotorRotationCount(MOTOR_DIREITA))*6*PI/360 <= distancia));
			OnRev(MOTOR_DIREITA, high_speed);
		}
	}while((count_A - MotorRotationCount(MOTOR_ESQUERDA))*6*PI/360 <= distancia);
	Off(AMBOS_MOTORES);
}

void distancia_re(int low_speed, int high_speed, int distancia){//função do Kaynã

	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	OnFwd(AMBOS_MOTORES, high_speed);
	do{
		if (MotorRotationCount(MOTOR_ESQUERDA) - count_A > MotorRotationCount(MOTOR_DIREITA) - count_C)
		{
			OnFwd(MOTOR_ESQUERDA, low_speed);
			until (((MotorRotationCount(MOTOR_DIREITA) - count_C) >  (MotorRotationCount(MOTOR_ESQUERDA) - count_A)) || ((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia));
			OnFwd(MOTOR_ESQUERDA, high_speed);
		}
		else
		{
			OnFwd(MOTOR_DIREITA, low_speed);
			until ( ((MotorRotationCount(MOTOR_ESQUERDA) - count_A) > (MotorRotationCount(MOTOR_DIREITA) - count_C)) || ((MotorRotationCount(MOTOR_DIREITA) - count_C)*6*PI/360 <= distancia));
			OnFwd(MOTOR_DIREITA, high_speed);
		}
	}while(((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia) && ((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia));
	Off(AMBOS_MOTORES);

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
			OnRev(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
			prev_time = time;
			time = CurrentTick();
			gyro = SensorHTGyro(SENSOR_GYRO);
			angle += (gyro - offset) * (time - prev_time)/1000.0;
			//ClearLine(LCD_LINE1);
			//TextOut(0, LCD_LINE1, "ANGLE:");
			//NumOut(48, LCD_LINE1, angle);
		}
	} else {


		while(angle > degrees)
		{
			OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
			OnRev(MOTOR_ESQUERDA, -VELOCIDADE_ALTA);
			prev_time = time;
			time = CurrentTick();
			gyro = SensorHTGyro(SENSOR_GYRO);
			angle += (gyro - offset) * (time - prev_time)/1000.0;
			//ClearLine(LCD_LINE1);
			//TextOut(0, LCD_LINE1, "ANGLE:");
			//NumOut(48, LCD_LINE1, angle);
		}
	}

	Off(AMBOS_MOTORES);
}

task main()
{
	istancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 3);
	girar_sem_tempo(180);
}