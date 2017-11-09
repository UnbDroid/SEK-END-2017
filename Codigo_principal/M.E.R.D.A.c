#define BT_CONN 1

#define MOTOR_ESQUERDA OUT_C
#define MOTOR_DIREITA OUT_A
#define AMBOS_MOTORES OUT_AC
#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B 
#define MOTOR(p,s) RemoteSetOutputState(CONEXAO, p, s, \
OUT_MODE_MOTORON+OUT_MODE_BRAKE+OUT_MODE_REGULATED, \
OUT_REGMODE_SPEED, 0, OUT_RUNSTATE_RUNNING, 0)

#define SENSOR_COR_ESQUERDA 1 
#define SENSOR_COR_DIREITA 2 
#define SENSOR_US_ESQUERDA IN_4
#define SENSOR_GYRO IN_1 
#define R_COLOR IN_2
#define L_COLOR IN_3
#define CONEXAO 1

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65
#define VELOCIDADE_ALTISSIMA 70

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
#define NAO_FRENTE 3
#define NAO_FRENTE_ESQUERDA 4

#define OFFSET_COLOR 1/9.0

#define CORRECAO 0.051

int passageiros = 0; 
int maximo = 40;
byte VERMELHO_r, VERDE_r, AZUL_r, BRANCO_r, num_r, VERMELHO_l, VERDE_l, AZUL_l, BRANCO_l, num_l, color_r, color_l;

sub BTCheck(){
     if (!BluetoothStatus(CONEXAO)==NO_ERR){
          ClearScreen();
          TextOut(5,LCD_LINE2,"Erro");
          Wait(1000);
          Stop(true);
      }
}


void ligar_sensores() //testada
{
	SetSensorHTGyro(SENSOR_GYRO);
	SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	SetSensorLowspeed(L_COLOR);
    SetSensorLowspeed(R_COLOR);
    SetHTColor2Mode(L_COLOR, HT_CMD_COLOR2_ACTIVE);
    SetHTColor2Mode(R_COLOR, HT_CMD_COLOR2_ACTIVE);
}

int detect_colors(){ // Tabela a saída dos sensores de luz para as cores lidas
    ReadSensorHTColor2Active(R_COLOR, num_r, VERMELHO_r, VERDE_r, AZUL_r, BRANCO_r);
    ReadSensorHTColor2Active(L_COLOR, num_l, VERMELHO_l, VERDE_l, AZUL_l, BRANCO_l);

    /*Achei melhor if em vez de switch pois os valores lidos dependem da calibração*/
    if (num_l >= 1 && num_l <= 4)
{
	if (AZUL_l > 150){
		color_l = AZUL;
	} 
	else{
		color_l = VERDE;
	} 	
}  
else if (num_l >= 5 && num_l <= 8)
{
	if (VERDE_l > 140){
		color_l = AMARELO;
	}
	else{
		color_l = VERMELHO;
	} 
}
else if (num_l >= 9 && num_l <= 12 )
{
	color_l = VERMELHO;
}
else if (num_l >= 13 && num_l <= 15)
{
	if (num_l == 14)
	{
		if (BRANCO_l < 200)
		{
			color_l = PRETO;
		} else
		{
			color_l = BRANCO;
		}
	}
	else 
	{
		color_l = BRANCO;
	}
}
else if (num_l == 17)
{
	if (BRANCO_l > 150){
		color_l = PRETO;
	}
	else{
		color_l = FORA;
	} 		
} 
else
{
	color_l = FORA;
}

if (num_r >= 1 && num_r <= 4)
{
	if (AZUL_r > 120){
		color_r = AZUL;
	} 
	else{
		color_r = VERDE;
	}
}
else if (num_r >= 5 && num_r <= 8)
{
	if (VERDE_r > 110){
		color_r = AMARELO;
	}
	else{
		color_r = VERMELHO;
	}
}
else if (num_r >= 9 && num_r <= 12 )
{
	color_r = VERMELHO;
}
else if (num_r >= 13 && num_r <= 15)
{
	if (num_r == 14)
	{
		if (BRANCO_r < 200)
		{
			color_r = PRETO;
		} else
		{
			color_r = BRANCO;
		}
	}
	else 
	{
		color_r = BRANCO;
	}
}
else if (num_r == 17)
{
	if (BRANCO_r > 50){ //SE DER MERDA E AQUI, calibrar melhor
		color_r = PRETO;
	}
	else{
		color_r = FORA;
    }
}
else{
	color_r = FORA;
}


}

int sensor_cor(int sensor)
{
	detect_colors();
	/*for(int i = 0; i < 1/OFFSET_COLOR; i++) //Acho que precisamos filtraro sensor assim
	{
		leitura += get_value_color(sensor)*OFFSET_COLOR;
		Wait(10); 
	}*/
	if (SENSOR_COR_ESQUERDA == sensor){
		NumOut(10,10, color_l);
		return color_l;
	}
	if (SENSOR_COR_DIREITA == sensor){
		NumOut(20,20, color_r);
		return color_r;
	}
}

void distancia_reto(int low_speed, int high_speed, int distancia){//função do kaynã
	
	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	OnRev(AMBOS_MOTORES, high_speed);
	do{
		if (count_A - MotorRotationCount(MOTOR_ESQUERDA) > count_C - MotorRotationCount(MOTOR_DIREITA))
		{
			OnRev(MOTOR_ESQUERDA, low_speed);
			until ((count_C - MotorRotationCount(MOTOR_DIREITA)) >  (count_A - MotorRotationCount(MOTOR_ESQUERDA)) || ((count_A - MotorRotationCount(MOTOR_ESQUERDA))*6*PI/360 <= distancia));
			OnRev(MOTOR_ESQUERDA, high_speed);
		}
		else
		{
			OnRev(MOTOR_DIREITA, low_speed);
			until ( (count_A - MotorRotationCount(MOTOR_ESQUERDA)) > (count_C - MotorRotationCount(MOTOR_DIREITA)) || ((count_C - MotorRotationCount(MOTOR_DIREITA))*6*PI/360 <= distancia));
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
			until (((MotorRotationCount(MOTOR_DIREITA) - count_C) >  (MotorRotationCount(MOTOR_ESQUERDA) - count_A)) || ((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia));
			OnFwd(MOTOR_ESQUERDA, high_speed);
		}
		else
		{
			OnFwd(MOTOR_DIREITA, low_speed);
			until ( ((MotorRotationCount(MOTOR_ESQUERDA) - count_A) > (MotorRotationCount(MOTOR_DIREITA) - count_C)) || ((MotorRotationCount(MOTOR_DIREITA) - count_C)*6*PI/360 <= distancia));
			OnFwd(MOTOR_DIREITA, high_speed);
		}
	}while(((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia) && ((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia));
	Off(AMBOS_MOTORES);

}

void ajeitar_frente()//ajeita na cor em frente ao branco
{
	int contador = 0;	
	{
		while((sensor_cor(SENSOR_COR_DIREITA) == BRANCO || sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO)) // Aqui ele comeca a ajeitar, para caso chegue torto, ele fique certo no final
		{
			Off(AMBOS_MOTORES);
			//while(sensor_cor(SENSOR_COR_ESQUERDA) == FORA) OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
			//while(sensor_cor(SENSOR_COR_DIREITA) == FORA ) OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
			
			Off(AMBOS_MOTORES);
			contador = 0;
			while(sensor_cor(SENSOR_COR_DIREITA) == BRANCO && contador < 10)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnFwd(MOTOR_DIREITA,  (-1)*VELOCIDADE_MEDIA);
				Wait(10);
				contador += 1;
			}
			Off(AMBOS_MOTORES);
			contador = 0;
			while(sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO && contador < 10)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnFwd(MOTOR_ESQUERDA, (-1)*VELOCIDADE_MEDIA);
				Wait(10);
				contador += 1;
			}
			
		}
	}
	Off(AMBOS_MOTORES);
	Wait(500);
}

void ajeitar(int cor) //arruma o robo pra ficar alinhado no quadrado da cor que recebe
{
	int contador, contador_2 = 0;
	int left, right;
	Off(AMBOS_MOTORES);
	ClearScreen();
	TextOut(0,0,"ajeitando");
	NumOut(10,10, cor);

		while(sensor_cor(SENSOR_COR_ESQUERDA) == FORA) OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
		while(sensor_cor(SENSOR_COR_DIREITA) == FORA ) OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);

		while(sensor_cor(SENSOR_COR_DIREITA) != cor && sensor_cor(SENSOR_COR_ESQUERDA) != cor) OnRevSync(AMBOS_MOTORES, -VELOCIDADE_BAIXA, 0);

		while((sensor_cor(SENSOR_COR_DIREITA) != cor || sensor_cor(SENSOR_COR_ESQUERDA) != cor) && contador_2 < 4) // Aqui ele comeca a ajeitar, para caso chegue torto, ele fique certo no final
		{
			contador_2++;
			Off(AMBOS_MOTORES);
			//while(sensor_cor(SENSOR_COR_ESQUERDA) == FORA) OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
			//while(sensor_cor(SENSOR_COR_DIREITA) == FORA ) OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
			
			Off(AMBOS_MOTORES);
			contador = 0;
			while(sensor_cor(SENSOR_COR_DIREITA) != cor && contador < 10)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnRev(MOTOR_DIREITA,  (-1)*VELOCIDADE_MEDIA);
				Wait(10);
				contador += 1;
			}
			contador = 0;
			while(sensor_cor(SENSOR_COR_ESQUERDA) != cor && contador < 10)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnRev(MOTOR_ESQUERDA, (-1)*VELOCIDADE_MEDIA);
				Wait(10);
				contador += 1;
			}
			
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
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 10);
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

		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
		giro(90);
	}
	if (degrees != 90 && degrees != -90 && degrees != 180)
		giro(degrees);
}


float ultrassom_filtrado(int sensor) //testada
{
	float valor = SensorUS(sensor);
	float aux;
	for (int i = 0; i < 3; ++i)
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

int pegar_passageiro (/*int lado*/) //testado, mas precisa mudar a função gira para o robô girar no centro dele
{

	int confirma_que_pegou = 0;

	/*if(lado == DIREITA && passageiros < 3){ //Ainda é necessário adaptar a função agarrar() pra depois de ela agarrar, ela voltar para a
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
	}*/

	if(passageiros < 3){

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
	//int d = ultrassom_filtrado(SENSOR_US_DIREITA);
	/*if (d <= 15)
	{
		pegar_passageiro(DIREITA);
	} */
	if(l <= 15)
	{
		pegar_passageiro();
	}
	//ClearScreen();
	//NumOut(10, 30, l);
	//NumOut(10, 10, d);
}

void corrige_intersecao(int cor)
{
	int contador = 0;
	if(sensor_cor(SENSOR_COR_ESQUERDA) == cor && sensor_cor(SENSOR_COR_DIREITA) == BRANCO)
	{
		Off(AMBOS_MOTORES);
		Wait(100);
		OnRevSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		while (sensor_cor(SENSOR_COR_DIREITA) == BRANCO)
		{ 
			OnRevSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		}
		Wait(250);
		Off(AMBOS_MOTORES);
		contador = 0;
		while (sensor_cor(SENSOR_COR_DIREITA) == cor && contador < 25)
		{
			OnFwd(MOTOR_DIREITA,  -VELOCIDADE_MEDIA);
			contador++;
			Wait(10);
		}
		if (sensor_cor(SENSOR_COR_DIREITA) == BRANCO)
		{
			Off(AMBOS_MOTORES);
			while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
			{
				while(sensor_cor(SENSOR_COR_ESQUERDA) == cor)
				{
					OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
				}
				Off(AMBOS_MOTORES);
				while(sensor_cor(SENSOR_COR_DIREITA) == cor)
				{
					OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
				}
				Off(AMBOS_MOTORES);
			}
		}
		Off(AMBOS_MOTORES);
	}
				
	ClearScreen();
	if(sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO && sensor_cor(SENSOR_COR_DIREITA) == cor)
	{
		Off(AMBOS_MOTORES);
		Wait(100);
		OnRevSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		while (sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO)
		{
			OnRevSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		}
		Wait(250);
		Off(AMBOS_MOTORES);
		Wait(100);
		contador = 0;
		while (sensor_cor(SENSOR_COR_ESQUERDA) == cor && contador < 25)
		{
			OnFwd(MOTOR_ESQUERDA,  -VELOCIDADE_MEDIA);
			contador++;
			Wait(10);
		}
		Off(AMBOS_MOTORES);
		if (sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO)
		{
			Off(AMBOS_MOTORES);
			while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
			{
				while(sensor_cor(SENSOR_COR_DIREITA) == cor)
				{
					OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
				}
				Off(AMBOS_MOTORES);
				while(sensor_cor(SENSOR_COR_ESQUERDA) == cor)
				{
					OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
				}
				Off(AMBOS_MOTORES);
			}
		}
		Off(AMBOS_MOTORES);
	}
	//correçao do fora
	if(sensor_cor(SENSOR_COR_ESQUERDA) == cor && sensor_cor(SENSOR_COR_DIREITA) == FORA)
	{
		Off(AMBOS_MOTORES);
		Wait(100);
		OnRevSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		while (sensor_cor(SENSOR_COR_DIREITA) == FORA)
		{ 
			OnRevSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		}
		Wait(250);
		Off(AMBOS_MOTORES);
		contador = 0;
		while (sensor_cor(SENSOR_COR_DIREITA) == cor && contador < 25)
		{
			OnFwd(MOTOR_DIREITA,  -VELOCIDADE_MEDIA);
			contador++;
			Wait(10);
		}
		if (sensor_cor(SENSOR_COR_DIREITA) == FORA)
		{
			Off(AMBOS_MOTORES);
			while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
			{
				while(sensor_cor(SENSOR_COR_ESQUERDA) == cor)
				{
					OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
				}
				Off(AMBOS_MOTORES);
				while(sensor_cor(SENSOR_COR_DIREITA) == cor)
				{
					OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
				}
				Off(AMBOS_MOTORES);
			}
		}
		Off(AMBOS_MOTORES);
	}
				
	ClearScreen();
	if(sensor_cor(SENSOR_COR_ESQUERDA) == FORA && sensor_cor(SENSOR_COR_DIREITA) == cor)
	{
		Off(AMBOS_MOTORES);
		Wait(100);
		OnRevSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		while (sensor_cor(SENSOR_COR_ESQUERDA) == FORA)
		{
			OnRevSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		}
		Wait(250);
		Off(AMBOS_MOTORES);
		Wait(100);
		contador = 0;
		while (sensor_cor(SENSOR_COR_ESQUERDA) == cor && contador < 25)
		{
			OnFwd(MOTOR_ESQUERDA,  -VELOCIDADE_MEDIA);
			contador++;
			Wait(10);
		}
		Off(AMBOS_MOTORES);
		if (sensor_cor(SENSOR_COR_ESQUERDA) == FORA)
		{
			Off(AMBOS_MOTORES);
			while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
			{
				while(sensor_cor(SENSOR_COR_DIREITA) == cor)
				{
					OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
				}
				Off(AMBOS_MOTORES);
				while(sensor_cor(SENSOR_COR_ESQUERDA) == cor)
				{
					OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
				}
				Off(AMBOS_MOTORES);
			}
		}
		Off(AMBOS_MOTORES);
	}
}

void reto(int cor) //robo move ate que os dois sensores parem de ver a cor
{
	int contador = 0;
	if (cor == BRANCO)
	{
		while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
		{
			OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, -6);
			
			while(sensor_cor(SENSOR_COR_ESQUERDA) == FORA && sensor_cor(SENSOR_COR_DIREITA) == BRANCO)
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
			while(sensor_cor(SENSOR_COR_DIREITA) == FORA && sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO)
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
			while(sensor_cor(SENSOR_COR_ESQUERDA) != BRANCO && sensor_cor(SENSOR_COR_ESQUERDA) != FORA && sensor_cor(SENSOR_COR_DIREITA) == BRANCO)
			{
				Off(MOTOR_ESQUERDA);
				OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
			}
			while(sensor_cor(SENSOR_COR_DIREITA) != BRANCO && sensor_cor(SENSOR_COR_DIREITA) != FORA && sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO)
			{
				Off(MOTOR_DIREITA);
				OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
			}
			ClearScreen();
			if (cor == BRANCO){
				procura_boneco();
			}
		}
	} else 
	{
		while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
		{
			OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, -6);
			corrige_intersecao(cor);				
			ClearScreen();
			//PlayTone(400, 100);
		}
		Off(AMBOS_MOTORES);
	}
	Off(AMBOS_MOTORES);
	ajeitar(cor);
	
	//alterei o laço para dentro da função recenbendo a cor como argumento
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
	ResetRotationCount(MOTOR_ESQUERDA);  // Checkpoint 1 do rotation count

	while (color_r !=PRETO && color_l !=PRETO)
	{
		retinho(VELOCIDADE_ALTA);
	}

	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 30);

	aux = abs(MotorRotationCount(MOTOR_DIREITA));

	giro(180);

	ResetRotationCount(MOTOR_DIREITA);
	ResetRotationCount(MOTOR_ESQUERDA);  // Checkpoint 2 do rotation count

	MOTOR(MOTOR_PORTA, VELOCIDADE_BAIXA);
	Wait(400);
	MOTOR(MOTOR_PORTA, 0);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 30);
	MOTOR(MOTOR_PORTA, -VELOCIDADE_BAIXA);
	Wait(400);
	MOTOR(MOTOR_PORTA, 0);

    retinho(VELOCIDADE_ALTA);

	while(prev_motor < aux)
	{
		Wait(50);
		prev_motor = abs(MotorRotationCount(MOTOR_DIREITA));
	}

	Off(AMBOS_MOTORES);
	
	for (int i = 0; i < 5; ++i)
	{
		direcoes[i] = -direcoes[i];
	}
	passageiros = 0;
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

	if (sensor_cor(SENSOR_COR_ESQUERDA) != PRETO && sensor_cor(SENSOR_COR_DIREITA) != PRETO)//se os dois nao veem preto entao o robo acertou o caminho
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
	ajeitar(BRANCO);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
	reto(cor);
	ajeitar(cor);
}

int testar_caminho(int cor, int direcoes[])//testa as direções verificando se ja há alguma cor com a direção
{	
	if (direcoes[AMARELO] != FRENTE && direcoes[VERMELHO] != FRENTE && direcoes[VERDE] != FRENTE && direcoes[cor] != NAO_FRENTE && direcoes[cor] != NAO_FRENTE_ESQUERDA)
	{
		TextOut(10,10, "teste frente");
		if (verificar_direcao(cor))
		{
			return FRENTE;
		}
		// alteração do vetor direção
		//ESCREVER NO ARQUIVO: direcao[cor] = NAO_FRENTE
		voltar(cor);
		if (direcoes[AMARELO] != ESQUERDA && direcoes[VERMELHO] != ESQUERDA && direcoes[VERDE] != ESQUERDA && direcoes[cor] != NAO_FRENTE_ESQUERDA)
		{
			girar(-90);
			if (verificar_direcao(cor))
			{
				return ESQUERDA;
			}
			//ESCREVER NO ARQUIVO[]
			voltar(cor);
			verificar_direcao(cor);
			return DIREITA;
		}
		else
		{
			girar(90);
			verificar_direcao(cor);
			return DIREITA;
		}
	}
	if (direcoes[AMARELO] != ESQUERDA && direcoes[VERMELHO] != ESQUERDA && direcoes[VERDE] != ESQUERDA && direcoes[cor] == NAO_FRENTE_ESQUERDA)
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
		verificar_direcao(cor);
		return DIREITA;
		
	}
	girar(90);
	TextOut(10,10, "teste direita");
	verificar_direcao(cor);
	return DIREITA;
}


void seguir_direcao(int cor, int direcoes[])//função que sera usada quando a cor ja tiver uma direção definida
{
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
	if (sensor_cor(SENSOR_COR_ESQUERDA) == AZUL || sensor_cor(SENSOR_COR_ESQUERDA) == VERDE)
	{
		modo_plaza(direcoes);
	}
	else if (sensor_cor(SENSOR_COR_DIREITA) == AZUL || sensor_cor(SENSOR_COR_DIREITA) == VERDE)
	{
		modo_plaza(direcoes);
	}
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

int identifica_cor()
{
	int cor_e, cor_d, i = 0;
	do
	{
		i++;
		ajeitar(BRANCO);
		ajeitar_frente();
		cor_e = sensor_cor(SENSOR_COR_ESQUERDA);
		cor_d = sensor_cor(SENSOR_COR_DIREITA);
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
	int CORES[3] = {AMARELO, VERMELHO, VERDE};
	int direcoes[6] = {NADA, NADA, NADA, NADA, NADA, NADA};
	int i;
	//BTCheck();
	ligar_sensores();
 	Wait(1000);
 	//MOTOR(MOTOR_PORTA, -10);
 	Wait(200);
	//MOTOR(MOTOR_PORTA, 0);

	while (true){
		reto(BRANCO);
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
					//ESCREVER NO ARQUIVO: direcoes[CORES[i]]
				} else
				{
					seguir_direcao(CORES[i], direcoes);
				}
				break;
			}
		}
	}
}
