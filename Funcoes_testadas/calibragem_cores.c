#define CONEXAO 1 // Numero da porta BlueTooth

#define MOTOR_PORTA OUT_B
#define SENSOR_COR_ESQUERDA IN_4
#define SENSOR_COR_DIREITA IN_1

#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_2
#define SENSOR_GYRO IN_1
#define TOUCH IN_4

#define PRETO 1
#define AZUL 2
#define VERDE 3
#define AMARELO 4
#define VERMELHO 5
#define BRANCO 6
#define FORA 7

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
#define BLUEUP_B 310
#define	BLACKUP_B 200
#define	REDUP_B 220
#define GREENUP_B 310
#define BLUEUP_G 280
#define	BLACKUP_G 210
#define	REDUP_G 240
#define GREENUP_G 435
#define	REDUP_R 590
#define GREENUP_R 385

#define WHITEDOWN_R 560
#define REDDOWN_R 495
#define BLACKUP_R 270
#define BLUEDOWN_R 210
#define BLUEUP_R 310
#define FORAUP_R 450
#define FORADOWN_R 350

#define WHITEDOWN 579
#define WHITEUP 560 //<--------
#define REDUP 375 //<--------
#define REDDOWN 445
#define FORAUP 310 //<--------
#define FORADOWN 390
#define GREENUP 285 //<--------
#define GREENDOWN 367
#define BLUEUP 265 //<--------
#define BLUEDOWN 270
#define BLACKUP 225 //<--------
#define BLACKDOWN 243
#define DESVIO 5
#define OFFSET_COLOR 1/9.0

#define COL1 0
#define COL2 12
#define COL3 43
#define COL4 55

sub BTCheck(){
     if (!BluetoothStatus(CONEXAO)==NO_ERR){
          ClearScreen();
          TextOut(5,LCD_LINE2,"Erro");
          Wait(1000);
          Stop(true);
      }
}

void set_sensor_color(char porta, char color)
{
     if(color == VERMELHO){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORRED, SENSOR_MODE_RAW);
     }else if(color == VERDE){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORGREEN, SENSOR_MODE_RAW);
     }else if(color == AZUL){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORBLUE, SENSOR_MODE_RAW);
     }else if(color == BRANCO){
          RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORFULL, SENSOR_MODE_RAW);
     }
}

int get_value_color(char porta)
{
     InputValuesType params;

     params.Port = porta;
     RemoteGetInputValues(CONEXAO, params);

     return params.RawValue;
}

void ligar_sensores()
{
	SetSensorHTGyro(SENSOR_GYRO);
	set_sensor_color(SENSOR_COR_DIREITA, VERMELHO);
	Wait(100);
	set_sensor_color(SENSOR_COR_ESQUERDA, VERMELHO);
}
int abss(int x)
{
	if(x < 0)
		return -x;
	return x;
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
	leitura = (5*leitura_r + 3*leitura_g + leitura_b)/9;
	return leitura; // Se quisermos o valor entr 100 e 700
}

void get_two_rgb(int & leitura_e, int & leitura_d )
{
	// Essa funcao pega os dois valores simultaneamente, da esquerda e da direita
	int left[3], right[3];
	int CORES[3] = {VERMELHO, VERDE, AZUL};
	int LINE[3] = {LCD_LINE3, LCD_LINE4, LCD_LINE5};
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
		ClearLine(LINE[i]);
		if(i == 0){
			TextOut(COL1, LINE[i], "R");
			TextOut(COL3, LINE[i], "R");
		}else if(i == 1){
			TextOut(COL1, LINE[i], "G");
			TextOut(COL3, LINE[i], "G");
		}else{
			TextOut(COL1, LINE[i], "B");
			TextOut(COL3, LINE[i], "B");
		}
		NumOut( COL2, LINE[i], left[i]);
		NumOut( COL4, LINE[i], right[i]);
	}
	leitura_e = (5*left[0]+3*left[1]+left[2])/9;
	leitura_d = (5*right[0]+3*right[1]+right[2])/9;
}

int trata_leitura(int leitura)
{
	int cor;
	//comentar quando for ver parametros

	/*
	// Condicionais novas 
	int Mwhite 	= abss((WHITEUP + WHITEDOWN)/2	- leitura);
	int Mgreen 	= abss((GREENUP + GREENDOWN)/2	- leitura);
	int Mred	= abss( (REDUP   + REDDOWN) /2	- leitura);
	int Mblue	= abss((BLUEUP  + BLUEDOWN) /2	- leitura);
	int Mout 	= abss((FORAUP  + FORADOWN) /2	- leitura);
	int Mblack	= abss((BLACKUP + BLACKDOWN)/2	- leitura);
	if(Mwhite < Mred && Mwhite < Mout && Mwhite < Mgreen)
	{
		cor = BRANCO;
	}
	else if(Mred < Mout && Mred < Mgreen && Mred < Mblue)
	{
		cor = VERMELHO;
	}
	else if(Mout < Mgreen && Mout < Mblue)
	{
		cor = FORA;
	}
	else if(Mgreen < Mblue && Mgreen < Mblack)
	{
		cor = VERDE;
	}
	else if(Mblue < Mblack)
	{
		cor = AZUL;
	}
	else
	{
		cor = PRETO;
	}
	*/
	// Condicionais antigas
	if (leitura <= BLACKUP)
	{
		cor = PRETO;
	}
	else if (leitura <= BLUEUP)
	{
		cor = AZUL;
	}
	else if (leitura <= GREENUP)
	{
		cor = VERDE;
	}
	else if (leitura <= FORAUP)
	{
		cor = FORA;
	}
	else if (leitura <= REDUP)
	{
		cor = VERMELHO;
	}
	else
	{
		cor = BRANCO;
	}
	
	
	// Toca os sons
	
	int wait = 200, duration = 100, frequency = 1000;
	/*
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
	}*/
	if(cor == VERMELHO)
		PlayTone(1200, duration);
	else if(cor == VERDE)
		PlayTone(750, duration);
	else if(cor == AZUL)
		PlayTone(300, duration);
	else if(cor == BRANCO)
	{
		PlayTone(400, duration);
		Wait(wait);
		PlayTone(1200, duration);
	}
	else if(cor == PRETO)
	{
		PlayTone(400, duration);
		Wait(wait);
		PlayTone(750, duration);
	}
	else if(cor == FORA)
	{
		PlayTone(400, duration);
		Wait(wait);
		PlayTone(300, duration);
	}
	Wait(2*wait);
	return cor;		// Se quisermos o valor entre 1 e 7	
}

int teste_cor(int sensor)
{
	int cor, leitura;
	leitura = get_leitura_rgb(sensor);
	cor 	= trata_leitura(leitura);
	return cor;	
}

task main()
{
	int corD, corE, leit, leituraD, leituraE;
	ligar_sensores();
	//BTCheck();
	ClearScreen();
	TextOut(COL1, LCD_LINE1, "LEFT:");
	TextOut(COL3, LCD_LINE1, "RIGHT:");
	while(1)
	{
		// leituraD = get_leitura_rgb(SENSOR_COR_DIREITA);
		// leituraE = get_leitura_rgb(SENSOR_COR_ESQUERDA);
		
		get_two_rgb(leituraE, leituraD);
		
		ClearLine(LCD_LINE6);

		TextOut(COL1, LCD_LINE6, "T ");
		NumOut( COL2, LCD_LINE6, leituraE);

		TextOut(COL3, LCD_LINE6, "T ");		
		NumOut( COL4, LCD_LINE6, leituraD);
		
		corD 	 = trata_leitura(leituraD);
		corE 	 = trata_leitura(leituraE);

		ClearLine(LCD_LINE8);

		if(corE == VERMELHO) 	TextOut(COL2, LCD_LINE8, "RED");
		else if(corE == VERDE) 	TextOut(COL2, LCD_LINE8, "GREEN");
		else if(corE == AZUL) 	TextOut(COL2, LCD_LINE8, "BLUE");
		else if(corE == BRANCO) TextOut(COL2, LCD_LINE8, "WHITE");
		else if(corE == PRETO)	TextOut(COL2, LCD_LINE8, "BLACK");
		else					TextOut(COL2, LCD_LINE8, "OUT");
		
		if(corD == VERMELHO) 	TextOut(COL4, LCD_LINE8, "RED");
		else if(corD == VERDE) 	TextOut(COL4, LCD_LINE8, "GREEN");
		else if(corD == AZUL) 	TextOut(COL4, LCD_LINE8, "BLUE");
		else if(corD == BRANCO) TextOut(COL4, LCD_LINE8, "WHITE");
		else if(corD == PRETO)	TextOut(COL4, LCD_LINE8, "BLACK");
		else					TextOut(COL4, LCD_LINE8, "OUT");

		Wait(100);

	}
}