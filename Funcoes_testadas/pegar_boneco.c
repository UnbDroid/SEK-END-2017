
#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define AMBOS_MOTORES OUT_AC
#define MOTOR_GARRA OUT_B
#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_4
#define SENSOR_GYRO IN_1

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 45
#define VELOCIDADE_ALTA 65

#define SENSIBILIDADE 0.9

#define ESQUERDA 1
#define DIREITA -1
#define FRENTE 0
#define NADA 2

#define OFFSET_SAMPLES 2000

int passageiros = 0;

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

task main()
{
	while(passageiros < 3){
		OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
		procura_boneco();
	}
}