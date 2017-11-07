#define BT_CONN 1

#define MOTOR_ESQUERDA OUT_C
#define MOTOR_DIREITA OUT_A
#define MOTORES OUT_AC
#define AMBOS_MOTORES OUT_AC
#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B /*conexão com o outro cérebro*/
#define MOTOR(p,s) RemoteSetOutputState(CONEXAO, p, s, \
OUT_MODE_MOTORON+OUT_MODE_BRAKE+OUT_MODE_REGULATED, \
OUT_REGMODE_SPEED, 0, OUT_RUNSTATE_RUNNING, 0)

/*no define acima, o argumento p correnponde à porta do motor e o argumento s corresponde à potencia que será passada ao motor*/

#define SENSOR_COR_ESQUERDA IN_4 /*conexÃ£o com o outro cÃ©rebro*/
#define SENSOR_COR_DIREITA IN_1 /*conexÃ£o com o outro cÃ©rebro*/
#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_4
#define SENSOR_GYRO IN_1 /*teste*/
#define SENSOR_US_GARRA IN_3 /*teste*/
#define CONEXAO 1

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65
#define VELOCIDADE_ALTISSIMA 90

#define PRETO    1
#define AZUL     2
#define VERDE    3
#define AMARELO  4
#define VERMELHO 5
#define BRANCO   6
#define FORA     7

#define SENSIBILIDADE 0.9
#define OFFSET_SAMPLES 2000

#define ESQUERDA 1
#define DIREITA -1
#define FRENTE 0
#define NADA 2

#define OFFSET_COLOR 1/9.0

#define CORRECAO 0.051

byte VERMELHO_r, VERDE_r, AZUL_r, BRANCO_r, num_r, VERMELHO_l, VERDE_l, AZUL_l, BRANCO_l, num_l, color_r, color_l;
int passageiros = 0, maximo = 40; //achei mais prático criar um vetor de 6 posiçoes e usar as constantes como o valor do índice

sub BTCheck(){
     if (!BluetoothStatus(CONEXAO)==NO_ERR){
          ClearScreen();
          TextOut(5,LCD_LINE2,"Erro");
          Wait(1000);
          Stop(true);
      }
}

// Inicio das funcoes para cores
void ligar_sensores() //testada
{
	SetSensorHTGyro(SENSOR_GYRO);
	SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	SetSensorUltrasonic(SENSOR_US_DIREITA);
    SetSensorLowspeed(SENSOR_COR_ESQUERDA);
    SetSensorLowspeed(SENSOR_COR_DIREITA);
    SetHTColor2Mode(SENSOR_COR_ESQUERDA, HT_CMD_COLOR2_ACTIVE);
    SetHTColor2Mode(SENSOR_COR_DIREITA, HT_CMD_COLOR2_ACTIVE);
}

sub detect_colors(){ // Tabela a saída dos sensores de luz para as cores lidas
    ReadSensorHTColor2Active(SENSOR_COR_DIREITA, num_r, VERMELHO_r, VERDE_r, AZUL_r, BRANCO_r);
    ReadSensorHTColor2Active(SENSOR_COR_ESQUERDA, num_l, VERMELHO_l, VERDE_l, AZUL_l, BRANCO_l);

    /*Achei melhor if em vez de switch pois os valores lidos dependem da calibração*/
    if(num_r == 14)
        if(BRANCO_r > 217)
            color_r = BRANCO;
        else
            color_r = FORA;
    else if(num_r == 12)
        if(BRANCO_r > 217)
            color_r = BRANCO;
        else
            color_r = PRETO;
    else if(num_r == 17)
        if(BRANCO_r > 217)
            color_r = BRANCO;
        else if(BRANCO_r < 90)
            color_r = PRETO;
        else
            color_r = FORA;
    else if(num_r == 13)
        color_r = BRANCO;
    else if(num_r == 3 || num_r == 4)
        color_r = VERDE;
    else if(num_r == 5 || num_r == 6)
        color_r = AMARELO;
    else if(num_r == 7)
        if(VERDE_r > 130)
            color_r = AMARELO;
        else
            color_r = VERMELHO;
    else if(num_r > 7 && num_r <= 10)
        color_r = VERMELHO;
    else if(num_r == 2)
        color_r = AZUL;

    if(num_l == 14)
        if(BRANCO_l > 217)
            color_l = BRANCO;
        else
            color_l = FORA;
    else if(num_l == 12)
        if(BRANCO_l > 217)
            color_l = BRANCO;
        else
            color_l = PRETO;
    else if(num_l == 17)
        if(BRANCO_l > 217)
            color_l = BRANCO;
        else if(BRANCO_l < 90)
            color_l = PRETO;
        else
            color_l = FORA;
    else if(num_l == 13)
        color_l = BRANCO;
    else if(num_l == 3 || num_l == 4)
        color_l = VERDE;
    else if(num_l == 5 || num_l == 6)
        color_l = AMARELO;
    else if(num_l == 7)
        if(VERDE_l > 130)
            color_l = AMARELO;
        else
            color_l = VERMELHO;
    else if(num_l > 7 && num_l <= 10)
        color_l = VERMELHO;
    else if(num_l == 2)
        color_l = AZUL;
}// Locomoção

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
	int left, right;
	Off(MOTORES);
	ClearScreen();
	TextOut(0,0,"ajeitando");
	NumOut(10,10, cor);
	
	while(color_r != cor || color_l != cor) // Aqui ele comeca a ajeitar, para caso chegue torto, ele fique certo no final
	{
		Off(AMBOS_MOTORES);
		
		contador = 0;
		while(color_l != cor && contador < 3)
		{ // Ajusta a roda esquerda para ficar em cima da linha.
			OnRev(MOTOR_ESQUERDA, (-1)*VELOCIDADE_MEDIA);
			Wait(50);
			contador += 1;
		}
		Off(AMBOS_MOTORES);
		contador = 0;
		while(color_r != cor && contador < 3)
		{ // Ajusta a roda esquerda para ficar em cima da linha.
			OnRev(MOTOR_DIREITA,  (-1)*VELOCIDADE_MEDIA);
			Wait(50);
			contador += 1;
		}
	}
	Off(MOTORES);
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
		while(color_l != FORA && color_r != FORA)
			OnFwdSync(MOTORES, -VELOCIDADE_MEDIA, 0);
		Off(MOTORES);

		while(color_r == FORA){
			OnFwd(MOTOR_DIREITA, VELOCIDADE_MEDIA);
		}
		Off(MOTOR_DIREITA);

		while(color_l == FORA)
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

void modo_plaza (int direcoes[])
{
	int aux, prev_motor; //Essa ultima constante é para armazenar o ultimo parâmetro do Sync
	SetSensorHTGyro(SENSOR_GYRO);
	float angle = 0, gyro = 0, angle_inicial = 0, result, offset_velocidade = 0, tolerancia = 40.0;
	unsigned long time = CurrentTick(), prev_time;
	float offset = getGyroOffset();


	PlayTone(880, 500);


	ResetRotationCount(MOTOR_DIREITA);
	ResetRotationCount(MOTOR_ESQUERDA);
	OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, -6);
	
	PlayTone(880, 500);

	while (color_r !=PRETO && color_l !=PRETO)
	{
		OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, -6);
		detect_colors();
	}
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, maximo);
	giro(180);
	aux = abs(MotorRotationCount(MOTOR_DIREITA));
	ResetRotationCount(MOTOR_DIREITA);
	ResetRotationCount(MOTOR_ESQUERDA);
	MOTOR(MOTOR_PORTA, VELOCIDADE_BAIXA);
	Wait(400);
	MOTOR(MOTOR_PORTA, 0);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 30);
	MOTOR(MOTOR_PORTA, -VELOCIDADE_BAIXA);
	Wait(400);
	MOTOR(MOTOR_PORTA, 0);
	giro(-180);
	
    OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, -6);
	while(prev_motor < aux)
	{
		Wait(50);
		prev_motor = abs(MotorRotationCount(MOTOR_DIREITA));
	}
	if (maximo > 10) maximo -= 10;

	Off(AMBOS_MOTORES);
	for (int i = 0; i < 5; ++i)
	{
		direcoes[i] = -direcoes[i];
	}
	passageiros = 0;
}


task main ()
{
	int direcoes[6] = {1, 1, 1, 1, 1, 1};
	BTCheck();

 	ligar_sensores();
 	Wait(1000);
 	MOTOR(MOTOR_PORTA, -10);
 	Wait(200);
	MOTOR(MOTOR_PORTA, 0);


	OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
	detect_colors();
	while(color_l != VERMELHO && color_r != VERMELHO) detect_colors();
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 2);
	detect_colors();
	if((color_l == AZUL || color_r == AZUL) || (color_l == VERDE || color_r == VERDE)) modo_plaza(direcoes);
}