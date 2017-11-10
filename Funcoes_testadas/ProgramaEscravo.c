//SLAVE
#define CONEXAO 0
#define INBOX 5
#define OUTBOX 1

sub BTCheck(int conn){
    if (!BluetoothStatus(conn)==NO_ERR){
        TextOut(5,LCD_LINE2,"Error");
        Wait(1000);
        Stop(true);
    }
}


#define US_LEFT IN_1
#define US_RIGHT IN_2
#define PORTAO OUT_B

#define SENSIBILIDADE 0.9

#define TEM_BONECO true
#define NAO_TEM_BONECO false

float ultrassom_filtrado_ponderado(int sensor) //testada
{
	float valor = SensorUS(sensor);
	float aux;
	for (int i = 0; i < 5; ++i)
	{
		aux = SensorUS(sensor);

		if (aux < 19)
		{
			valor = (valor * SENSIBILIDADE * 0.3) + (aux * (1-SENSIBILIDADE) * 0.7); //ponderando para medidas proximas aparecerem mais rapido
		}

		else
		{
			
			valor = valor * SENSIBILIDADE + aux * (1-SENSIBILIDADE); // Algoritimo passado pelo B.Andreguetti da aula de SisMed

		}
	}
	return valor;
}

bool verifica_boneco(int sensor)
{
	float distancia;

	distancia = ultrassom_filtrado_ponderado(sensor);

	if (distancia < 17)
	{
		return TEM_BONECO;
	}
	else
	{
		return NAO_TEM_BONECO;
	}
}

int achou_boneco()
{
	bool left, right;
	int soma = 0;
	//PlayTone(1000, 100);
	// Retorna 0 caso nao tenha achado nenhum boneco nas laterais
	// Retorna 1 caso ache o boneco pela esquerda
	// Retorna 2 caso ache o boneco pela direita
	// Retorna 3 caso ache o boneco nos dois lados
	left = verifica_boneco(US_LEFT);
	right = verifica_boneco(US_RIGHT);
	if(left)
		soma += 1;
	if(right)
		soma += 2;
	return soma;
}

void abre_porta()
{
	int prev_motor;
	int inicial_motor;
	
	// aqui cabe uma função para movimentar o robô até que o sensor ultrassônico ache o boneco
	// valor de teste, mas já é uma distância que a garra consegue pegar o boneco

	inicial_motor = MotorRotationCount(MOTOR_GARRA);

	OnFwd(PORTAO, -VELOCIDADE_BAIXINHA);
	Wait(50);
	while (MotorRotationCount(PORTAO) != prev_motor)
	{
		prev_motor = MotorRotationCount(PORTAO);
		Wait(50);
	}

	Off(PORTAO);
	
}

void fecha_porta()
{
	int prev_motor;
	int inicial_motor;
	
	// aqui cabe uma função para movimentar o robô até que o sensor ultrassônico ache o boneco
	// valor de teste, mas já é uma distância que a garra consegue pegar o boneco

	inicial_motor = MotorRotationCount(MOTOR_GARRA);

	OnRev(PORTAO, -VELOCIDADE_BAIXINHA);
	Wait(50);
	while (MotorRotationCount(PORTAO) != prev_motor)
	{
		prev_motor = MotorRotationCount(PORTAO);
		Wait(50);
	}

	Off(PORTAO);
	
}


task main(){
	int found = 3;
	int temp;
	
	// Definicao dos ultrassonicos
	SetSensorUltrasonic(US_LEFT);
	SetSensorUltrasonic(US_RIGHT);

	BTCheck(CONEXAO); //check master connection
	Wait(500);
	/*
	while(true){


		PlayTone(500, 200);
		achou_boneco(left, right);
		if(left || right) // Achou boneco na esquerda
		{
			if(left)
				sucesso = SendRemoteBool(CONEXAO, OUTBOX, false);
			else
				sucesso = SendRemoteBool(CONEXAO, OUTBOX, true);
			//			if(RemoteConnectionIdle(CONEXAO))
		}
		TextOut(10,LCD_LINE1,"Slave Test");
		TextOut(0,LCD_LINE3,"Left:");
		TextOut(0,LCD_LINE4,"Right:");
		if(left)
		{
			TextOut(50,LCD_LINE3,"Sim!");	
		}
		else
		{
			TextOut(50,LCD_LINE3,"Nao!");
		}
		if(right)
		{
			TextOut(50,LCD_LINE4,"Sim!");
		}
		else
		{
			TextOut(50,LCD_LINE4,"Nao!");
		}
		Wait(100);
		
		TextOut(10,LCD_LINE1,"Slave Test");
		TextOut(0,LCD_LINE2,"Sensor 1:");
		TextOut(0,LCD_LINE4,"Sensor 2:");
		SendRemoteString(BT_CONN, OUTBOX,sensor1);
		SendRemoteString(BT_CONN, OUTBOX,sensor2);
		TextOut(10,LCD_LINE3,sensor1);
		TextOut(10,LCD_LINE5,sensor2);
		Wait(100);
		
	}*/
	while(true){
        TextOut(10,LCD_LINE1,"Slave Test");
        TextOut(0,LCD_LINE2,"Status:");
        found = achou_boneco();
        NumOut(0,LCD_LINE8, found);
        ClearLine(LCD_LINE3);
        temp = -2;
        if (ReceiveRemoteNumber(INBOX,true,temp) != STAT_MSG_EMPTY_MAILBOX)
        {
        	if(temp == 1)
        		abre_porta();
        	else if(temp == 2)
        		fecha_porta();
        	NumOut(60,LCD_LINE8, found);
            TextOut(10,LCD_LINE3,"Enviando");
            SendResponseNumber(OUTBOX, found);
        }
        else
        {
            TextOut(10,LCD_LINE3,"Nao enviando");
        }
        NumOut(30,LCD_LINE8, found);
        TextOut(0,LCD_LINE4,"Left:");
        TextOut(0,LCD_LINE6,"Right:");
		if(found %2 == 1)
			TextOut(10,LCD_LINE5, "SIM!");
		else
			TextOut(10,LCD_LINE5, "NAO!");
		if(found /2 == 1)
			TextOut(10,LCD_LINE7, "SIM!");
		else
			TextOut(10,LCD_LINE7, "NAO!");
		//Wait(20);
    }
}
