#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define AMBOS_MOTORES OUT_AC
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
#define VELOCIDADE_ALTA 50
#define PRETO 1
#define VERDE 3
#define BRANCO 6
#define AZUL 2
#define VERMELHO 5
#define AMARELO 4
#define SENSIBILIDADE 0.1
#define OFFSET_SAMPLES 2000
#define ESQUERDA 1
#define DIREITA (-1)
#define FRENTE 0
#define WHITEUP_B 170
#define WHITEDOWN_B 100
#define BLUEUP_B 130
#define	BLUEDOWN_B 85
#define	BLACKUP_B 85
#define BLACKDOWN_B 30
#define	REDUP_B 50
#define REDDOWN_B 20
#define GREENUP_B 110
#define GREENDOWN_B 60
#define WHITEUP_G 220
#define WHITEDOWN_G 120
#define BLUEUP_G 115
#define	BLUEDOWN_G 70
#define	BLACKUP_G 100
#define BLACKDOWN_G 50
#define	REDUP_G 60
#define REDDOWN_G 20
#define GREENUP_G 160
#define GREENDOWN_G 115
#define WHITEUP_R 220
#define WHITEDOWN_R 120
#define BLUEUP_R 105
#define	BLUEDOWN_R 50
#define	BLACKUP_R 100
#define BLACKDOWN_R 50
#define	REDUP_R 180
#define REDDOWN_R 110
#define GREENUP_R 120
#define GREENDOWN_R 65
#define OFFSET_COLOR 1/3.0

#define OFFSET_SAMPLES 2000

void ligar_sensores()
{
	SetSensorHTGyro(SENSOR_GYRO);
	SetSensorColorFull(SENSOR_COR_DIREITA);
	SetSensorColorFull(SENSOR_COR_ESQUERDA);
	SetSensorUltrasonic(SENSOR_US_GARRA);
}

void distancia_reto(int low_speed, int high_speed, float distancia)
{
	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	OnRev(AMBOS_MOTORES, high_speed);
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
	}while((count_A - MotorRotationCount(MOTOR_ESQUERDA))*5*PI/360 <= distancia);
	Off(AMBOS_MOTORES);
}

void distancia_re(int low_speed, int high_speed, float distancia)
{
	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	OnFwd(AMBOS_MOTORES, high_speed);
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
	}while((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*5*PI/360 <= distancia);
	Off(AMBOS_MOTORES);
}

float getGyroOffset() 
{
	SetSensorHTGyro(SENSOR_GYRO);
	float gyro_sum = 0, i;

	for(i = 0; i < OFFSET_SAMPLES; ++i)
	{
		gyro_sum += SensorHTGyro(SENSOR_GYRO);
	}

	return gyro_sum/OFFSET_SAMPLES;
}

void giro(float degrees) // Algoritimo usado pela sek do ano passado //testada
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
			ClearLine(LCD_LINE1);
			TextOut(0, LCD_LINE1, "ANGLE:");
			NumOut(48, LCD_LINE1, angle);
			Wait(100); //MUDAR OS VALORES DOS WAITS PARA ALTERAR AS POSIÇÕES DAS RODAS
			Off(MOTORES);
			OnRev(MOTOR_ESQUERDA, -VELOCIDADE_ALTA);
			OnRev(MOTOR_DIREITA, -VELOCIDADE_ALTA);
			Wait(20); //USANDO 100 E 20 AS RODAS E AS CASTER BALLS ESTÃO FICANDO DENTRO DO QUADRADO, SWEET, DUDE !
			Off(MOTORES);
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
			ClearLine(LCD_LINE1);
			TextOut(0, LCD_LINE1, "ANGLE:");
			NumOut(48, LCD_LINE1, angle);
		  	Wait(100);
			Off(MOTORES);
			OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_ALTA);
			OnFwd(MOTOR_DIREITA, -VELOCIDADE_ALTA);
		  	Wait(20);
			Off(MOTORES);
		}
	}

	Off(MOTORES);
}

void girar(float degrees) //função para mover o robo de acordo com o giro e girar, valores de acordo com testes
{
	if (degrees == 90 || degrees == -90)
	{
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 15.5);
		giro(degrees);
	}
	if (degrees == 180)
	{
		giro(90);
		Wait(2000);
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 3.5);
		giro(90);		
	}
	if (degrees != 90 && degrees != -90 && degrees != 180)
		giro(degrees);
	
}


int sensor_cor(int sensor) //funcao estilo colorfull
{
	int red = 0, green = 0, blue = 0, i;
	for(i = 0; i < 3; i++)
	{
		red += ColorSensorValue(sensor, INPUT_RED)*OFFSET_COLOR;
	}
	for(i = 0; i < 3; i++)
	{
		green += ColorSensorValue(sensor, INPUT_GREEN)*OFFSET_COLOR;
	}
	for(i = 0; i < 3; i++)
	{
		blue += ColorSensorValue(sensor, INPUT_BLUE)*OFFSET_COLOR;
	}
	if(red >= REDDOWN_R && red <= REDUP_R && green >= REDDOWN_G && green <= REDUP_G && blue >= REDDOWN_B && blue <= REDUP_B)
		return VERMELHO;

	if(red >= GREENDOWN_R && red <= GREENUP_R && green >= GREENDOWN_G && green <= GREENUP_G && blue >= GREENDOWN_B && blue <= GREENUP_B)
		return VERDE;
	
	if(red >= BLUEDOWN_R && red <= BLUEUP_R && green >= BLUEDOWN_G && green <= BLUEUP_G && blue >= BLUEDOWN_B && blue <= BLUEUP_B)
		return AZUL;
	
	if(red >= BLACKDOWN_R && red <= BLACKUP_R && green >= BLACKDOWN_G && green <= BLACKUP_G && blue >= BLACKDOWN_B && blue <= BLACKUP_B)
		return PRETO;
	
	if(red >= WHITEDOWN_R && green >= WHITEDOWN_G && blue >= WHITEDOWN_B)
		return BRANCO;

	return PRETO;
}

void modo_plaza ()
{
	int aux, prev_motor;

	ResetRotationCount(MOTOR_DIREITA);
	ResetRotationCount(MOTOR_ESQUERDA);
	OnRevSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
	while(sensor_cor(SENSOR_COR_ESQUERDA) != PRETO && sensor_cor(SENSOR_COR_DIREITA) != PRETO);
	Off(AMBOS_MOTORES);
	PlayTone(880, 500);
	aux = abs(MotorRotationCount(MOTOR_DIREITA));

	//girar(180);
	distancia_re(VELOCIDADE_BAIXA, VELOCIDADE_ALTA, 15);
	OnFwd(MOTOR_PORTA, VELOCIDADE_BAIXA);
	Wait(900);
	Off(MOTOR_PORTA);
	distancia_reto(VELOCIDADE_BAIXA, VELOCIDADE_ALTA, 30);
	OnRev(MOTOR_PORTA, VELOCIDADE_BAIXA);
	Wait(900);
	Off(MOTOR_PORTA);

	ResetRotationCount(MOTOR_DIREITA);
	Wait(50);
	while(prev_motor < aux)
	{
		OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
		Wait(50);
		prev_motor = abs(MotorRotationCount(MOTOR_DIREITA));
	}
	Off(AMBOS_MOTORES);
}


task main ()
{
 	int direcoes[6] = {2, 2, 2, 2, 2, 2};//1: Preto

 	ligar_sensores();

 	/*while(1){
		OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
		while(sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO && sensor_cor(SENSOR_COR_DIREITA) == BRANCO);
		if (sensor_cor(SENSOR_COR_ESQUERDA) == VERMELHO && sensor_cor(SENSOR_COR_DIREITA) == VERMELHO)
		{
			Wait(210);//testar esse tempo
			if (sensor_cor(SENSOR_COR_ESQUERDA == AZUL) && sensor_cor(SENSOR_COR_DIREITA) == AZUL)
			{
				modo_plaza();
				break;
			}
		}
	}*/
	while(1){
		OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
		while(sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO && sensor_cor(SENSOR_COR_DIREITA) == BRANCO);
		if (sensor_cor(SENSOR_COR_ESQUERDA) == PRETO && sensor_cor(SENSOR_COR_DIREITA) == PRETO)
		{
			if (direcoes[AZUL] != 2 && direcoes[VERDE] != 2 && direcoes[VERMELHO] != 2)
			{
				girar(180);

			}
		}
	}

}