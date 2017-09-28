#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define MOTORES OUT_AC
#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B /*conexão com o outro cérebro*/
#define SENSOR_COR_ESQUERDA
#define SENSOR_COR_DIREITA
#define SENSOR_US_ESQUERDA IN_1
#define SENSOR_US_DIREITA IN_3
#define SENSOR_GYRO IN_4 /*teste*/
#define SENSOR_US_GARRA IN_2 /*teste*/
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65
#define PRETO 1
#define VERDE 3
#define BRANCO 6
#define AZUL 2
#define VERMELHO 5
#define AMARELO 4
#define SENSIBILIDADE 0.9
#define OFFSET_SAMPLES 2000

void ligar_sensores() //testada
{
	SetSensorHTGyro(SENSOR_GYRO);
	SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	SetSensorUltrasonic(SENSOR_US_DIREITA);
	//SetSensorColorFull(SENSOR_COR_DIREITA);
	//SetSensorColorFull(SENSOR_COR_ESQUERDA);
	SetSensorUltrasonic(SENSOR_US_GARRA);
}


void reto() //testada
{

	OnFwdSync(MOTORES, -VELOCIDADE_ALTA, 0);
	//este OnFwdSync ficava dentro de um while, mas é melhor que ele fique de fora para que tenhamos melhor controle dos laços de repetição
	//ou seja, o laço de repetição deve ficar na main
	// o último valor da função corrige a diferença entre os motores, que acontece devido a diferença de peso em cada um
	//o último valor foi modificado para 0, já que os pesos foram ajustados para que não fossem necessária nenhuma compensação

}

void virar_esquerda() //testada
{
	OnFwd(MOTOR_DIREITA, VELOCIDADE_BAIXA);
	OnRev(MOTOR_ESQUERDA, VELOCIDADE_BAIXA);
}

void virar_direita() //testada
{
	OnFwd(MOTOR_ESQUERDA, VELOCIDADE_BAIXA);
	OnRev(MOTOR_DIREITA, VELOCIDADE_BAIXA);
}

void re() //testada
{
	OnRevSync(MOTORES, -VELOCIDADE_ALTA, 0);
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

	OnFwd(MOTOR_GARRA, VELOCIDADE_BAIXA);
	Wait(50);
	while(prev_motor != MotorRotationCount(MOTOR_GARRA)) // a garra irá se movimentar até travar na estrutura
	{
		prev_motor = MotorRotationCount(MOTOR_GARRA);
		Wait(50);
	}
	Off(MOTOR_GARRA);
	prev_motor = MotorRotationCount(MOTOR_GARRA);
	while(-90 < MotorRotationCount(MOTOR_GARRA) - prev_motor) // rotação necessária para a garra fechar um pouco e não agarrar no chão
	{
		OnFwd(MOTOR_GARRA, -VELOCIDADE_BAIXA);
	}
	Off(MOTOR_GARRA); // com essa função a garra fica na posição adequada para pegar o boneco
}

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

void agarrar()//testada
{
	int prev_motor;

	// aqui cabe uma função para movimentar o robô até que o sensor ultrassônico ache o boneco
	// valor de teste, mas já é uma distância que a garra consegue pegar o boneco

	OnFwd(MOTOR_GARRA, -VELOCIDADE_MEDIA);
	Wait(50);
	while (MotorRotationCount(MOTOR_GARRA) != prev_motor)
	{
		prev_motor = MotorRotationCount(MOTOR_GARRA);
		Wait(50);
	}

	Off(MOTOR_GARRA);
}



float getGyroOffset() //testada
{
    float gyro_sum = 0, i;

    for(i = 0; i < OFFSET_SAMPLES; ++i)
    {
       gyro_sum += SensorHTGyro(SENSOR_GYRO);
    }

    return gyro_sum/OFFSET_SAMPLES;
}

void girar(float degrees) // Algoritimo usado pela sek do ano passado //testada
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

void abrir_porta () //testada
{
	const float RAIO = 2.3405/2;
	const float CREMALHEIRA = 5.1;
	float Rotacao;
	Rotacao = (360*CREMALHEIRA)/(2*PI*RAIO);
	while (MotorRotationCount(MOTOR_PORTA) < Rotacao)
		{
		OnFwd(MOTOR_PORTA, 50);
		}
	Off(MOTOR_PORTA);
}

void fechar_porta () //testada
{
	while(MotorRotationCount(MOTOR_PORTA) > 0)
	{
	OnRev(MOTOR_PORTA, 50);
	}
	Off(MOTOR_PORTA);
}

void pegar_passageiro (int passageiros) //testado, mas precisa mudar a função gira para o robô girar no centro dele
{
	if(ultrassom_filtrado(SENSOR_US_ESQUERDA) < 25 && passageiros < 4){ //Ainda é necessário adaptar a função agarrar() pra depois de ela agarrar, ela voltar para a
															   //posição que o robô estava antes. Além disso, colocar para verificar se pegou o boneco
		/*re();
		Wait(300);*/
		Off(MOTORES);
		Wait(500);
		girar(-90);
		abaixar_garra();
		while (ultrassom_filtrado(SENSOR_US_GARRA) >= 21)
		{
			reto();
			Wait(25);
		}
		agarrar();
		girar(90);
	}
	else if(ultrassom_filtrado(SENSOR_US_DIREITA) < 25 && passageiros < 4){

		/*re();
		Wait(300);*/
		Off(MOTORES);
		Wait(500);
		girar(90);
		abaixar_garra();
		while (ultrassom_filtrado(SENSOR_US_GARRA) >= 21)
		{
			reto();
			Wait(25);
		}
		agarrar();
		girar(-90);
	}
	++passageiros;
}

task main ()
{
	ligar_sensores();


	float help;
	int  passageiros = 0;

	while (true)
	{

		reto();
		while(ultrassom_filtrado(SENSOR_US_ESQUERDA) > 25 && ultrassom_filtrado(SENSOR_US_DIREITA) > 25);
		Off(MOTORES);
		pegar_passageiro(passageiros);
		Off(MOTORES);
	}
}
