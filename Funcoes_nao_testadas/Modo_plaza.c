#define CONEXAO 1 // Numero da porta BlueTooth

#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define AMBOS_MOTORES OUT_AC
//#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B /*conexÃ£o com o outro cÃ©rebro*/
#define SENSOR_COR_ESQUERDA IN_4
#define SENSOR_COR_DIREITA IN_1
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

#define WHITEUP_R 580
#define WHITEDOWN_R 550
#define BLUEUP_R 235
#define	BLUEDOWN_R 220
#define	BLACKUP_R 200
#define BLACKDOWN_R 190
#define	REDUP_R 530
#define REDDOWN_R 500
#define GREENUP_R 320
#define GREENDOWN_R 290
#define OFFSET_COLOR 1/3.0

#define OFFSET_SAMPLES 2000

sub BTCheck(){
     if (!BluetoothStatus(CONEXAO)==NO_ERR){
          ClearScreen();
          TextOut(5,LCD_LINE2,"Erro");
          Wait(1000);
          Stop(true);
      }
}

void set_sensor_color(char porta, char color)
{
     if(color == VERMELHO){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORRED, SENSOR_MODE_RAW);
     }else if(color == VERDE){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORGREEN, SENSOR_MODE_RAW);
     }else if(color == AZUL){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORBLUE, SENSOR_MODE_RAW);
     }else if(color == BRANCO){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORFULL, SENSOR_MODE_RAW);
     }
}

int get_value_color(char porta)
{
     InputValuesType params;

     params.Port = porta;
     RemoteGetInputValues(CONEXAO, params);

     return params.RawValue;
}

void ligar_sensores()
{
	SetSensorHTGyro(SENSOR_GYRO);
	set_sensor_color(SENSOR_COR_ESQUERDA, VERMELHO);
	Wait(50);
	set_sensor_color(SENSOR_COR_DIREITA, VERMELHO);
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

void girar(float degrees) // Algoritimo usado pela sek do ano passado //testada
{

	SetSensorHTGyro(SENSOR_GYRO);

	float angle = 0, gyro = 0;
	unsigned long time = CurrentTick(), prev_time;

	Off(AMBOS_MOTORES);

	degrees = -degrees;

	float offset = getGyroOffset();

	distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 3.5);

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
			Off(AMBOS_MOTORES);
			OnRev(MOTOR_ESQUERDA, -VELOCIDADE_ALTA);
			OnRev(MOTOR_DIREITA, -VELOCIDADE_ALTA);
			Wait(20); //USANDO 100 E 20 AS RODAS E AS CASTER BALLS ESTÃO FICANDO DENTRO DO QUADRADO, SWEET, DUDE !
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
			ClearLine(LCD_LINE1);
			TextOut(0, LCD_LINE1, "ANGLE:");
			NumOut(48, LCD_LINE1, angle);
		  	Wait(100);
			Off(AMBOS_MOTORES);
			OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_ALTA);
			OnFwd(MOTOR_DIREITA, -VELOCIDADE_ALTA);
		  	Wait(20);
			Off(AMBOS_MOTORES);
		}
	}

	Off(AMBOS_MOTORES);
}


int sensor_cor(int sensor) //funcao estilo colorfull
{
	int value = 0, i;
	for(i = 0; i < 3; i++)
	{
		value += get_value_color(sensor)*OFFSET_COLOR;
	}
	if(value >= REDDOWN_R && value <= REDUP_R)
		return VERMELHO;

	if(value >= GREENDOWN_R && value <= GREENUP_R)
		return VERDE;

	if(value >= BLUEDOWN_R && value <= BLUEUP_R)
		return AZUL;

	if(value >= BLACKDOWN_R && value <= BLACKUP_R)
		return PRETO;

	if(value >= WHITEDOWN_R)
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
	Wait(2000);
	Off(AMBOS_MOTORES);
	OnRevSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
	while(sensor_cor(SENSOR_COR_ESQUERDA) != PRETO && sensor_cor(SENSOR_COR_DIREITA) != PRETO);
	Off(AMBOS_MOTORES);
	PlayTone(880, 500);
	aux = abs(MotorRotationCount(MOTOR_DIREITA));

	//girar(180);
	distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 15);
	OnFwd(MOTOR_PORTA, VELOCIDADE_BAIXA);
	Wait(900);
	Off(MOTOR_PORTA);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 30);
	OnRev(MOTOR_PORTA, VELOCIDADE_BAIXA);
	Wait(900);
	Off(MOTOR_PORTA);

	ResetRotationCount(MOTOR_DIREITA);
	Wait(50);
	while(prev_motor < aux)
	{
    PlayTone(880, 500);
    OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
		Wait(50);
		prev_motor = abs(MotorRotationCount(MOTOR_DIREITA));
	}
	Off(AMBOS_MOTORES);
}


task main ()
{
	BTCheck();
 	int direcoes[6] = {1, 1, 1, 1, 1, 1};

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
	/*while(1){
		OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
		Wait(1000);
		Off(AMBOS_MOTORES);
		if (direcoes[AZUL] != 2 && direcoes[VERDE] != 2 && direcoes[VERMELHO] != 2)
		{
			distancia_re(VELOCIDADE_BAIXA, VELOCIDADE_ALTA, 5);
			girar(180);
			modo_plaza();

		}
	}*/
	OnRevSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 1.3);
	while(sensor_cor(SENSOR_COR_ESQUERDA) != VERMELHO || sensor_cor(SENSOR_COR_DIREITA) != VERMELHO);
	Off(AMBOS_MOTORES);
	PlayTone(880, 500);
	modo_plaza();

}