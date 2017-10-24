#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define MOTORES OUT_AC
#define AMBOS_MOTORES OUT_AC
#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B /*conexão com o outro cérebro*/
#define SENSOR_COR_ESQUERDA IN_1
#define SENSOR_COR_DIREITA IN_2
#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_4
#define SENSOR_GYRO IN_4 /*teste*/
#define SENSOR_US_GARRA IN_3 /*teste*/
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65
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
#define WHITEUP_B 650
#define WHITEDOWN_B 450
#define BLUEUP_B 450
#define	BLUEDOWN_B 250
#define	BLACKUP_B 250
#define BLACKDOWN_B 100
#define	REDUP_B 450
#define REDDOWN_B 250
#define GREENUP_B 450
#define GREENDOWN_B 250
#define WHITEUP_G 650
#define WHITEDOWN_G 450
#define BLUEUP_G 350
#define	BLUEDOWN_G 200
#define	BLACKUP_G 300
#define BLACKDOWN_G 100
#define	REDUP_G 450
#define REDDOWN_G 250
#define GREENUP_G 550
#define GREENDOWN_G 300
#define WHITEUP_R 750
#define WHITEDOWN_R 550
#define BLUEUP_R 350
#define	BLUEDOWN_R 200
#define	BLACKUP_R 350
#define BLACKDOWN_R 150
#define	REDUP_R 700
#define REDDOWN_R 475
#define GREENUP_R 475
#define GREENDOWN_R 350
#define WHITEUP_N 450
#define WHITEDOWN_N 150
#define BLUEUP_N 200
#define	BLUEDOWN_N 0
#define	BLACKUP_N 200
#define BLACKDOWN_N 0
#define	REDUP_N 400
#define REDDOWN_N 200
#define GREENUP_N 300
#define GREENDOWN_N 100

void ligar_sensores() //testada
{
	SetSensorHTGyro(SENSOR_GYRO);
	//SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	//SetSensorUltrasonic(SENSOR_US_DIREITA);
	SetSensorColorFull(SENSOR_COR_DIREITA);
	SetSensorColorFull(SENSOR_COR_ESQUERDA);
	
}

int sensor_cor(int sensor)
{
	unsigned int rawData[];
	int red = 0, green = 1 , blue = 2, none = 3, result = ReadSensorColorRaw(sensor, rawData);

	if(rawData[red] < REDUP_R && rawData[red] > REDDOWN_R && rawData[green] < REDUP_G && rawData[green] > REDDOWN_G && rawData[blue] < REDUP_B && rawData[blue] > REDDOWN_B && rawData[none] < REDUP_N && rawData[none] > REDDOWN_N)
		return VERMELHO;

	if(rawData[red] < BLUEUP_R && rawData[red] > BLUEDOWN_R && rawData[green] < BLUEUP_G && rawData[green] > BLUEDOWN_G && rawData[blue] < BLUEUP_B && rawData[blue] > BLUEDOWN_B && rawData[none] < BLUEUP_N && rawData[none] > BLUEDOWN_N)
		return AZUL;
	
	if(rawData[red] < GREENUP_R && rawData[red] > GREENDOWN_R && rawData[green] < GREENUP_G && rawData[green] > GREENDOWN_G && rawData[blue] < GREENUP_B && rawData[blue] > GREENDOWN_B && rawData[none] < GREENUP_N && rawData[none] > GREENDOWN_N)
		if (Sensor(sensor) == VERDE)
				return VERDE;
	
	if(rawData[red] < WHITEUP_R && rawData[red] > WHITEDOWN_R && rawData[green] < WHITEUP_G && rawData[green] > WHITEDOWN_G && rawData[blue] < WHITEUP_B && rawData[blue] > WHITEDOWN_B && rawData[none] < WHITEUP_N && rawData[none] > WHITEDOWN_N)
		return BRANCO;

	return PRETO;
}

void reto(int cor) //robo move ate que os dois sensores parem de ver a cor
{
	while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
	{
		OnFwdSync(MOTORES, -VELOCIDADE_MEDIA, 0);
		while(sensor_cor(SENSOR_COR_ESQUERDA) == PRETO && sensor_cor(SENSOR_COR_DIREITA) == cor)
		{
			OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
			OnRev(MOTOR_DIREITA, - VELOCIDADE_BAIXA);
			ClearScreen();
			TextOut(50,50, "E:P");
			NumOut(0,0, sensor_cor(SENSOR_COR_ESQUERDA));
			NumOut(0,40, sensor_cor(SENSOR_COR_DIREITA));
		}
		while(sensor_cor(SENSOR_COR_DIREITA) == PRETO && sensor_cor(SENSOR_COR_ESQUERDA) == cor)
		{
			OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
			OnRev(MOTOR_ESQUERDA, - VELOCIDADE_BAIXA);
			ClearScreen();
			TextOut(50,50, "D:P");
			NumOut(0,0, sensor_cor(SENSOR_COR_ESQUERDA));
			NumOut(0,40, sensor_cor(SENSOR_COR_DIREITA));
		}
		ClearScreen();
		NumOut(0,0, sensor_cor(SENSOR_COR_ESQUERDA));
		NumOut(0,40, sensor_cor(SENSOR_COR_DIREITA));
	}
	Off(MOTORES);
	
	//alterei o laço para dentro da função recenbendo a cor como argumento

}

void distancia_reto(int low_speed, int high_speed, int distancia){//função do kaynã
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
	Off(AMBOS_MOTORES);
}

void ajeitar(int cor) //arruma o robo pra ficar alinhado no quadrado da cor que recebe
{
	Off(MOTORES);

	while(sensor_cor(SENSOR_COR_DIREITA) != cor)
		OnFwd(MOTOR_DIREITA, VELOCIDADE_MEDIA);

	Off(MOTOR_DIREITA);

	while(sensor_cor(SENSOR_COR_ESQUERDA) != cor)
		OnFwd(MOTOR_ESQUERDA, VELOCIDADE_MEDIA);

	Off(MOTOR_ESQUERDA);
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

bool verificar_direcao(int cor)
{
	reto(cor);
	PlayTone(440, 100);
	reto(BRANCO);

	if (sensor_cor(SENSOR_COR_DIREITA) != PRETO && sensor_cor(SENSOR_COR_ESQUERDA) != PRETO)//se os dois nao veem preto entao o robo acertou o caminho
	{
		TextOut(10,10, "caminho certo");
		Wait(2000);
		return true;
	}
	TextOut(10, 10, "Caminho errado");
	Wait(2000);
	return false;
}

void voltar(int cor)//voltar para o quadrado de origem visto que errou co caminho
{
	girar(180);
	reto(BRANCO);
	PlayTone(880, 500);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 1.5);
	reto(cor);
}

int testar_caminho(int cor, int vetor_direcao[])//testa as direções verificando se ja há alguma cor com a direção
{
	if (vetor_direcao[AZUL] != ESQUERDA && vetor_direcao[VERMELHO] != ESQUERDA && vetor_direcao[VERDE] != ESQUERDA)
	{
		TextOut(10,10, "teste esquerda");
		Wait(2000);
		ClearScreen();
		girar(90);
		ClearScreen();

		if (verificar_direcao(cor))
		{
			return ESQUERDA;
		}
		voltar(cor);
		ajeitar(cor);
		girar(90); 
	if (vetor_direcao[AZUL] != FRENTE && vetor_direcao[VERMELHO] != FRENTE && vetor_direcao[VERDE] != FRENTE)
	{
		if (verificar_direcao(cor))
		{
		return FRENTE;
		}
		voltar(cor);
		ajeitar(cor);
		girar(180);	
	}
	girar(-90);	
	verificar_direcao(cor);
	return DIREITA;
	}
}	


void seguir_direcao(int vetor_direcao[], int cor)//função que sera usada quando a cor ja tiver uma direção definida
{
	if (vetor_direcao[cor] == ESQUERDA)
	{
		girar(90);
	}
	if (vetor_direcao[cor] == DIREITA)
	{
		girar(-90);
	}
	while(sensor_cor(SENSOR_COR_ESQUERDA) == cor || sensor_cor(SENSOR_COR_DIREITA) == cor)
	{
		reto(cor);
	}
}



task main () //por enquato a maior parte está só com a lógica, tem que alterar as funções pra ele conseguir andar certinho e girar
{
	int direcoes[6] = {0, 0, 0, 0, 0, 0}; //achei mais prático criar um vetor de 6 posiçoes e usar as constantes como o valor do índice
	ligar_sensores();
	
	while (true){
	reto(BRANCO);
	Wait(2000);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 1.5);
	if (sensor_cor(SENSOR_COR_DIREITA) == AZUL || sensor_cor(SENSOR_COR_ESQUERDA) == AZUL)
	{	
		ClearScreen();
		TextOut(10,10, "ACHEI AZUL");
		reto(AZUL);
		ajeitar(AZUL);
		if (direcoes [AZUL] == 0)
		{
			direcoes[AZUL] = testar_caminho(AZUL, direcoes);
		} else 
		{
			seguir_direcao(direcoes, AZUL);		
		}
	}
  
  if (sensor_cor(SENSOR_COR_DIREITA) == VERMELHO || sensor_cor(SENSOR_COR_ESQUERDA) == VERMELHO)
	{
		reto(VERMELHO);
		Wait(2000);
		ajeitar(VERMELHO);
		if (direcoes [VERMELHO] == 0)
		{
			direcoes[VERMELHO] = testar_caminho(VERMELHO, direcoes);
		} else 
		{
			seguir_direcao(direcoes, VERMELHO);		
		}
	}
  
  if (sensor_cor(SENSOR_COR_DIREITA) == VERDE || sensor_cor(SENSOR_COR_ESQUERDA) == VERDE)
	{
		reto(VERDE);
		ajeitar(VERDE);
		if (direcoes [VERDE] == 0)
		{
			direcoes[VERDE] = testar_caminho(VERDE, direcoes);
		} else 
		{
			seguir_direcao(direcoes, VERDE);		
		}
	}
}
}
