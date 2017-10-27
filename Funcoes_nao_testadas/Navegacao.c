#define BT_CONN 1

#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define MOTORES OUT_AC
#define AMBOS_MOTORES OUT_AC
#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B /*conexão com o outro cérebro*/

#define SENSOR_COR_ESQUERDA IN_4
#define SENSOR_COR_DIREITA IN_1
#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_4
#define SENSOR_GYRO IN_1 /*teste*/
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
#define FORA 7

#define SENSIBILIDADE 0.1
#define OFFSET_SAMPLES 2000

#define ESQUERDA 1
#define DIREITA 2
#define FRENTE 3

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
#define BLUEUP_B 320
#define	BLACKUP_B 200
#define	REDUP_B 250
#define GREENUP_B 350
#define BLUEUP_G 300
#define	BLACKUP_G 220
#define	REDUP_G 290
#define GREENUP_G 450
#define	REDUP_R 590
#define GREENUP_R 390

//defines mais importantes, separados pra facilitar quando tiver de mudar
#define WHITEDOWN_R 590
#define REDDOWN_R 550
#define BLACKUP_R 280
#define BLUEDOWN_R 250
#define BLUEUP_R 310
#define FORAUP_R 450
#define FORADOWN_R 350
#define DESVIO 20

sub BTCheck(int conn){
     if (!BluetoothStatus(conn)==NO_ERR){
          ClearScreen();
          TextOut(5,LCD_LINE2,"Erro");
          Wait(1000);
          Stop(true);
      }
}

void set_sensor_color(char porta, char color)
{
     if(color == VERMELHO){
          RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORRED, SENSOR_MODE_RAW);
     }else if(color == VERDE){
          RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORGREEN, SENSOR_MODE_RAW);
     }else if(color == AZUL){
          RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORBLUE, SENSOR_MODE_RAW);
     }else if(color == BRANCO){
          RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORFULL, SENSOR_MODE_RAW);
     }
}

void ligar_sensores() //testada
{
	SetSensorHTGyro(SENSOR_GYRO);
	SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	SetSensorUltrasonic(SENSOR_US_DIREITA);
	set_sensor_color(SENSOR_COR_ESQUERDA, VERMELHO);
	Wait(100);
	set_sensor_color(SENSOR_COR_DIREITA, VERMELHO);
	Wait(100);
		
}

int get_value_color(char porta)
{
	int leitura = 0, i;
     InputValuesType params;

     params.Port = porta;
    /* for (i = 0; i < 3; i++){
     	 RemoteGetInputValues(BT_CONN, params)
     	leitura += (params.RawValue)*(1/3.0);
     }*/
     RemoteGetInputValues(BT_CONN, params)
     leitura = params.RawValue;

     return leitura;
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

int teste_cor(int sensor)
{
	int leitura_r, leitura_g, leitura_b;
	leitura_r = get_value_color(sensor);
	Wait(50);
	set_sensor_color(sensor, VERDE);
	Wait(100);
	leitura_g = get_value_color(sensor);
	Wait(50);
	set_sensor_color(sensor, AZUL);
	Wait(100); 
	leitura_b = get_value_color(sensor);
	Wait(50);
	set_sensor_color(sensor, VERMELHO);
	Wait(100);
	if (leitura_r <= (BLACKUP_R + DESVIO) && (leitura_g <= (BLACKUP_G + DESVIO) || leitura_b <= (BLACKUP_B + DESVIO)))
		return PRETO;
	if (leitura_r <= (BLUEUP_R + DESVIO) && (leitura_g <= (BLUEUP_G + DESVIO) || leitura_b <= (BLUEUP_B + DESVIO)))
		return AZUL;
	if (leitura_r  <= (GREENUP_R + DESVIO) && (leitura_g <= (GREENUP_G + DESVIO) || leitura_b <= (GREENUP_B + DESVIO)))
		return VERDE;
	if (leitura_r <= (REDUP_R + DESVIO) && (leitura_g <= (REDUP_G + DESVIO) || leitura_b <= (REDUP_B + DESVIO)))
		return VERMELHO;

	return FORA;
}

void reto(int cor) //robo move ate que os dois sensores parem de ver a cor
{
	while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
	{
		OnFwdSync(MOTORES, -VELOCIDADE_MEDIA, 0);
		while(sensor_cor(SENSOR_COR_ESQUERDA) == FORA && sensor_cor(SENSOR_COR_DIREITA) == cor)
		{
			OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
			OnRev(MOTOR_DIREITA,  -VELOCIDADE_BAIXA);
			ClearScreen();
			TextOut(50,50, "E:P");
		}
		while(sensor_cor(SENSOR_COR_DIREITA) == FORA && sensor_cor(SENSOR_COR_ESQUERDA) == cor)
		{
			OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
			OnRev(MOTOR_ESQUERDA, -VELOCIDADE_BAIXA);
			ClearScreen();
			TextOut(50,50, "D:P");
		}
		ClearScreen();
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
	int contador;
	Off(MOTORES);
	ClearScreen();
	TextOut(0,0,"ajeitando");
	NumOut(10,10, cor);
	Wait(1000);

	while(sensor_cor(SENSOR_COR_DIREITA) != cor && sensor_cor(SENSOR_COR_ESQUERDA) != cor) // Aqui ele comeca a ajeitar, para caso chegue torto, ele fique certo no final
	{
		Off(AMBOS_MOTORES);
		OnRev(MOTOR_ESQUERDA, (-1)*VELOCIDADE_MEDIA);
		contador = 0;
		while(sensor_cor(SENSOR_COR_ESQUERDA) != cor && contador < 3)
		{ // Ajusta a roda esquerda para ficar em cima da linha.
			Wait(75);
			contador += 1;
		}
		Off(AMBOS_MOTORES);
		OnRev(MOTOR_DIREITA,  (-1)*VELOCIDADE_MEDIA);
		contador = 0;
		while(sensor_cor(SENSOR_COR_DIREITA) != cor && contador < 3)
		{ // Ajusta a roda esquerda para ficar em cima da linha.
			Wait(75);
			contador += 1;
		}
	}
	Off(MOTORES);
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
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 12);
		giro(degrees);
	}
	if (degrees == 180)
	{
		giro(90);
		Wait(2000);
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
/*
		OnRev(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
		Wait(200);
		Off(MOTORES);
		OnRev(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
		Wait(150);
		Off(MOTORES);*/

		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 13);
		giro(90);	
	}
	if (degrees != 90 && degrees != -90 && degrees != 180)
		giro(degrees);
	
}

bool verificar_direcao(int cor)
{
	int cor_d, cor_e;
	reto(cor);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 1.5);
	reto(BRANCO);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 2);

	cor_d = teste_cor(SENSOR_COR_DIREITA);
	cor_e = teste_cor(SENSOR_COR_ESQUERDA);

	if (cor_e != PRETO && cor_d != PRETO)//se os dois nao veem preto entao o robo acertou o caminho
	{
		TextOut(10,10, "caminho certo");
		Wait(2000);
		return true;
	}
	TextOut(10, 10, "Caminho errado");
	Wait(2000);
	return false;
}

void voltar(int cor)//voltar para o quadrado de origem visto que errou o caminho
{	
	ajeitar(BRANCO);
	girar(180);
	reto(BRANCO);
	PlayTone(880, 500);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 1.5);
	reto(cor);
	ajeitar(cor);
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

		if (vetor_direcao[AZUL] != FRENTE && vetor_direcao[VERMELHO] != FRENTE && vetor_direcao[VERDE] != FRENTE)
		{
			girar(90); 
	
			if (verificar_direcao(cor))
			{
				return FRENTE;
			}
			voltar(cor);
			girar(90);
			verificar_direcao(cor);
			return DIREITA;	
		} else 
		{
			verificar_direcao(cor);
			return DIREITA;
		}
	}
	if (vetor_direcao[AZUL] != FRENTE && vetor_direcao[VERMELHO] != FRENTE && vetor_direcao[VERDE] != FRENTE)
	{
		if (verificar_direcao(cor))
		{
			return FRENTE;
		}
		voltar(cor);
		girar(90);
		verificar_direcao(cor);
		return DIREITA;
	}
	verificar_direcao(cor);
	return DIREITA; 
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
	int cor_e, cor_d;
	BTCheck(BT_CONN);
	ligar_sensores();

	
while (true){
	reto(BRANCO);
	ajeitar(BRANCO);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
	cor_e = teste_cor(SENSOR_COR_ESQUERDA);
	cor_d = teste_cor(SENSOR_COR_DIREITA);

	if (cor_e == AZUL || cor_d == AZUL)
	{	
		PlayTone(440, 200);
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
	} else if (cor_e == VERMELHO || cor_d == VERMELHO)
	{
		PlayTone(440, 200);
		Wait(300);
		PlayTone(440, 200);
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
	} else if (cor_e == VERDE || cor_d == VERDE)
	{
		PlayTone(440, 200);
		Wait(300);
		PlayTone(440, 200);
		Wait(300);
		PlayTone(440, 200);
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
