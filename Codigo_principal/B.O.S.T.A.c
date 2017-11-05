#define BT_CONN 1

#define MOTOR_ESQUERDA OUT_C
#define MOTOR_DIREITA OUT_A
#define AMBOS_MOTORES OUT_AC
#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B /*conexão com o outro cérebro*/
#define MOTOR(p,s) RemoteSetOutputState(CONEXAO, p, s, \
OUT_MODE_MOTORON+OUT_MODE_BRAKE+OUT_MODE_REGULATED, \
OUT_REGMODE_SPEED, 0, OUT_RUNSTATE_RUNNING, 0)

#define SENSOR_COR_ESQUERDA IN_4 /*conexÃ£o com o outro cÃ©rebro*/
#define SENSOR_COR_DIREITA IN_1 /*conexÃ£o com o outro cÃ©rebro*/
#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_4
#define SENSOR_GYRO IN_1 /*teste*/
#define CONEXAO 1

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 45
#define VELOCIDADE_ALTA 65

#define PRETO 1
#define VERDE 3
#define BRANCO 6
#define AZUL 2
#define VERMELHO 5
#define AMARELO 4
#define FORA 7

#define SENSIBILIDADE 0.9
#define OFFSET_SAMPLES 2000

#define ESQUERDA 1
#define DIREITA -1
#define FRENTE 0
#define NADA 2

//defines mais importantes, separados pra facilitar quando tiver de mudar
//#define BLACKUP_R 250
//#define FORAUP_R 450
//#define FORADOWN_R 350
#define L_WHITEDOWN_R 570
#define L_REDDOWN_R 470
#define L_BLUEUP_R 270
#define L_GREENUP_R 255
#define R_WHITEDOWN_R 590
#define R_REDDOWN_R 530
#define R_BLUEUP_R 270
#define R_GREENUP_R 310

#define L_GREENDOWN_G 270
#define L_GREENUP_G 300
#define L_WHITEDOWN_G 495
#define R_GREENDOWN_G 330
#define R_GREENUP_G 365
#define R_WHITEDOWN_G 500

#define L_BLUEDOWN_B 240
#define L_BLUEUP_B 265
#define L_BLACKUP_B 180
#define R_BLUEDOWN_B 290
#define R_BLUEUP_B 310
#define R_BLACKUP_B 180
#define DESVIO 20

//defines do teste cor
/*#define WHITEDOWN 540
#define WHITEUP 595
#define BLACKDOWN 180
#define REDDOWN 410
#define GREENDOWN 335
#define BLUEDOWN 260
#define FORADOWN 350 
#define L_BLACKUP 260
#define L_REDUP 420
#define L_GREENUP 315
#define L_BLUEUP 267
#define L_FORAUP 342
#define R_BLACKUP 240
#define R_REDUP 486
#define R_GREENUP 280
#define R_BLUEUP 263
#define R_FORAUP 384 */
#define OFFSET_COLOR 1/9.0

#define CORRECAO 0.051

int passageiros = 0; //achei mais prático criar um vetor de 6 posiçoes e usar as constantes como o valor do índice

sub BTCheck(){
     if (!BluetoothStatus(CONEXAO)==NO_ERR){
          ClearScreen();
          TextOut(5,LCD_LINE2,"Erro");
          Wait(1000);
          Stop(true);
      }
}

// Inicio das funcoes para cores
void set_sensor_color(char porta, char color)
{
     if(color == VERMELHO){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORRED, 	 SENSOR_MODE_RAW);
     }else if(color == VERDE){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORGREEN, SENSOR_MODE_RAW);
     }else if(color == AZUL){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORBLUE,  SENSOR_MODE_RAW);
     }else if(color == BRANCO){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORFULL,  SENSOR_MODE_RAW);
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
	RemoteGetInputValues(CONEXAO, params)
	leitura = params.RawValue;

	return leitura;
}



int sensor_cor(int sensor)
{
	int leitura = get_value_color(sensor);
	/*for(int i = 0; i < 1/OFFSET_COLOR; i++) //Acho que precisamos filtraro sensor assim
	{
		leitura += get_value_color(sensor)*OFFSET_COLOR;
		Wait(10); 
	}*/
	if (sensor == SENSOR_COR_ESQUERDA){
		NumOut(0,0, leitura);
	
		if(leitura >= L_WHITEDOWN_R){
			return BRANCO;
		} else if (leitura >= L_REDDOWN_R){
			return VERMELHO;
		}/* else if (leitura <= BLACKUP_R){
			return PRETO; 
		}*/else if (leitura <= L_BLUEUP_R){
			return AZUL;
		} else if (leitura <= L_GREENUP_R){
			return VERDE;
		} else{
			return FORA;
		}
	}
	if (sensor == SENSOR_COR_DIREITA){
		NumOut(0,40, leitura);

		if(leitura >= R_WHITEDOWN_R){
			return BRANCO;
		} else if (leitura >= R_REDDOWN_R){
			return VERMELHO;
		} /*else if (leitura <= BLACKUP_R){
			return PRETO; 
		}*/else if (leitura <= R_BLUEUP_R){
			return AZUL;
		} else if (leitura <= R_GREENUP_R){
			return VERDE;
		} else{
			return FORA;
		}
	}
}

int sensor_cor_verde(int sensor)
{
	int leitura = get_value_color(sensor);

	/*for(int i = 0; i < 1/OFFSET_COLOR; i++) //Acho que precisamos filtraro sensor assim
	{
		leitura += get_value_color(sensor)*OFFSET_COLOR;
		Wait(10); 
	}*/
	if (sensor == SENSOR_COR_ESQUERDA)
	{
		if (leitura >= L_WHITEDOWN_G)
			return BRANCO;
		if (leitura >= (L_GREENDOWN_G - DESVIO) && leitura <= (L_GREENUP_G + DESVIO))
			return VERDE;
		else 
			return FORA;
	}
	if (sensor == SENSOR_COR_DIREITA)
	{
		if (leitura >= R_WHITEDOWN_G)
			return BRANCO;
		if (leitura >= (R_GREENDOWN_G - DESVIO) && leitura <= (R_GREENUP_G + DESVIO))
			return VERDE;
		else 
			return FORA;
	}
}

int get_leitura_rgb(int sensor)
{
	int leitura_r = 0, leitura_g = 0, leitura_b = 0;
	int i, leitura;
	for(int i = 0; i < 1/OFFSET_COLOR; i++)
	{
		leitura_r += get_value_color(sensor)*OFFSET_COLOR;
		Wait(50);
	}
	if (sensor == SENSOR_COR_ESQUERDA){
		if (leitura_r >= L_WHITEDOWN_R){
			return BRANCO;
		}
		if (leitura_r >= L_REDDOWN_R){
			return VERMELHO;
		}
	} else {
		if (leitura_r >= R_WHITEDOWN_R){
			return BRANCO;
		}
		if (leitura_r >= R_REDDOWN_R){
			return VERMELHO;
		}
	}
	//Wait(50);
	set_sensor_color(sensor, VERDE);
	Wait(100);
	for(int i = 0; i < 1/OFFSET_COLOR; i++)
	{
		leitura_g += get_value_color(sensor)*OFFSET_COLOR;
		Wait(50);
	}
	if (sensor == SENSOR_COR_ESQUERDA){
		if (leitura_g >= L_GREENDOWN_G && leitura_g <= L_GREENUP_G){
			set_sensor_color(sensor, VERMELHO);
			Wait(100);
			return VERDE;
		}
	} else {
		if (leitura_g >= L_GREENDOWN_G && leitura_g <= L_GREENUP_G){
			set_sensor_color(sensor, VERMELHO);
			Wait(100);
			return VERDE;	
		}
	}
	//Wait(50);
	set_sensor_color(sensor, AZUL);
	Wait(100);
	for(int i = 0; i < 1/OFFSET_COLOR; i++)
	{
		leitura_b += get_value_color(sensor)*OFFSET_COLOR;
		Wait(50);
	}
	if (sensor == SENSOR_COR_ESQUERDA){
		if (leitura_b >= L_BLUEDOWN_B && leitura_b <= L_BLUEDOWN_B){
			set_sensor_color(sensor, VERMELHO);
			Wait(100);
			return AZUL;
		}
		if (leitura_b <= L_BLACKUP_B){
			set_sensor_color(sensor, VERMELHO);
			Wait(100);
			return PRETO;
		}
	} else {
		if (leitura_b >= R_BLUEDOWN_B && leitura_b <= R_BLUEDOWN_B){
			set_sensor_color(sensor, VERMELHO);
			Wait(100);
			return AZUL;
		}
		if (leitura_b <= R_BLACKUP_B){
			set_sensor_color(sensor, VERMELHO);
			Wait(100);
			return PRETO;
		}
	}
	//Wait(50);
	set_sensor_color(sensor, VERMELHO);
	Wait(100);
	return FORA; // Se quisermos o valor entr 100 e 700
}

/*int trata_leitura(int sensor, int leitura)
{
	int cor;
	if (sensor == SENSOR_COR_ESQUERDA)
	{
		if (leitura <= L_BLACKUP){
			cor = PRETO;
		}else if (leitura <= L_BLUEUP){
			cor = AZUL;
		}else if (leitura <= L_GREENUP){
			cor = VERDE;
		}else if (leitura <= L_FORAUP){
			cor = FORA;
		}else if (leitura <= L_REDUP){
			cor = VERMELHO;
		}else{
			cor = BRANCO;
		}
	}
	if (sensor == SENSOR_COR_DIREITA)
	{
		if (leitura <= R_BLACKUP){
			cor = PRETO;
		}else if (leitura <= R_BLUEUP){
			cor = AZUL;
		}else if (leitura <= R_GREENUP){
			cor = VERDE;
		}else if (leitura <= R_FORAUP){
			cor = FORA;
		}else if (leitura <= R_REDUP){
			cor = VERMELHO;
		}else{
			cor = BRANCO;
		}
	}
	// Toca os sons
	int wait = 200, duration = 100, frequency = 1000;
	switch(cor)
	{
		case VERMELHO:
			PlayTone(frequency, duration);
			Wait(wait);
		case VERDE:
			PlayTone(frequency, duration);
			Wait(wait);
		case AZUL:
			PlayTone(frequency, duration);
			Wait(wait);
			Wait(wait);
		case BRANCO:
			PlayTone(frequency, duration);
			Wait(wait);
		case PRETO:
			PlayTone(frequency, duration);
			Wait(wait);
		case FORA:
			PlayTone(frequency, duration);
			Wait(wait);
	}
	return cor;		// Se quisermos o valor entre 1 e 7
}
*/
int teste_cor(int sensor)
{
	int cor, leitura;
	leitura = get_leitura_rgb(sensor);
	//cor 	= trata_leitura(leitura, sensor);
	return cor;
}

void get_two_rgb(int &leitura_e, int &leitura_d )
{
	// Essa funcao pega os dois valores simultaneamente, da esquerda e da direita
	int left[3], right[3];
	int CORES[3] = {VERMELHO, VERDE, AZUL};
	int Q_LEITURA = 9, DELAY = 30;
	float divide = 1.0/Q_LEITURA;
	int i, j;
	for(i = 0; i < 3; i++)
	{
		left[i] = 0;
		right[i] = 0;
		set_sensor_color(SENSOR_COR_ESQUERDA, CORES[i]);
		Wait(20);
		set_sensor_color(SENSOR_COR_DIREITA, CORES[i]);
		for(j = 0; j < Q_LEITURA; j++)
		{
			left[i] += get_value_color(SENSOR_COR_ESQUERDA);
			right[i] += get_value_color(SENSOR_COR_DIREITA);
			Wait(DELAY);
		}
		left[i]  *= divide;
		right[i] *= divide;
	}
	set_sensor_color(SENSOR_COR_ESQUERDA, VERMELHO);
	Wait(50);
	set_sensor_color(SENSOR_COR_DIREITA, VERMELHO);
	Wait(50);
	leitura_e = (5*left[0]+3*left[1]+1*left[2])/9;
	leitura_d = (5*right[0]+3*right[1]+1*right[2])/9;
	ClearScreen();
	NumOut(10,10,leitura_e);
	NumOut(10, 40, leitura_d);
	Wait(2000);
}
void teste_two_colors(int &left, int &right)
{
	int leitura_e = get_leitura_rgb(SENSOR_COR_ESQUERDA), leitura_d = get_leitura_rgb(SENSOR_COR_DIREITA);
	ClearScreen();
	NumOut(10,10,leitura_e);
	NumOut(10, 40, leitura_d);
	Wait(2000);
	left = leitura_e; //trata_leitura(SENSOR_COR_ESQUERDA, leitura_e);
	right = leitura_d; //trata_leitura(SENSOR_COR_DIREITA, leitura_d);
}
int preto_branco(int color)
{
	if(550 <= color)
		return BRANCO;
	else
		return PRETO;
}


// Fim das funcoes de cores
// Locomoção

void dist(int low_speed, int high_speed, int distancia)
{
	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	int sinal = 1;
	if(distancia < 0)
		sinal = -1;
	OnRev(AMBOS_MOTORES, sinal*high_speed);
	do{
		if ( sinal*(count_A - MotorRotationCount(MOTOR_ESQUERDA)) > sinal*(count_C - MotorRotationCount(MOTOR_DIREITA)))
		{
			OnRev(MOTOR_ESQUERDA, sinal*low_speed);
			until (sinal*(count_C - MotorRotationCount(MOTOR_DIREITA)) >  sinal*(count_A - MotorRotationCount(MOTOR_ESQUERDA)));
			OnRev(MOTOR_ESQUERDA, sinal*high_speed);
		}
		else
		{
			OnRev(MOTOR_DIREITA, sinal*low_speed);
			until ( sinal*(count_A - MotorRotationCount(MOTOR_ESQUERDA)) > sinal*(count_C - MotorRotationCount(MOTOR_DIREITA)));
			OnRev(MOTOR_DIREITA, sinal*high_speed);
		}
	}while(sinal*(count_A - MotorRotationCount(MOTOR_ESQUERDA))*6*PI/360 <= distancia);
	Off(AMBOS_MOTORES);

}

void distancia_reto(int low_speed, int high_speed, int distancia){//função do kaynã
	dist(low_speed, high_speed, distancia);
	/*
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
	*/
}


void distancia_re(int low_speed, int high_speed, int distancia){//função do Kaynã
	//dist(low_speed, high_speed, (-1)*distancia);

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
	int left, right;
	Off(AMBOS_MOTORES);
	ClearScreen();
	TextOut(0,0,"ajeitando");
	NumOut(10,10, cor);
	/*Wait(1000);
	left = sensor_cor(SENSOR_COR_DIREITA);
	right = sensor_cor(SENSOR_COR_ESQUERDA);
	while( left != cor || right != cor) // Aqui ele comeca a ajeitar, para caso chegue torto, ele fique certo no final
	{
		Off(AMBOS_MOTORES);
		contador = 0;
		left = sensor_cor(SENSOR_COR_ESQUERDA);
		if(left != cor)
		{
			OnRev(MOTOR_ESQUERDA, (-1)*VELOCIDADE_MEDIA);
			Wait(100);
			PlayTone(600, 100);
		}
		while(left != cor && contador < 3)
		{ // Ajusta a roda esquerda para ficar em cima da linha.
			Wait(100);
			PlayTone(600, 100);
			contador += 1;
			left = sensor_cor(SENSOR_COR_ESQUERDA);
		}
		Off(AMBOS_MOTORES);
		
		contador = 0;
		right = sensor_cor(SENSOR_COR_DIREITA);
		if(left != cor)
		{
			OnRev(MOTOR_DIREITA,  (-1)*VELOCIDADE_MEDIA);
			Wait(100);
			PlayTone(600, 100);
		}
		while(right != cor && contador < 3)
		{ // Ajusta a roda esquerda para ficar em cima da linha.
			Wait(100);
			PlayTone(600, 100);
			contador += 1;
			right = sensor_cor(SENSOR_COR_DIREITA);	
		}
	}
	Off(MOTORES);*/
	if (cor != VERDE){
		while(sensor_cor(SENSOR_COR_DIREITA) != cor && sensor_cor(SENSOR_COR_ESQUERDA) != cor)
			OnRevSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		while(sensor_cor(SENSOR_COR_DIREITA) != cor || sensor_cor(SENSOR_COR_ESQUERDA) != cor) // Aqui ele comeca a ajeitar, para caso chegue torto, ele fique certo no final
		{
			Off(AMBOS_MOTORES);
		
			contador = 0;
			while(sensor_cor(SENSOR_COR_ESQUERDA) != cor && contador < 3)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnRev(MOTOR_ESQUERDA, (-1)*VELOCIDADE_MEDIA);
				Wait(50);
				contador += 1;
			}
			Off(AMBOS_MOTORES);
			contador = 0;
			while(sensor_cor(SENSOR_COR_DIREITA) != cor && contador < 3)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnRev(MOTOR_DIREITA,  (-1)*VELOCIDADE_MEDIA);
				Wait(50);
				contador += 1;
			}
		}
	} else{
		set_sensor_color(SENSOR_COR_DIREITA, VERDE);
		Wait(100);
		set_sensor_color(SENSOR_COR_ESQUERDA, VERDE);
		Wait(100);
		while(sensor_cor_verde(SENSOR_COR_DIREITA) != cor && sensor_cor_verde(SENSOR_COR_ESQUERDA) != cor)
			OnRevSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		while(sensor_cor_verde(SENSOR_COR_DIREITA) != cor || sensor_cor_verde(SENSOR_COR_ESQUERDA) != cor) // Aqui ele comeca a ajeitar, para caso chegue torto, ele fique certo no final
		{
			Off(AMBOS_MOTORES);
		
			contador = 0;
			while(sensor_cor_verde(SENSOR_COR_ESQUERDA) != cor && contador < 3)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnRev(MOTOR_ESQUERDA, (-1)*VELOCIDADE_MEDIA);
				Wait(50);
				contador += 1;
			}
			Off(AMBOS_MOTORES);
			contador = 0;
			while(sensor_cor_verde(SENSOR_COR_DIREITA) != cor && contador < 3)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnRev(MOTOR_DIREITA,  (-1)*VELOCIDADE_MEDIA);
				Wait(50);
				contador += 1;
			}
		}
		Off(AMBOS_MOTORES);
		set_sensor_color(SENSOR_COR_DIREITA, VERMELHO);
		Wait(100);
		set_sensor_color(SENSOR_COR_ESQUERDA, VERMELHO);
		Wait(100);
	}
	Off(AMBOS_MOTORES);
	Wait(500);
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
void girar(float degrees) //função para mover o robo de acordo com o giro e girar, valores de acordo com testes
{
	int contador = 0;
	if (degrees == 90 || degrees == -90)
	{
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 12);
		giro(degrees);
	}
	if (degrees == 180)
	{
		giro(90);
		while(sensor_cor(SENSOR_COR_ESQUERDA) != FORA && sensor_cor(SENSOR_COR_DIREITA) != FORA)
			OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		Off(AMBOS_MOTORES);

		while(sensor_cor(SENSOR_COR_DIREITA) == FORA || sensor_cor(SENSOR_COR_ESQUERDA) == FORA) // Aqui ele comeca a ajeitar, para caso chegue torto, ele fique certo no final
		{
			Off(AMBOS_MOTORES);
		
			contador = 0;
			while(sensor_cor(SENSOR_COR_ESQUERDA) == FORA && contador < 3)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnRev(MOTOR_ESQUERDA, (-1)*VELOCIDADE_MEDIA);
				Wait(50);
				contador += 1;
			}
			Off(AMBOS_MOTORES);
			contador = 0;
			while(sensor_cor(SENSOR_COR_DIREITA) == FORA && contador < 3)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnRev(MOTOR_DIREITA,  (-1)*VELOCIDADE_MEDIA);
				Wait(50);
				contador += 1;
			}
		}

		while(sensor_cor(SENSOR_COR_DIREITA) == FORA){
			OnFwd(MOTOR_DIREITA, VELOCIDADE_MEDIA);
		}
		Off(MOTOR_DIREITA);

		while(sensor_cor(SENSOR_COR_ESQUERDA) == FORA)
		{
			OnFwd(MOTOR_ESQUERDA, VELOCIDADE_MEDIA);
		}
		Off(MOTOR_ESQUERDA);

		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 11);
		giro(90);
	}
	if (degrees != 90 && degrees != -90 && degrees != 180)
		giro(degrees);
}
// Fim da locomocao
// Inicio das funcoes para pegar boneco

float ultrassom_filtrado(int sensor) //testada
{
	float valor = SensorUS(sensor);
	float aux;
	for (int i = 0; i < 8; ++i)
	{
		aux = SensorUS(sensor);
		valor = valor * SENSIBILIDADE + aux * (1-SENSIBILIDADE); // Algoritimo passado pelo B.Andreguetti da aula de SisMed
	}
	return valor;
}

void levantar_garra() //testada
{
	int prev_motor = MotorRotationCount(MOTOR_GARRA);
	OnRev(MOTOR_GARRA, VELOCIDADE_BAIXA);
	Wait(50);
	while(prev_motor != MotorRotationCount(MOTOR_GARRA)) // a garra irá¡ se movimentar até travar na estrutura
	{
		prev_motor = MotorRotationCount(MOTOR_GARRA);
		Wait(50);
	}
	Off(MOTOR_GARRA);
}

void abaixar_garra() //testada
{
	int prev_motor = MotorRotationCount(MOTOR_GARRA);

	/*OnFwd(MOTOR_GARRA, VELOCIDADE_BAIXA);
	Wait(50);
	while(prev_motor != MotorRotationCount(MOTOR_GARRA)) // a garra irá se movimentar até travar na estrutura
	{
		prev_motor = MotorRotationCount(MOTOR_GARRA);
		Wait(50);
	}
	Off(MOTOR_GARRA);
	prev_motor = MotorRotationCount(MOTOR_GARRA);*/
	while(225 > abs(MotorRotationCount(MOTOR_GARRA) - prev_motor)) // rotação necessária para a garra fechar um pouco e não agarrar no chão
	{
		OnFwd(MOTOR_GARRA, VELOCIDADE_BAIXA);
		if (prev_motor == MotorRotationCount(MOTOR_GARRA))
		{
			Off(MOTOR_GARRA);
		}
	}

	Off(MOTOR_GARRA);
}

int agarrar()//testada
{
	int prev_motor;
	int prev_motor_segunda_etapa;
	int inicial_motor;
	int confirma_que_pegou;

	// aqui cabe uma função para movimentar o robô até que o sensor ultrassônico ache o boneco
	// valor de teste, mas já é uma distância que a garra consegue pegar o boneco

	inicial_motor = MotorRotationCount(MOTOR_GARRA);

	OnFwd(MOTOR_GARRA, -VELOCIDADE_BAIXINHA);
	Wait(50);
	while (MotorRotationCount(MOTOR_GARRA) != prev_motor)
	{
		prev_motor = MotorRotationCount(MOTOR_GARRA);
		Wait(50);
	}

	Off(MOTOR_GARRA);

	if (abs(prev_motor - inicial_motor) > 60) //quando o motor fecha a garra, o deslocamento angular diminui, ou seja, fica negativo. Portanto essa subtração resultara em um numero negativo
	{
		confirma_que_pegou = 0;
	}

	else
	{
		confirma_que_pegou = 1;
	}

	OnFwd(MOTOR_GARRA, -VELOCIDADE_MEDIA);
	Wait(50);
	while (MotorRotationCount(MOTOR_GARRA) != prev_motor_segunda_etapa)
	{
		prev_motor_segunda_etapa = MotorRotationCount(MOTOR_GARRA);
		Wait(50);
	}

	Off(MOTOR_GARRA);


	return confirma_que_pegou;
}

int pegar_passageiro ( int lado) //testado, mas precisa mudar a função gira para o robô girar no centro dele
{

	int confirma_que_pegou = 0;

	if(lado == DIREITA && passageiros < 3){ //Ainda é necessário adaptar a função agarrar() pra depois de ela agarrar, ela voltar para a
															   //posição que o robô estava antes. Além disso, colocar para verificar se pegou o boneco
		Off(AMBOS_MOTORES);
		Wait(500);
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 8);
		giro(-90);
		abaixar_garra();
		distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 20);
		Off(AMBOS_MOTORES);
		confirma_que_pegou = agarrar();
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 20);
		giro(90);
	}
	else if(lado == ESQUERDA && passageiros < 3){ //variavel lado valendo 0 corresponde a esquerda e valendo 1 corresponde à direta, bjs

		Off(AMBOS_MOTORES);
		Wait(500);
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 8);
		giro(90);
		abaixar_garra();
		distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 20);
		Off(AMBOS_MOTORES);
		confirma_que_pegou = agarrar();
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 20);
		giro(-80);
	}

	if (confirma_que_pegou == 1)
	{
		++passageiros;
	}

	return passageiros;
}

void procura_boneco()
{
	int l = ultrassom_filtrado(SENSOR_US_ESQUERDA);
	int d = ultrassom_filtrado(SENSOR_US_DIREITA);
	if (d <= 15)
	{
		pegar_passageiro(DIREITA);
	}
	if(l <= 15)
	{
		pegar_passageiro(ESQUERDA);
	}
	ClearScreen();
	NumOut(10, 30, l);
	NumOut(10, 10, d);
}

//Fim das funcoes para pegar boneco

// Inicio da mesclagem das funções

void retinho(int velocidade, int cor) // se dif == 1: realizar função enquanto não for "cor", se n, enquanto for "cor"
{
	float gyro1, gyro2, erro, velo1 = velocidade, velo2 = velocidade;
	gyro1 = SensorHTGyro(SENSOR_GYRO);
	OnFwdSync(AMBOS_MOTORES, -velocidade, -18);
	Wait(50);
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
	Wait(50);
}

void reto(int cor) //robo move ate que os dois sensores parem de ver a cor
{
	if (cor != VERDE){
		while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
		{
			OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
			//while (sensor_cor(SENSOR_COR_DIREITA) == cor && sensor_cor(SENSOR_COR_ESQUERDA) == cor)
			//	retinho(VELOCIDADE_MEDIA, cor);
			//Off(AMBOS_MOTORES);
			while(sensor_cor(SENSOR_COR_ESQUERDA) == FORA && sensor_cor(SENSOR_COR_DIREITA) == cor)
			{
				OnRevSync(AMBOS_MOTORES, - VELOCIDADE_BAIXA, 0); // Da uma pequena re
				Wait(200);
				Off(AMBOS_MOTORES);
				OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
				OnRev(MOTOR_DIREITA,  -VELOCIDADE_BAIXA);
				ClearScreen();
				//PlayTone(400, 100);
				TextOut(50,50, "E:P");
			}
			while(sensor_cor(SENSOR_COR_DIREITA) == FORA && sensor_cor(SENSOR_COR_ESQUERDA) == cor)
			{
				OnRevSync(AMBOS_MOTORES, - VELOCIDADE_BAIXA, 0); // Da uma pequena re
				Wait(200);
				Off(AMBOS_MOTORES);
				OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
				OnRev(MOTOR_ESQUERDA, -VELOCIDADE_BAIXA);
				ClearScreen();
				//PlayTone(800, 100);
				TextOut(50,50, "D:P");
			}
			ClearScreen();
			if (cor == BRANCO){
				procura_boneco();
			}
		}
	} else{ //andar reto no verde com o sensor verde
		set_sensor_color(SENSOR_COR_DIREITA, VERDE);
		Wait(100);
		set_sensor_color(SENSOR_COR_ESQUERDA, VERDE);
		while (sensor_cor_verde(SENSOR_COR_DIREITA) == cor || sensor_cor_verde(SENSOR_COR_ESQUERDA) == cor)
		{
			OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
			while(sensor_cor_verde(SENSOR_COR_ESQUERDA) == FORA && sensor_cor_verde(SENSOR_COR_DIREITA) == cor)
			{
				OnRevSync(AMBOS_MOTORES, - VELOCIDADE_BAIXA, 0); // Da uma pequena re
				Wait(200);
				Off(AMBOS_MOTORES);
				OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
				OnRev(MOTOR_DIREITA,  -VELOCIDADE_BAIXA);
				ClearScreen();
				PlayTone(400, 100);
				TextOut(50,50, "E:P");
			}
			while(sensor_cor_verde(SENSOR_COR_DIREITA) == FORA && sensor_cor_verde(SENSOR_COR_ESQUERDA) == cor)
			{
				OnRevSync(AMBOS_MOTORES, - VELOCIDADE_BAIXA, 0); // Da uma pequena re
				Wait(200);
				Off(AMBOS_MOTORES);
				OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
				OnRev(MOTOR_ESQUERDA, -VELOCIDADE_BAIXA);
				ClearScreen();
				PlayTone(800, 100);
				TextOut(50,50, "D:P");
			}
			ClearScreen();
		}
		Off(AMBOS_MOTORES);
		set_sensor_color(SENSOR_COR_DIREITA, VERMELHO);
		Wait(100);
		set_sensor_color(SENSOR_COR_ESQUERDA, VERMELHO);
		Wait(100);
	}
	Off(AMBOS_MOTORES);
	
	//alterei o laço para dentro da função recenbendo a cor como argumento
}

bool verificar_direcao(int cor)
{
	int cor_d, cor_e;
	reto(cor);
	ajeitar(cor);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
	reto(BRANCO);
	ajeitar(BRANCO);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);

	cor_d = teste_cor(SENSOR_COR_DIREITA);
	cor_e = teste_cor(SENSOR_COR_ESQUERDA);

	if (cor_e != PRETO && cor_d != PRETO)//se os dois nao veem preto entao o robo acertou o caminho
	{
		TextOut(10,10, "caminho certo");
		return true;
	}
	TextOut(10, 10, "Caminho errado");
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

int testar_caminho(int cor, int direcoes[])//testa as direções verificando se ja há alguma cor com a direção
{
	if (direcoes[AZUL] != ESQUERDA && direcoes[VERMELHO] != ESQUERDA && direcoes[VERDE] != ESQUERDA)
	{
		TextOut(10,10, "teste esquerda");
		ClearScreen();
		girar(90);
		ClearScreen();

		if (verificar_direcao(cor))
		{
			return ESQUERDA;
		}
		voltar(cor);

		if (direcoes[AZUL] != FRENTE && direcoes[VERMELHO] != FRENTE && direcoes[VERDE] != FRENTE)
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
	if (direcoes[AZUL] != FRENTE && direcoes[VERMELHO] != FRENTE && direcoes[VERDE] != FRENTE)
	{
		TextOut(10,10, "teste frente");
		if (verificar_direcao(cor))
		{
			return FRENTE;
		}
		voltar(cor);
		girar(90);
		verificar_direcao(cor);
		return DIREITA;
	}
	TextOut(10,10, "teste direita");
	verificar_direcao(cor);
	return DIREITA;
}


void seguir_direcao(int cor, int direcoes[])//função que sera usada quando a cor ja tiver uma direção definida
{
	if (direcoes[cor] == ESQUERDA)
	{
		girar(90);
	}
	if (direcoes[cor] == DIREITA)
	{
		girar(-90);
	}
	reto(cor);
	ajeitar(cor);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
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

void modo_plaza ()
{
	int aux, prev_motor;
	SetSensorHTGyro(SENSOR_GYRO);
	float angle = 0, gyro = 0, angle_inicial = 0;
	unsigned long time = CurrentTick(), prev_time;
	float offset = getGyroOffset();

	PlayTone(1400, 300);
	PlayTone(880, 300);
	PlayTone(220, 300);

	girar(90);
	girar(42); //por algum motivo esses angulos fazem o robo girar 180 graus perfeitamente
	//OnRevSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 1.3);
	//while(sensor_cor(SENSOR_COR_ESQUERDA) != FORA && sensor_cor(SENSOR_COR_DIREITA) != FORA);
	//Off(AMBOS_MOTORES);
	PlayTone(880, 500);

	prev_time = time;
	time = CurrentTick();
	gyro = SensorHTGyro(SENSOR_GYRO);
	angle_inicial = (gyro - offset) * (time - prev_time)/1000.0;

	ResetRotationCount(MOTOR_DIREITA);
	ResetRotationCount(MOTOR_ESQUERDA);
	//OnRevSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
	while(sensor_cor(SENSOR_COR_ESQUERDA) != PRETO && sensor_cor(SENSOR_COR_DIREITA) != PRETO)
	{

		time = CurrentTick();
		gyro = SensorHTGyro(SENSOR_GYRO);
		angle = (gyro - offset) * (time - prev_time)/1000.0;

		ClearLine(LCD_LINE1);
		ClearLine(LCD_LINE2);
		TextOut(0, LCD_LINE1, "ANGLE_I:");
		NumOut(55, LCD_LINE1, angle_inicial);
		TextOut(0, LCD_LINE2, "ANGLE:");
		NumOut(55, LCD_LINE2, angle);

		if (angle - angle_inicial > 0.01)
		{
			girar_sem_tempo(0.5);
		}
		else if (angle - angle_inicial < 0.01)
		{
			girar_sem_tempo(-0.5);
		}
		else
		{
			OnRevSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
			Wait(200);
			Off(AMBOS_MOTORES);
		}
	}
	PlayTone(880, 500);
	Wait(2000);
	Off(AMBOS_MOTORES);
	OnRevSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
	while(sensor_cor(SENSOR_COR_ESQUERDA) != PRETO && sensor_cor(SENSOR_COR_DIREITA) != PRETO);
	Off(AMBOS_MOTORES);
	PlayTone(880, 500);
	aux = abs(MotorRotationCount(MOTOR_DIREITA));

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
	PlayTone(880, 500);
	while(prev_motor < aux)
	{
    	OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
		Wait(50);
		prev_motor = abs(MotorRotationCount(MOTOR_DIREITA));
	}
	Off(AMBOS_MOTORES);
}

int identifica_cor()
{
	int cor_e, cor_d, i = 0;
	do
	{
		i++;
		ajeitar(BRANCO);
		distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
		teste_two_colors(cor_e, cor_d);
	}while((cor_e != cor_d) && i < 1);
	if (cor_e != FORA && cor_e != PRETO)
		return cor_e;
	if (cor_d != FORA && cor_d != PRETO)
		return cor_d;
	if (cor_e == PRETO)
		return cor_e;
	return cor_d;
}

task main () //por enquato a maior parte está só com a lógica, tem que alterar as funções pra ele conseguir andar certinho e girar
{
	int cor_achada;
	int CORES[3] = {AZUL, VERMELHO, VERDE};
	int direcoes[6] = {NADA, NADA, NADA, NADA, NADA, NADA};
	int i;
	BTCheck();
	ligar_sensores();
 	Wait(1000);
 	MOTOR(MOTOR_PORTA, -20);
 	Wait(200);
	MOTOR(MOTOR_PORTA, 0);

while (true){
	reto(BRANCO);
	if (sensor_cor(SENSOR_COR_ESQUERDA) == FORA && sensor_cor(SENSOR_COR_DIREITA) == FORA){
		Off(AMBOS_MOTORES);
		distancia_re(VELOCIDADE_BAIXA, VELOCIDADE_ALTA, 10);
		if(direcoes[AZUL] != NADA && direcoes[VERDE] != NADA && direcoes[VERMELHO] != NADA) modo_plaza();
		PlayTone(440, 200);
		Wait(300);
		PlayTone(440, 200);
		Wait(300);
	}
	cor_achada = identifica_cor();
	for(i = 0; i < 3; i++)
	{
		if(cor_achada == CORES[i])
		{
			switch(i)
			{
				case 2:
					PlayTone(440, 200);
					Wait(300);
				case 1:
					PlayTone(440, 200);
					Wait(300);
				case 0:
					PlayTone(440, 200);
			}
			reto(CORES[i]);
			ajeitar(CORES[i]);
			if (direcoes [CORES[i]] == NADA)
			{
				ClearScreen();
				TextOut(10,10, "NADA GRAVADO");
				direcoes[CORES[i]] = testar_caminho(CORES[i], direcoes);
			} else
			{
				seguir_direcao(CORES[i], direcoes);
			}
			break;
		}
	}
}
}
