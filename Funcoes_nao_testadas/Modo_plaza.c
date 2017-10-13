#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define MOTORES OUT_AC
//#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B /*conexÃ£o com o outro cÃ©rebro*/
#define SENSOR_COR_ESQUERDA IN_2
#define SENSOR_COR_DIREITA IN_3
//#define SENSOR_US_ESQUERDA IN_3
//#define SENSOR_US_DIREITA IN_4
#define SENSOR_GYRO IN_1 /*teste*/
#define SENSOR_US_GARRA IN_4 /*teste*/
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 85
#define PRETO 1
#define VERDE 3
#define BRANCO 6
#define AZUL 2
#define VERMELHO 5
#define AMARELO 4
#define SENSIBILIDADE 0.1
#define OFFSET_SAMPLES 2000
#define ESQUERDA 1
#define DIREITA 2
#define FRENTE 3
#define WHITEUP 630
#define WHITEDOWN 575
#define BLUEUP 340
#define	BLUEDOWN 300
#define	BLACKUP 220
#define BLACKDOWN 160
#define	REDUP 575
#define REDDOWN 545
#define GREENUP
#define GREENDOWN

void ligar_sensores()
{
	SetSensorHTGyro(SENSOR_GYRO);
	SetSensorColorRed(SENSOR_COR_DIREITA);
	SetSensorColorRed(SENSOR_COR_ESQUERDA);
	SetSensorUltrasonic(SENSOR_US_GARRA);
}

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

  Off(MOTORES);

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

	Off(MOTORES);
}

void distancia_reto(int low_speed, int high_speed, int distancia)
{
	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	OnRev(MOTORES, high_speed);
	do{
	    if (count_A - MotorRotationCount(MOTOR_ESQUERDA) > count_C - MotorRotationCount(MOTOR_DIREITA))
	    {
	       OnRev(MOTOR_ESQUERDA, low_speed);
	       until ((count_C - MotorRotationCount(MOTOR_DIREITA)) >  (count_A - MotorRotationCount(MOTOR_ESQUERDA)));
	       OnRev(MOTOR_ESQUERDA, high_speed);

	    }
	    else
	    {
	         OnRev(MOTOR_DIREITA, low_speed);
	         until ( (count_A - MotorRotationCount(MOTOR_ESQUERDA)) > (count_C - MotorRotationCount(MOTOR_DIREITA)));
	         OnRev(MOTOR_DIREITA, high_speed);
	    }
	}while((count_A - MotorRotationCount(MOTOR_ESQUERDA))*6*PI/360 <= distancia);
	Off(MOTORES);
}

void distancia_re(int low_speed, int high_speed, int distancia)
{
	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	OnFwd(MOTORES, high_speed);
	do{
		if (MotorRotationCount(MOTOR_ESQUERDA) - count_A > MotorRotationCount(MOTOR_DIREITA) - count_C)
		{
		   OnFwd(MOTOR_ESQUERDA, low_speed);
		   until ((MotorRotationCount(MOTOR_DIREITA) - count_C) >  (MotorRotationCount(MOTOR_ESQUERDA) - count_A));
		   OnFwd(MOTOR_ESQUERDA, high_speed);

		}
		else
		{
		     OnFwd(MOTOR_DIREITA, low_speed);
		     until ( (MotorRotationCount(MOTOR_ESQUERDA) - count_A) > (MotorRotationCount(MOTOR_DIREITA) - count_C));
		     OnFwd(MOTOR_DIREITA, high_speed);
		}
	}while((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia);
	Off(MOTORES);
}

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

void modo_plaza ()
{
	int aux, prev_motor;

	ResetRotationCount(MOTOR_DIREITA);
	ResetRotationCount(MOTOR_ESQUERDA);
	OnFwdSync(MOTORES, -VELOCIDADE_ALTA, 0);
	while(!intervalo_cor(PRETO, SENSOR_COR_ESQUERDA) && !intervalo_cor(PRETO, SENSOR_COR_DIREITA));
	Off(MOTORES);
	aux = abs(MotorRotationCount(MOTOR_DIREITA));

	girar(180);
	distancia_re(VELOCIDADE_BAIXA, VELOCIDADE_ALTA, 20);
	OnFwd(MOTOR_PORTA, VELOCIDADE_BAIXA);
	Wait(900);
	Off(MOTOR_PORTA);
	distancia_reto(VELOCIDADE_BAIXA, VELOCIDADE_ALTA, 30);
	OnRev(MOTOR_PORTA, VELOCIDADE_BAIXA);
	Wait(900);
	Off(MOTOR_PORTA);

	ResetRotationCount(MOTOR_DIREITA);
	while(prev_motor < aux)
	{
		SetSensorColorFull(SENSOR_COR_DIREITA);
		SetSensorColorRed(SENSOR_COR_DIREITA);
		OnFwdSync(MOTORES, -VELOCIDADE_ALTA, 0);
		Wait(50);
		prev_motor = abs(MotorRotationCount(MOTOR_DIREITA));
	}
	Off(MOTORES);
}


task main ()
{
 	ligar_sensores();
	modo_plaza();
}