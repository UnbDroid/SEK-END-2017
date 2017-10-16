#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define MOTORES OUT_AC
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

void ligar_sensores() //testada
{
	SetSensorHTGyro(SENSOR_GYRO);
	//SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	//SetSensorUltrasonic(SENSOR_US_DIREITA);
	SetSensorColorFull(SENSOR_COR_DIREITA);
	SetSensorColorFull(SENSOR_COR_ESQUERDA);
	
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

void reto(int cor)
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

float getGyroOffset() 
{
    float gyro_sum = 0, i;

    for(i = 0; i < OFFSET_SAMPLES; ++i)
    {
       gyro_sum += SensorHTGyro(SENSOR_GYRO);
    }

    return gyro_sum/OFFSET_SAMPLES;
}

void girar(float degrees) // Algoritimo passado, falta alterar pra função que o kaynã fez
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

bool verificar_direcao(int cor)
{
	reto(cor);
	reto(BRANCO);

	if (sensor_cor(SENSOR_COR_DIREITA) != PRETO && sensor_cor(SENSOR_COR_ESQUERDA) != PRETO)
	{
		TextOut(10,10, "caminho certo");
		Wait(2000);
		return true;
	}
	TextOut(10, 10, "Caminho errado");
	Wait(2000);
	return false;
}

void voltar()
{
	girar(180);
	reto(BRANCO);
}

int testar_caminho(int cor, int vetor_direcao[])
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
		voltar();
		girar(90); 
	if (vetor_direcao[AZUL] != FRENTE && vetor_direcao[VERMELHO] != FRENTE && vetor_direcao[VERDE] != FRENTE)
	{
		if (verificar_direcao(cor))
		{
		return FRENTE;
		}
		voltar();
		girar(180);	
	}
	girar(-90);	
	verificar_direcao(cor);
	return DIREITA;
	}
}	


void seguir_direcao(int vetor_direcao[], int cor)
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

task main () //precisa de melhorias, mas ta razoavel
{
	int direcoes[6] = {0, 0, 0, 0, 0, 0}; //achei mais prático criar um vetor de 6 posiçoes e usar as constantes como o valor do índice
	ligar_sensores();
	while (true){
	reto(BRANCO);
	Wait(2000);
	
	if (sensor_cor(SENSOR_COR_DIREITA) == AZUL || sensor_cor(SENSOR_COR_ESQUERDA) == AZUL)
	{	
		ClearScreen();
		TextOut(10,10, "ACHEI AZUL");
		reto(AZUL);
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
