void girar_sem_tempo_roda(float degrees, int motor) // Algoritimo usado pela sek do ano passado //testada
{

	SetSensorHTGyro(SENSOR_GYRO);

	float angle = 0, gyro = 0;
	unsigned long time = CurrentTick(), prev_time;

	Off(AMBOS_MOTORES);

	degrees = -degrees;

	float offset = getGyroOffset();

	int sinal;
	if(degrees > 0)
		sinal = 1;
	else
		sinal = -1;

	while(angle * sinal < degrees * sinal)
	{
		if(motor == MOTOR_ESQUERDA)
			OnRev(MOTOR_DIREITA, -sinal*VELOCIDADE_ALTA);
		else
			OnFwd(MOTOR_ESQUERDA, sinal*VELOCIDADE_ALTA);
		prev_time = time;
		time = CurrentTick();
		gyro = SensorHTGyro(SENSOR_GYRO);
		angle += (gyro - offset) * (time - prev_time)/1000.0;
		Off(AMBOS_MOTORES);	
	}
	Off(AMBOS_MOTORES);
}


void distancia_reto_safe(int low_speed, int high_speed, int distancia) // Ainda nao terminado
{
	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	int left, right;
	int flagl = 0, flagr = 0;
	int disl, disr;
	OnRev(AMBOS_MOTORES, high_speed);
	do{
		left = sensor_cor(SENSOR_COR_ESQUERDA);
		right = sensor_cor(SENSOR_COR_DIREITA);
		if(left == FORA || left == PRETO)
			flagl = 1;
		if(right == FORA || right == PRETO)
			flagr = 1;
		if(flagr || flagl)
			break;
		if (count_A - MotorRotationCount(MOTOR_ESQUERDA) > count_C - MotorRotationCount(MOTOR_DIREITA))
		{
			do {
				OnRev(MOTOR_ESQUERDA, low_speed);
			} while ((count_C - MotorRotationCount(MOTOR_DIREITA)) >  (count_A - MotorRotationCount(MOTOR_ESQUERDA)) && ((count_A - MotorRotationCount(MOTOR_ESQUERDA))*6*PI/360 <= distancia));
			OnRev(MOTOR_ESQUERDA, high_speed);
		}
		else
		{
			do {
				OnRev(MOTOR_DIREITA, low_speed);
			}while ( (count_A - MotorRotationCount(MOTOR_ESQUERDA)) > (count_C - MotorRotationCount(MOTOR_DIREITA)) && ((count_C - MotorRotationCount(MOTOR_DIREITA))*6*PI/360 <= distancia));
			OnRev(MOTOR_DIREITA, high_speed);
		}
		disl = (count_A - MotorRotationCount(MOTOR_ESQUERDA))*6*PI/360;
		disr = (count_C - MotorRotationCount(MOTOR_DIREITA))*6*PI/360;
	}while(disl <= distancia && disr <= distancia);
	if(disl <= distancia && !flagl)
	{
		OnRev(MOTOR_ESQUERDA, high_speed);
		do
		{
			disl = (count_A - MotorRotationCount(MOTOR_ESQUERDA))*6*PI/360;	
		}while(disl <= distancia);
	}
	else
		Off(MOTOR_ESQUERDA);
	if(disr <= distancia && !flagr)
	{
		OnRev(MOTOR_DIREITA, high_speed);
		do
		{
			disr = (count_C - MotorRotationCount(MOTOR_DIREITA))*6*PI/360;	
		}while(disr <= distancia);
	}
	else
		Off(MOTOR_DIREITA);
	Off(AMBOS_MOTORES);
}