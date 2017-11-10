#define BT_CONN 1
#define INBOX 1
#define OUTBOX 5

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

#define VELOCIDADE_BAIXINHA 17
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 45
#define VELOCIDADE_ALTA 60
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
#define POT_MAX 90

#define CORRECAO 0.01
#define RAZAO 77/70

#define ESQUERDA 1
#define DIREITA -1
#define FRENTE 0
#define NADA 2
#define NAO_ESQUERDA 3
#define NAO_DIREITA 4
#define INVERTER 5
#define NORMAL 6

#define OFFSET_COLOR 1/9.0

#define SIZE 3*4*10 // 3 inteiros
#define NAME "direcoes.dat"

#define TEM_BONECO 1
#define NAO_TEM_BONECO 0

int passageiros = 0;
int maximo = 40;
int ultimo_fora = 0;
byte VERMELHO_r, VERDE_r, AZUL_r, BRANCO_r, num_r, VERMELHO_l, VERDE_l, AZUL_l, BRANCO_l, num_l, color_r, color_l;

sub BTCheck(){
     if (!BluetoothStatus(CONEXAO)==NO_ERR){
          ClearScreen();
          TextOut(5,LCD_LINE2,"Erro");
          Wait(1000);
          Stop(true);
      }
}

int alerta(int frequency)
{
	int i;
	for(i = 0; i < 20; i++)
	{
		PlayTone(frequency, 100);
		Wait(150);
	}
}

/*void shutdown( const int delay)
{
   if (handle) CloseFile(handle);
   // Get user's attention.
   // Give the user time to read screen messages.
   Wait(delay);
   Stop(true);
}*/

void imprime_erro(int p)
{
	ClearScreen();
	if(p == LDR_SUCCESS)
		TextOut(10, LCD_LINE1, "LDR_SUCCESS");
	else if(p == LDR_INPROGRESS)
		TextOut(10, LCD_LINE1, "LDR_INPROGRESS");
	else if(p == LDR_REQPIN)
		TextOut(10, LCD_LINE1, "LDR_REQPIN");
	else if(p == LDR_NOMOREHANDLES)
		TextOut(10, LCD_LINE1, "LDR_NOMOREHANDLES");
	else if(p == LDR_NOSPACE)
		TextOut(10, LCD_LINE1, "LDR_NOSPACE");
	else if(p == LDR_NOMOREFILES)
		TextOut(10, LCD_LINE1, "LDR_NOMOREFILES");
	else if(p == LDR_EOFEXPECTED)
		TextOut(10, LCD_LINE1, "LDR_EOFEXPECTED");
	else if(p == LDR_ENDOFFILE)
		TextOut(10, LCD_LINE1, "LDR_ENDOFFILE");
	else if(p == LDR_NOTLINEARFILE)
		TextOut(10, LCD_LINE1, "LDR_NOTLINEARFILE");
	else if(p == LDR_FILENOTFOUND)
		TextOut(10, LCD_LINE1, "LDR_FILENOTFOUND");
	else if(p == LDR_HANDLEALREADYCLOSED)
		TextOut(10, LCD_LINE1, "LDR_HANDLEALREADYCLOSED");
	else if(p == LDR_NOLINEARSPACE)
		TextOut(10, LCD_LINE1, "LDR_NOLINEARSPACE");
	else if(p == LDR_UNDEFINEDERROR)
		TextOut(10, LCD_LINE1, "LDR_UNDEFINEDERROR");
	else if(p == LDR_FILEISBUSY)
		TextOut(10, LCD_LINE1, "LDR_FILEISBUSY");
	else if(p == LDR_NOWRITEBUFFERS)
		TextOut(10, LCD_LINE1, "LDR_NOWRITEBUFFERS");
	else if(p == LDR_APPENDNOTPOSSIBLE)
		TextOut(10, LCD_LINE1, "LDR_APPENDNOTPOSSIBLE");
	else if(p == LDR_FILEISFULL)
		TextOut(10, LCD_LINE1, "LDR_FILEISFULL");
	else if(p == LDR_FILEEXISTS)
		TextOut(10, LCD_LINE1, "LDR_FILEEXISTS");
	else if(p == LDR_MODULENOTFOUND)
		TextOut(10, LCD_LINE1, "LDR_MODULENOTFOUND");
	else if(p == LDR_OUTOFBOUNDARY)
		TextOut(10, LCD_LINE1, "LDR_OUTOFBOUNDARY");
	else if(p == LDR_ILLEGALFILENAME)
		TextOut(10, LCD_LINE1, "LDR_ILLEGALFILENAME");
	else if(p == LDR_ILLEGALHANDLE)
		TextOut(10, LCD_LINE1, "LDR_ILLEGALHANDLE");
	else if(p == LDR_BTBUSY)
		TextOut(10, LCD_LINE1, "LDR_BTBUSY");
	else if(p == LDR_BTCONNECTFAIL)
		TextOut(10, LCD_LINE1, "LDR_BTCONNECTFAIL");
	else if(p == LDR_BTTIMEOUT)
		TextOut(10, LCD_LINE1, "LDR_BTTIMEOUT");
	else if(p == LDR_FILETX_TIMEOUT)
		TextOut(10, LCD_LINE1, "LDR_FILETX_TIMEOUT");
	else if(p == LDR_FILETX_DSTEXISTS)
		TextOut(10, LCD_LINE1, "LDR_FILETX_DSTEXISTS");
	else if(p == LDR_FILETX_SRCMISSING)
		TextOut(10, LCD_LINE1, "LDR_FILETX_SRCMISSING");
	else if(p == LDR_FILETX_STREAMERROR)
		TextOut(10, LCD_LINE1, "LDR_FILETX_STREAMERROR");
	else if(p == LDR_FILETX_CLOSEERROR)
		TextOut(10, LCD_LINE1, "LDR_FILETX_CLOSEERROR");
	else if(p == LDR_INVALIDSEEK)
		TextOut(10, LCD_LINE1, "LDR_INVALIDSEEK");
	else
		TextOut(10, LCD_LINE1, "NENHUM");
	Wait(1500);
}

int write_direcoes(int direcoes[]) // Retorna 1 caso consiga gravar
{
	byte CORES[3] = {VERMELHO, VERDE, AMARELO};
	unsigned int file_size = SIZE;
	int p, i, d, handle = 0;

	// Start with the assumptions the file doesn't exist and needs to be created.
	p = CreateFile(NAME, file_size, handle);
	// If the file already exists, open it with the intent of adding to the data
	// that is already there.
	if (p == LDR_FILEEXISTS)
	{
		p = DeleteFile(NAME);
		if(p == LDR_SUCCESS)
		{
			p = CreateFile(NAME, file_size, handle);
			if(p != LDR_SUCCESS)
			{
				return 2;
			}
			
		}
		else
		{
			// Deu algum erro, acho que nunca vai entrar aqui
			return 3;
		}
	}
	for(i = 0; i < 3; i++) // Pois sao 3 cores para gravar as direcoes
	{
		PlayTone(880, 300);
		Wait(500);
		PlayTone(880,300);
		d = direcoes[CORES[i]];
		p = Write(handle, d);
		if (p != LDR_SUCCESS)
		{
			break;
		}
	}
	if (handle)
		CloseFile(handle);
	if(p != LDR_SUCCESS){
		imprime_erro(p);
		return 0;
	}
	return 1;
}




int read_direcoes(int &d1, int &d2, int &d3)
{
	int p, i, d, handle = 0;
	const int file_size = SIZE;
	p = OpenFileRead(NAME, file_size, handle);
	// Return code handling
	ClearScreen();
	NumOut(30, 20, p);
	NumOut(40,30,LDR_SUCCESS);
	Wait(1000);
	if (p != LDR_SUCCESS)
	{
		PlayTone(220,500);
		return 0;
	}
	for(i = 0; i < 3; i++)
	{
		

		p = Read(handle, d);
		if (p != LDR_SUCCESS)
		{
			PlayTone(500,1500);
			d1 = NADA;
			d2 = NADA;
			d3 = NADA;
			return 2;
		}
		if(i == 0)
			d1 = d;
		else if(i == 1)
			d2 = d;
		else if(i == 2)
			d3 = d;
	}
	if(handle) CloseFile(handle);
	return 1;
}

void printf_arquivo(int direcoes[])
{
	ClearScreen();
	int CORES[3] = {AMARELO, VERMELHO, VERDE};
	int d1, d2, d3;
	read_direcoes(d1, d2, d3);
	direcoes[CORES[0]] = d1;
	direcoes[CORES[1]] = d2;
	direcoes[CORES[2]] = d3;
	TextOut(0, 0, "AMARELO: ");
	NumOut(35, 0, direcoes[CORES[0]]);
	TextOut(0, 15, "VERMELHO: ");
	NumOut(35,15, direcoes[CORES[1]]);
	TextOut(0,30, "VERDE");
	NumOut(35,30, direcoes[CORES[2]]);
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
		if (BRANCO_l < 225)
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
	if (BRANCO_l > 120){
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
else if (num_r >= 9 && num_r <= 11 )
{
	color_r = VERMELHO;
}
else if (num_r == 12)
{
	color_r = PRETO;
}
else if (num_r >= 13 && num_r <= 15)
{
	if (num_r == 14)
	{
		if (BRANCO_r < 225)
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
	if (BRANCO_r > 70){ //SE DER MERDA E AQUI, calibrar melhor
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
			do {
				OnRev(MOTOR_ESQUERDA, low_speed);
			} while ((count_C - MotorRotationCount(MOTOR_DIREITA)) >  (count_A - MotorRotationCount(MOTOR_ESQUERDA)) && ((count_A - MotorRotationCount(MOTOR_ESQUERDA))*6*PI/360 <= distancia));
			OnRev(MOTOR_ESQUERDA, high_speed);
		}
		else
		{
			do {
				OnRev(MOTOR_DIREITA, low_speed);
			}while ( (count_A - MotorRotationCount(MOTOR_ESQUERDA)) > (count_C - MotorRotationCount(MOTOR_DIREITA)) && ((count_C - MotorRotationCount(MOTOR_DIREITA))*6*PI/360 <= distancia));
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
			do{
			OnFwd(MOTOR_ESQUERDA, low_speed);
			}while (((MotorRotationCount(MOTOR_DIREITA) - count_C) >  (MotorRotationCount(MOTOR_ESQUERDA) - count_A)) && ((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia));
			OnFwd(MOTOR_ESQUERDA, high_speed);
		}
		else
		{
			do{
			OnFwd(MOTOR_DIREITA, low_speed);
			}while ( ((MotorRotationCount(MOTOR_ESQUERDA) - count_A) > (MotorRotationCount(MOTOR_DIREITA) - count_C)) && ((MotorRotationCount(MOTOR_DIREITA) - count_C)*6*PI/360 <= distancia));
			OnFwd(MOTOR_DIREITA, high_speed);
		}
	}while(((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia) && ((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia));
	Off(AMBOS_MOTORES);

}

void ajeitar_frente_branco()//ajeita na cor em frente ao branco
{
	int contador = 0;
	{
		while((sensor_cor(SENSOR_COR_DIREITA) != BRANCO || sensor_cor(SENSOR_COR_ESQUERDA) != BRANCO)) // Aqui ele comeca a ajeitar, para caso chegue torto, ele fique certo no final
		{
			Off(AMBOS_MOTORES);
			//while(sensor_cor(SENSOR_COR_ESQUERDA) == FORA) OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
			//while(sensor_cor(SENSOR_COR_DIREITA) == FORA ) OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);

			Off(AMBOS_MOTORES);
			contador = 0;
			while(sensor_cor(SENSOR_COR_DIREITA) != BRANCO && contador < 10)
			{ // Ajusta a roda esquerda para ficar em cima da linha.
				OnFwd(MOTOR_DIREITA,  (-1)*VELOCIDADE_MEDIA);
				Wait(10);
				contador += 1;
			}
			Off(AMBOS_MOTORES);
			contador = 0;
			while(sensor_cor(SENSOR_COR_ESQUERDA) != BRANCO && contador < 10)
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
	ultimo_fora = 0;
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
			Off(AMBOS_MOTORES);
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

		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 9);
		giro(90);
	}
	if (degrees != 90 && degrees != -90 && degrees != 180)
		giro(degrees);
}


/*float ultrassom_filtrado(int sensor) //testada
{
	float valor = SensorUS(sensor);
	float aux;
	for (int i = 0; i < 3; ++i)
	{
		aux = SensorUS(sensor);
		valor = valor * SENSIBILIDADE + aux * (1-SENSIBILIDADE); // Algoritimo passado pelo B.Andreguetti da aula de SisMed
	}
	return valor;
}*/


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
		OnFwd(MOTOR_GARRA, VELOCIDADE_MEDIA);
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
	int confirma_que_pegou;

	// aqui cabe uma função para movimentar o robô até que o sensor ultrassônico ache o boneco
	// valor de teste, mas já é uma distância que a garra consegue pegar o boneco

	OnFwd(MOTOR_GARRA, -20);
	Wait(50);
	while (MotorRotationCount(MOTOR_GARRA) != prev_motor)
	{
		prev_motor = MotorRotationCount(MOTOR_GARRA);
		Wait(100);
	}

	OnFwd(MOTOR_GARRA, -45);
	while (MotorRotationCount(MOTOR_GARRA) != prev_motor)
	{
		prev_motor = MotorRotationCount(MOTOR_GARRA);
		Wait(100);
	}

	Off(MOTOR_GARRA);

	confirma_que_pegou = 1;

	return confirma_que_pegou;
}

int pegar_passageiro () //testado, mas precisa mudar a função gira para o robô girar no centro dele
{

	int confirma_que_pegou = 0;

	if(/*lado == DIREITA && */passageiros < 5){ //Ainda é necessário adaptar a função agarrar() pra depois de ela agarrar, ela voltar para a
															   //posição que o robô estava antes. Além disso, colocar para verificar se pegou o boneco
		Off(AMBOS_MOTORES);
		Wait(500);
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 17);
		giro(90);
		abaixar_garra();
		distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 18);
		Off(AMBOS_MOTORES);
		confirma_que_pegou = agarrar();
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 15);
		giro(-90);
	}
	/*else if(lado == ESQUERDA && passageiros < 5){ //variavel lado valendo 0 corresponde a esquerda e valendo 1 corresponde à direta, bjs

		Off(AMBOS_MOTORES);
		Wait(500);
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 17);
		giro(90);
		abaixar_garra();
		distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 18);
		Off(AMBOS_MOTORES);
		confirma_que_pegou = agarrar();
		ClearScreen();
		NumOut(10,10, confirma_que_pegou);
		Wait(2000);
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 15);
		giro(-80);
	}

	/*if(passageiros < 3){

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

	if (confirma_que_pegou == 1)
	{
		++passageiros;
	}

	return passageiros;
}

void verifica_boneco()
{
	float distancia;

	distancia = ultrassom_filtrado_ponderado(SENSOR_US_ESQUERDA);

	if (distancia < 17)
	{
		pegar_passageiro();
	}
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
}

void retinho(int velocidadeR)
{
	float erro;
	int velo1 = 0, velo2 = 0, velocidadeL = velocidadeR * RAZAO;

	if(velocidadeL > POT_MAX) velocidadeL = POT_MAX;
	if(velocidadeL < -POT_MAX) velocidadeL = -POT_MAX;
	if(velocidadeR > POT_MAX) velocidadeR = POT_MAX;
	if(velocidadeR < -POT_MAX) velocidadeR = -POT_MAX;
	OnFwd(MOTOR_DIREITA, -velocidadeR);
	OnFwd(MOTOR_ESQUERDA, -velocidadeL);
	Wait(100);
	while(erro > -0.5 && erro < 0.5){
		erro = SensorHTGyro(SENSOR_GYRO);
	}
	velo1 = velocidadeR - CORRECAO * erro;
	velo2 = velocidadeL + CORRECAO * erro;
	if (velo1 > POT_MAX) velo1 = POT_MAX;
	else if (velo1 < -POT_MAX) velo1 = -POT_MAX;
	if (velo2 > POT_MAX) velo2 = POT_MAX;
	else if (velo2 < -POT_MAX) velo2 = -POT_MAX;
	Off(AMBOS_MOTORES);
	OnFwd(MOTOR_DIREITA, -velo1);
	OnFwd(MOTOR_ESQUERDA, -velo2);
}

void reto(int cor) //robo move ate que os dois sensores parem de ver a cor
{
	int contador = 0;
	if (cor == BRANCO)
	{
		while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
		{
			OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, -6);
			retinho(VELOCIDADE_ALTA);
		
			while((sensor_cor(SENSOR_COR_ESQUERDA) == FORA || sensor_cor(SENSOR_COR_ESQUERDA) == PRETO) && sensor_cor(SENSOR_COR_DIREITA) == BRANCO)
			{
				OnRevSync(AMBOS_MOTORES, - VELOCIDADE_BAIXA, 0); // Da uma pequena re
				Wait(200);
				Off(AMBOS_MOTORES);
				OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
				OnRev(MOTOR_DIREITA,  -VELOCIDADE_BAIXA);
				ClearScreen();
				//PlayTone(400, 100);
				TextOut(50,50, "E:P");
				ultimo_fora = ESQUERDA;
			}
			while((sensor_cor(SENSOR_COR_DIREITA) == FORA || sensor_cor(SENSOR_COR_DIREITA) == PRETO) && sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO)
			{
				OnRevSync(AMBOS_MOTORES, - VELOCIDADE_BAIXA, 0); // Da uma pequena re
				Wait(200);
				Off(AMBOS_MOTORES);
				OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
				OnRev(MOTOR_ESQUERDA, -VELOCIDADE_BAIXA);
				ClearScreen();
				//PlayTone(800, 100);
				TextOut(50,50, "D:P");
				ultimo_fora = DIREITA;
			}
			while(sensor_cor(SENSOR_COR_ESQUERDA) != BRANCO && sensor_cor(SENSOR_COR_ESQUERDA) != FORA  && sensor_cor(SENSOR_COR_ESQUERDA) != PRETO && sensor_cor(SENSOR_COR_DIREITA) == BRANCO)
			{
				Off(MOTOR_ESQUERDA);
				OnFwd(MOTOR_DIREITA, -VELOCIDADE_MEDIA);
			}
			while(sensor_cor(SENSOR_COR_DIREITA) != BRANCO && sensor_cor(SENSOR_COR_DIREITA) != FORA && sensor_cor(SENSOR_COR_DIREITA) != PRETO && sensor_cor(SENSOR_COR_ESQUERDA) == BRANCO)
			{
				Off(MOTOR_DIREITA);
				OnFwd(MOTOR_ESQUERDA, -VELOCIDADE_MEDIA);
			}
			ClearScreen();
			
			verifica_boneco();
			
		}
	} else
	{
		while (sensor_cor(SENSOR_COR_DIREITA) == cor || sensor_cor(SENSOR_COR_ESQUERDA) == cor)
		{
			OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, -6);
			retinho(VELOCIDADE_ALTA);
			corrige_intersecao(cor);
			ClearScreen();
			//PlayTone(400, 100);
		}
		Off(AMBOS_MOTORES);
		ajeitar(cor);
	}
	Off(AMBOS_MOTORES);
	//ajeitar(cor); //melhor não ajeitar no branco dentro da função

	//alterei o laço para dentro da função recenbendo a cor como argumento
}

int modo_plaza (int direcoes[])
{
	int aux, prev_motor; //Essa ultima constante é para armazenar o ultimo parâmetro do Sync
	SetSensorHTGyro(SENSOR_GYRO);
	float angle = 0, gyro = 0, angle_inicial = 0, result, offset_velocidade = 0, tolerancia = 40.0;


	PlayTone(880, 500);


	ResetRotationCount(MOTOR_DIREITA);
	ResetRotationCount(MOTOR_ESQUERDA);
	OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, -6);

	PlayTone(880, 500);

	while (color_r !=PRETO && color_l !=PRETO)
	{
		retinho(VELOCIDADE_ALTA);
		detect_colors();
	}
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 10);
	while (color_r !=PRETO || color_l !=PRETO)
	{
		retinho(VELOCIDADE_ALTA);
		detect_colors();
	}
	aux = abs(MotorRotationCount(MOTOR_DIREITA));
	girar_sem_tempo(175);
	ResetRotationCount(MOTOR_DIREITA);
	ResetRotationCount(MOTOR_ESQUERDA);
	MOTOR(MOTOR_PORTA, VELOCIDADE_BAIXA);
	Wait(400);
	MOTOR(MOTOR_PORTA, 0);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 10);
	MOTOR(MOTOR_PORTA, -VELOCIDADE_BAIXA);
	Wait(400);
	MOTOR(MOTOR_PORTA, 0);

	retinho(VELOCIDADE_ALTA);

	while(prev_motor < aux)
	{
		Wait(50);
		prev_motor = abs(MotorRotationCount(MOTOR_DIREITA));
	}

	reto(BRANCO);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 15);


	Off(AMBOS_MOTORES);
	passageiros = 0;

	return INVERTER;
}

bool verificar_direcao(int cor, int tentativa, int direcoes[])
{
	int cor_d, cor_e;
	reto(cor);
	ajeitar_frente_branco();
	reto(BRANCO);
	cor_d = sensor_cor(SENSOR_COR_DIREITA);
	cor_e = sensor_cor(SENSOR_COR_ESQUERDA);
	if ((cor_e == PRETO || cor_e == FORA) && (cor_d == PRETO || cor_d == FORA))
	{
		PlayTone(440,500);
		if (tentativa == ESQUERDA)
		{
			direcoes[cor] = NAO_ESQUERDA;
			write_direcoes(direcoes);
			printf_arquivo(direcoes);
			Wait(5000);
		}
		if (tentativa == DIREITA)
		{
			direcoes[cor] = FRENTE;
		}
	}
	ajeitar(BRANCO);
	ajeitar_frente();

	if ((cor_e == PRETO || cor_e == FORA) && (cor_d == PRETO || cor_d == FORA))//se os dois nao veem preto entao o robo acertou o caminho
	{
		TextOut(10,10, "caminho errado");
		return false;
	}
	TextOut(10, 10, "Caminho certo");
	return true;
}

void voltar(int cor)//voltar para o quadrado de origem visto que errou o caminho
{
	//ajeitar(BRANCO);
	if (ultimo_fora == ESQUERDA)
	{
		girar_sem_tempo(135); //se a ultima que o sensor saiu foi pela esquerda, o robo gira pela direita, ou seja, argumento positivo
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 4);
		girar_sem_tempo(35);
	}
	else if (ultimo_fora == DIREITA)
	{
		girar_sem_tempo(-135); //estamos girando 170 para que a caster ball não saia da arena
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 4);
		girar_sem_tempo(-35);
	}
	else
	{
		girar_sem_tempo(135); //se a ultima que o sensor saiu foi pela esquerda, o robo gira pela direita, ou seja, argumento positivo
		distancia_re(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 4);
		girar_sem_tempo(35);
	}
	ultimo_fora = 0;
	reto(BRANCO);
	PlayTone(880, 500);
	ajeitar(BRANCO);
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
	reto(cor);
	ajeitar(cor);
}

int testar_caminho(int cor, int direcoes[])//testa as direções verificando se ja há alguma cor com a direção
{
	if (cor == VERMELHO)
	{
		distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
	}
	if (cor == VERMELHO && (sensor_cor(SENSOR_COR_ESQUERDA) == AZUL || sensor_cor(SENSOR_COR_ESQUERDA) == VERDE))
	{
		return modo_plaza(direcoes);
	}
	else if (cor == VERMELHO && (sensor_cor(SENSOR_COR_DIREITA) == AZUL || sensor_cor(SENSOR_COR_DIREITA) == VERDE))
	{
		return modo_plaza(direcoes);
	} else if (cor == VERMELHO)
	{
		ajeitar(VERMELHO);
	}
	if (direcoes[AZUL] != ESQUERDA && direcoes[VERMELHO] != ESQUERDA && direcoes[VERDE] != ESQUERDA && direcoes[cor] != NAO_ESQUERDA)
	{
		TextOut(10,10, "teste esquerda");
		ClearScreen();
		girar(90);
		ClearScreen();

		if (verificar_direcao(cor, ESQUERDA, direcoes))
		{
			return ESQUERDA;
		}
		direcoes[cor] = NAO_ESQUERDA;
		write_direcoes(direcoes);
		voltar(cor);

		if (direcoes[AZUL] != DIREITA && direcoes[VERMELHO] != DIREITA && direcoes[VERDE] != DIREITA && direcoes[cor] != NAO_DIREITA)
		{
			if (verificar_direcao(cor, DIREITA, direcoes))
			{
				return DIREITA;
			}
			direcoes[cor] = FRENTE;
			write_direcoes(direcoes);
			voltar(cor);
			girar(-90);
			verificar_direcao(cor, FRENTE, direcoes);
			return FRENTE;
		} else
		{
			direcoes[cor] = DIREITA;
			write_direcoes(direcoes);
			girar(90);
			verificar_direcao(cor, FRENTE, direcoes);
			return FRENTE;
		}
	}
	if (direcoes[AZUL] != DIREITA && direcoes[VERMELHO] != DIREITA && direcoes[VERDE] != DIREITA && direcoes[cor] != NAO_DIREITA)
	{
		TextOut(10,10, "teste direita");
		girar(-90);
		if (verificar_direcao(cor, DIREITA, direcoes))
		{
			return DIREITA;
		}
		direcoes[cor] = FRENTE;
		write_direcoes(direcoes);
		voltar(cor);
		girar(90);
		verificar_direcao(cor, FRENTE, direcoes);
		return FRENTE;
	}
	TextOut(10,10, "teste frente");
	direcoes[cor] = FRENTE;
	write_direcoes(direcoes);
	verificar_direcao(cor, FRENTE, direcoes);
	return FRENTE;
}


int seguir_direcao(int cor, int direcoes[])//função que sera usada quando a cor ja tiver uma direção definida
{
	if (cor == VERMELHO)
	{
		distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
	}
	if (cor == VERMELHO && (sensor_cor(SENSOR_COR_ESQUERDA) == AZUL || sensor_cor(SENSOR_COR_ESQUERDA) == VERDE))
	{
		return modo_plaza(direcoes);
	}
	else if (cor == VERMELHO && (sensor_cor(SENSOR_COR_DIREITA) == AZUL || sensor_cor(SENSOR_COR_DIREITA) == VERDE))
	{
		return modo_plaza(direcoes);
	}
	else if (cor == VERMELHO)
	{
		ajeitar(VERMELHO);
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
	distancia_reto(VELOCIDADE_MEDIA, VELOCIDADE_ALTA, 5);
	return NORMAL;
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

bool existe_arquivo()
{
	int p, handle = 0;
	int file_size = SIZE;
	p = OpenFileRead(NAME, file_size, handle);
	if(p != LDR_SUCCESS)
		return false;
	CloseFile(handle);
	return true;
}

task main () //por enquato a maior parte está só com a lógica, tem que alterar as funções pra ele conseguir andar certinho e girar
{
	int cor_achada, auxiliar;
	int CORES[3] = {AMARELO, VERMELHO, VERDE};
	int direcoes[6] = {NADA, NADA, NADA, NADA, NADA, NADA};
	int i;
	BTCheck();
	ligar_sensores();
 	Wait(1000);

 	int d1, d2, d3;
	int handle = 0;
	int r;
	if(!existe_arquivo())
	{
		write_direcoes(direcoes);
	} else
	// Comeca a leitura das direcoes
	 // Conseguiu abrir o arquivo
	{
		read_direcoes(d1, d2, d3);
		direcoes[CORES[0]] = d1;
		direcoes[CORES[1]] = d2;
		direcoes[CORES[2]] = d3;
	}
	direcoes[VERDE]= DIREITA;
	ClearScreen();
	r = write_direcoes(direcoes);
	NumOut(50,10, r);
	Wait(1500);
	ClearScreen();
	r = read_direcoes(d1, d2, d3);
	NumOut(50,20, r);
	Wait(1500);
	
	NumOut(10,10, d1);
	NumOut(20,20, d2);
	NumOut(30,30, d3);
	Wait(3000);

	

	 
 	MOTOR(MOTOR_PORTA, -10);
 	Wait(200);
	MOTOR(MOTOR_PORTA, 0);

	while (true){
		reto(BRANCO);
		ajeitar(BRANCO);
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
				if (direcoes [CORES[i]] == NADA)
				{
					ClearScreen();
					TextOut(10,10, "NADA GRAVADO");
					auxiliar = testar_caminho(CORES[i], direcoes);
					if (auxiliar = INVERTER)
					{
						for (int i = 0; i < 5; ++i)
						{
							direcoes[i] = -direcoes[i];
						}
					}
					else
					{
						direcoes[CORES[i]] = auxiliar;
						write_direcoes(direcoes);
					}
				} else
				{
					auxiliar = seguir_direcao(CORES[i], direcoes);
					if (auxiliar = INVERTER)
					{
						for (int i = 0; i < 5; ++i)
						{
							direcoes[i] = -direcoes[i];
						}
					}
				}
				break;
			}
		}
	}
}