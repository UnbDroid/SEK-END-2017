#define BT_CONN 1

#define DIRECOES_FILE "direcoes.dat"
#define DIRECOES_SIZE 3 // Pois é um vetor de 3 valores byte.
#define CORES_FILE "cores.dat"
#define VALUE_SIZE 3*4 // Pois sao 3 valores inteiros(4 bytes): min, med e max
#define COR_SIZE 3*VALUE_SIZE // Pois sao 3 cores: red, green, blue
#define QUANT_CORES 6 // Pois existe: red, green, blue, white, black and out
#define CORES_SIZE 2*QUANT_CORES*COR_SIZE // Esquerda e direita

#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
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
#define DESVIO 15

#define ESQUERDA 1
#define DIREITA -1
#define FRENTE 0
#define NADA 2

struct type_INTERVAL
{
	int up, down;
};
struct type_cor
{
	type_INTERVAL red, green, blue;
	type_INTERVAL white, black, out;
}
struct type_lado
{
	type_cor r, g, t;
}
type_INTERVAL init_interval(int u, int d)
{
	type_INTERVAL new;
	new.up 	 = u;
	new.down = d;
	return new;
}


type_LADO L, R;
L.r.red 	= init_interval(500, 555);
L.r.green 	= init_interval(220, 290);
L.r.blue	= init_interval(270, 285);
L.r.white 	= init_interval(555, 605);
//L.r.black	= init_interval(549, 558);
//L.r.out		= init_interval(549, 558);
R.r.red 	= init_interval(530, 550);
R.r.green 	= init_interval(300, 340);
R.r.blue	= init_interval(250, 285);
R.r.white	= init_interval(551, 600);
//R.r.black	= init_interval(549, 558);
//R.r.out		= init_interval(549, 558);

L.g.green 	= init_interval(510, 605);
L.g.white 	= init_interval(549, 558);
R.g.green 	= init_interval(365, 400);
R.g.white 	= init_interval(520, 550);

L.t.red 	= init_interval(438, 447);
L.t.green 	= init_interval(320, 335);
L.t.blue	= init_interval(265, 280);
//L.t.white 	= init_interval(549, 558);
L.t.black	= init_interval(215, 225);
L.t.out		= init_interval(315, 330);
R.t.red 	= init_interval(540, 585);
R.t.green 	= init_interval(260, 315);
R.t.blue	= init_interval(265, 281);
//R.t.white 	= init_interval(549, 558);
R.t.black	= init_interval(250, 260);
R.t.out		= init_interval(300, 365);

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
	for(int i = 0; i < 1/OFFSET_COLOR; i++) //Acho que precisamos filtraro sensor assim
	{
		leitura += get_value_color(sensor)*OFFSET_COLOR;
		Wait(10); 
	}
	if (sensor == SENSOR_COR_ESQUERDA){
		NumOut(0,0, leitura);
	
		if(leitura >= L.r.white.down){
			return BRANCO;
		} else if (leitura >= L.r.red.down){
			return VERMELHO;
		}/* else if (leitura <= BLACKUP_R){
			return PRETO; 
		}*/else if (leitura <= L.r.blue.up){
			return AZUL;
		} else if (leitura <= L.r.green.up){
			return VERDE;
		} else{
			return FORA;
		}
	}
	if (sensor == SENSOR_COR_DIREITA){
		NumOut(0,40, leitura);

		if(leitura >= R.r.white.down){
			return BRANCO;
		} else if (leitura >= R.r.red.down){
			return VERMELHO;
		} /*else if (leitura <= BLACKUP_R){
			return PRETO; 
		}*/else if (leitura <= R.r.blue.up){
			return AZUL;
		} else if (leitura <= R.r.green.up){
			return VERDE;
		} else{
			return FORA;
		}
	}
}

int sensor_cor_verde(int sensor)
{
	int leitura = get_value_color(sensor);

	for(int i = 0; i < 1/OFFSET_COLOR; i++) //Acho que precisamos filtraro sensor assim
	{
		leitura += get_value_color(sensor)*OFFSET_COLOR;
		Wait(10); 
	}
	if (sensor == SENSOR_COR_ESQUERDA)
	{
		if (leitura >= L.g.white.down)
			return BRANCO;
		if (leitura >= (L.g.green.down - DESVIO) && leitura <= (L.g.green.down + DESVIO))
			return VERDE;
		else 
			return FORA;
	}
	if (sensor == SENSOR_COR_DIREITA)
	{
		if (leitura >= R.g.white.down)
			return BRANCO;
		if (leitura >= (R.g.green.down - DESVIO) && leitura <= (R.g.green.down + DESVIO))
			return VERDE;
		else 
			return FORA;
	}
}
/*
int get_leitura_rgb(int sensor)
{
	int leitura_r = 0, leitura_g = 0, leitura_b = 0;
	int i, leitura;
	for(int i = 0; i < 1/OFFSET_COLOR; i++)
	{
		leitura_r += get_value_color(sensor)*OFFSET_COLOR;
		Wait(50);
	}
	//Wait(50);
	set_sensor_color(sensor, VERDE);
	Wait(100);
	for(int i = 0; i < 1/OFFSET_COLOR; i++)
	{
		leitura_g += get_value_color(sensor)*OFFSET_COLOR;
		Wait(50);
	}
	//Wait(50);
	set_sensor_color(sensor, AZUL);
	Wait(100);
	for(int i = 0; i < 1/OFFSET_COLOR; i++)
	{
		leitura_b += get_value_color(sensor)*OFFSET_COLOR;
		Wait(50);
	}
	//Wait(50);
	set_sensor_color(sensor, VERMELHO);
	Wait(100);
	leitura = (4*leitura_r + leitura_g + 2*leitura_b)/7;
	return leitura; // Se quisermos o valor entr 100 e 700
}
*/
int trata_leitura(int leitura, int sensor)
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
/*
int teste_cor(int sensor)
{
	int cor, leitura;
	leitura = get_leitura_rgb(sensor);
	cor 	= trata_leitura(leitura, sensor);
	return cor;
}
*/
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
	leitura_e = (9*left[0]+3*left[1]+1*left[2])/13;
	leitura_d = (9*right[0]+3*right[1]+1*right[2])/13;
}
void teste_two_colors(int &left, int &right)
{
	int leitura_e, leitura_d;
	get_two_rgb(leitura_e, leitura_d);
	left = trata_leitura(SENSOR_COR_ESQUERDA, leitura_e);
	right = trata_leitura(SENSOR_COR_DIREITA, leitura_d);
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
	OnFwdSync(AMBOS_MOTORES, -velocidade, 5);
	while (sensor_cor(SENSOR_COR_DIREITA) == cor && sensor_cor(SENSOR_COR_ESQUERDA) == cor) {
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
		if (cor == BRANCO)
		procura_boneco();
	}
}

void reto(int cor) //robo move ate que os dois sensores parem de ver a cor
{
	if (cor != VERDE){
		while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
		{
			OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
			retinho(VELOCIDADE_MEDIA, cor);
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
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 1.5);
	reto(BRANCO);
	ajeitar(BRANCO);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);

	teste_two_colors(cor_e, cor_d);

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
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 3);
}

void girar_sem_re(float degrees) // Algoritimo usado pela sek do ano passado //testada
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
			//ClearLine(LCD_LINE1);
			//TextOut(0, LCD_LINE1, "ANGLE:");
			//NumOut(48, LCD_LINE1, angle);
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
			girar_sem_re(0.5);
		}
		else if (angle - angle_inicial < 0.01)
		{
			girar_sem_re(-0.5);
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
		cor_e = teste_cor(SENSOR_COR_ESQUERDA);
		cor_d = teste_cor(SENSOR_COR_DIREITA);
	}while((cor_e != cor_d) && i < 1);
	if (cor_e != FORA && cor_e != PRETO)
		return cor_e;
	if (cor_d != FORA && cor_d != PRETO)
		return cor_d;
	if (cor_e == PRETO)
		return cor_e;
	return cor_d;
}

// Funcoes para trabalhar com a leitura de arquivos para gravar as direcoes e as cores

int open_for_write(int &handle, int SIZE, char NAME[])
{
	unsigned int file_size = SIZE;
	handle = 0;
	// Start with the assumptions the file doesn't exist and needs to be created.
	unsigned int rtn_code = CreateFile(NAME, file_size, handle);
	// If the file already exists, open it with the intent of adding to the data
	// that is already there.
	if (rtn_code == LDR_FILEEXISTS)
	{
		rtn_code = DeleteFile(NAME);
		if(rtn_code == LDR_SUCCESS)
		{
			rtn_code = CreateFile(NAME, file_size, handle);
		}
		else
		{
			// Deu algum erro, acho que nunca vai entrar aqui
		}
	}
	// Return code handling
	switch (rtn_code)
	{
		case LDR_SUCCESS:
			return 1;
		case LDR_FILEISFULL:
			// Acho que nunca vai entrar nesse caso, pois o arquivo sempre é criado novamente
			TextOut(0, LCD_LINE1, "file is full    ");
			break;
		default:
			// Unanticipated exception.
			TextOut(0, LCD_LINE1, "write open      ");
			break;
	}
	alerta(800);
	return 0;
}
int write_direcoes(int &handle, byte direcoes[]) // Retorna 1 caso consiga gravar
{
	byte CORES[3] = [VERMELHO, VERDE, AZUL];
	unsigned int p; //Problem
	int i;

	for(i = 0; i < 3; i++) // Pois sao 3 direcoes apenas
	{
		p = Write(handle, direcoes[CORES[i]]);
		// Return code handling
		if (p != LDR_SUCCESS)
			return 0;
	}
	return 1;
}
int open_for_read(int &handle, int SIZE, char NAME[])
{
   unsigned int file_size = SIZE;
   handle = 0;
   unsigned int rtn_code = OpenFileRead(NAME, file_size, handle);
   // Return code handling
   if (rtn_code != LDR_SUCCESS)
   {
      return 0;
   }
   return 1;
}
int read_direct(int &handle, byte &direct)
{

	unsigned int p;
	p = Read(handle, direct);
	// rtn_code_out(rtn_code);
	// Return code handling
	if (p == LDR_SUCCESS)
		return 1;
	return 0;
}

task main () //por enquato a maior parte está só com a lógica, tem que alterar as funções pra ele conseguir andar certinho e girar
{
	int handle = 0;
	byte cor_achada, direc;
	byte CORES[3] = {VERMELHO, VERDE, AZUL};
	byte direcoes[6] = {NADA, NADA, NADA, NADA, NADA, NADA};
	int r = 1, i;
	BTCheck();
	ligar_sensores();
	// Inicia lendo do arquivo de cores
	r = open_for_read(handle, CORES_SIZE, CORES_FILE);
	if(r == 1) // Abriu o arquivo com sucesso
	{
		
	}
	if(r == 0)
	{
		
	}
	// Inicia lendo do arquivo direcoes 
	r = open_for_read(handle, DIRECOES_SIZE, DIRECOES_FILE);
	if(r == 1) // Abriu o arquivo com sucesso
	{
		for(i = 0; i < 3 && r == 1; i++)
		{
			r = read_direct(handle, direc);
			direcoes[CORES[i]] = direc;
		}
	}
	if(r == 0) // Ou seja, teve algum erro na abertura do arquivo, ou durante a leitura do arquivo
	{
		for(i = 0; i < 3; i++)
		{
			direcoes[CORES[i]] = NADA; // Retorna às configurações iniciais caso dê errado
		}
	}
	// Fim de ler as direcoes do arquivo
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
