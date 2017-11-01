#define CONEXAO 1 // Numero da porta BlueTooth

#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define AMBOS_MOTORES OUT_AC
#define MOTORES OUT_AC
#define MOTOR(p,s) RemoteSetOutputState(CONEXAO, p, s, \
OUT_MODE_MOTORON+OUT_MODE_BRAKE+OUT_MODE_REGULATED, \
OUT_REGMODE_SPEED, 0, OUT_RUNSTATE_RUNNING, 0)
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
#define VELOCIDADE_ALTA 65
#define VELOCIDADE_ALTISSIMA 90
#define PRETO 1
#define VERDE 3
#define BRANCO 6
#define AZUL 2
#define VERMELHO 5
#define AMARELO 4
#define FORA 7

#define SENSIBILIDADE 0.1
#define OFFSET_SAMPLES 2000
#define ESQUERDA 1
#define DIREITA (-1)
#define FRENTE 0

#define WHITEUP_B 550
#define WHITEDOWN_B 470
#define	BLUEDOWN_B 260
#define BLACKDOWN_B 120
#define FORAUP_B 380
#define FORADOWN_B 310
#define REDDOWN_B 310
#define GREENDOWN_B 300
#define WHITEUP_G 560
#define WHITEDOWN_G 490
#define	BLUEDOWN_G 220
#define BLACKDOWN_G 130
#define REDDOWN_G 320
#define GREENDOWN_G 370
#define FORAUP_G 390
#define FORADOWN_G 340
#define WHITEUP_R 620
#define BLACKDOWN_R 190
#define GREENDOWN_R 350

//defines secundarios
#define BLUEUP_B 310
#define	BLACKUP_B 200
#define	REDUP_B 220
#define GREENUP_B 310
#define BLUEUP_G 280
#define	BLACKUP_G 210
#define	REDUP_G 240
#define GREENUP_G 435
#define	REDUP_R 590
#define GREENUP_R 385

//defines mais importantes, separados pra facilitar quando tiver de mudar
#define WHITEDOWN_R 590
#define REDDOWN_R 550
#define BLACKUP_R 250
#define BLUEDOWN_R 220
#define BLUEUP_R 310
#define FORAUP_R 450
#define FORADOWN_R 350
#define DESVIO 20
#define OFFSET_COLOR 1/9.0

#define OFFSET_SAMPLES 2000

#define CORRECAO 0.051


int direcoes[6] = {1, 1, 1, 1, 1, 1};

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

int sensor_cor(int sensor)
{	
	int leitura = get_value_color(sensor);

	if (sensor == SENSOR_COR_ESQUERDA)
		NumOut(0,0, leitura);
	if(sensor == SENSOR_COR_DIREITA)
		NumOut(0,40, leitura);

	if(leitura >= WHITEDOWN_R){
		return BRANCO;
	}
	else if (leitura >= REDDOWN_R - DESVIO){
		return VERMELHO;
	} else if (leitura <= BLACKUP_R){
		return PRETO;
	} else if (leitura >= (BLUEDOWN_R - DESVIO) && leitura <= (BLUEUP_R + DESVIO)){
		return AZUL;
	} else 	if (leitura <= FORAUP_R && leitura >= FORADOWN_R){
		return FORA;
	}
	return FORA;
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
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 12);
		giro(degrees);
	}
	if (degrees == 180)
	{
		giro(90);
		while(sensor_cor(SENSOR_COR_ESQUERDA) != FORA && sensor_cor(SENSOR_COR_DIREITA) != FORA)
			OnFwdSync(MOTORES, -VELOCIDADE_MEDIA, 0);
		Off(MOTORES);

		while(sensor_cor(SENSOR_COR_DIREITA) == FORA){
			OnFwd(MOTOR_DIREITA, VELOCIDADE_MEDIA);
		}
		Off(MOTOR_DIREITA);

		while(sensor_cor(SENSOR_COR_ESQUERDA) == FORA)
		{
			OnFwd(MOTOR_ESQUERDA, VELOCIDADE_MEDIA);
		}
		Off(MOTOR_ESQUERDA);

		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 10);
		giro(90);	
	}
	if (degrees != 90 && degrees != -90 && degrees != 180)
		giro(degrees);
	
}

void retinho(int velocidade ) // se dif == 1: realizar função enquanto não for "cor", se n, enquanto for "cor"
{
	float gyro1, gyro2, erro, velo1 = velocidade, velo2 = velocidade;
	gyro1 = SensorHTGyro(SENSOR_GYRO);
	OnFwdSync(AMBOS_MOTORES, -velocidade, 5);
	Wait(100);
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
	Wait(250);
}

void modo_plaza ()
{
	int aux, prev_motor; //Essa ultima constante é para armazenar o ultimo parâmetro do Sync
	SetSensorHTGyro(SENSOR_GYRO);
	float angle = 0, gyro = 0, angle_inicial = 0, result, offset_velocidade = 0, tolerancia = 40.0;
	unsigned long time = CurrentTick(), prev_time;
	float offset = getGyroOffset();


	PlayTone(880, 500);

	distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
	girar(180);
	PlayTone(880, 500);


	//OnRevSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
	OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, -20);
	Wait(500);
	OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
	Wait(300);
	OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 18);
	Wait(600);
	Off(AMBOS_MOTORES);
	Wait(500);
	ResetRotationCount(MOTOR_DIREITA);
	ResetRotationCount(MOTOR_ESQUERDA);
	prev_time = time;
	time = CurrentTick();
	gyro = SensorHTGyro(SENSOR_GYRO);
	angle_inicial = (gyro - offset) * (time - prev_time)/1000.0;
	while (sensor_cor(SENSOR_COR_ESQUERDA) != PRETO && sensor_cor(SENSOR_COR_DIREITA) != PRETO) retinho(-VELOCIDADE_ALTISSIMA);
	OnRevSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
	Wait(3000);
	Off(AMBOS_MOTORES);
	while (sensor_cor(SENSOR_COR_ESQUERDA) != PRETO && sensor_cor(SENSOR_COR_DIREITA) != PRETO) retinho(-VELOCIDADE_ALTISSIMA);
	PlayTone(880, 500);
	aux = abs(MotorRotationCount(MOTOR_DIREITA));

	distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 15);
 	MOTOR(MOTOR_PORTA, -VELOCIDADE_BAIXA);
	Wait(400);
	MOTOR(MOTOR_PORTA, 0);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 30);
	MOTOR(MOTOR_PORTA, VELOCIDADE_BAIXA);
	Wait(400);
	MOTOR(MOTOR_PORTA, 0);

	ResetRotationCount(MOTOR_DIREITA);
	Wait(50);
	PlayTone(880, 500);
	while(prev_motor < aux)
	{
    	OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
		Wait(50);
		prev_motor = abs(MotorRotationCount(MOTOR_DIREITA));
	}
	Off(AMBOS_MOTORES);
	for (int i = 0; i < 5; ++i)
	{
		direcoes[i] = -direcoes[i];
	}
}


task main ()
{
	BTCheck();

 	ligar_sensores();
 	Wait(1000);
 	MOTOR(MOTOR_PORTA, 20);
 	Wait(200);
	MOTOR(MOTOR_PORTA, 0);

 	
	OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
	while(sensor_cor(SENSOR_COR_ESQUERDA) != FORA && sensor_cor(SENSOR_COR_DIREITA) != FORA);
	Off(AMBOS_MOTORES);
	distancia_re(VELOCIDADE_BAIXA, VELOCIDADE_ALTA, 10);
	if(direcoes[AZUL] != 2 && direcoes[VERDE] != 2 && direcoes[VERMELHO] != 2) modo_plaza();
}
