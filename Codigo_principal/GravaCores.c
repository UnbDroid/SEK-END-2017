#define CONEXAO 1 // Numero da porta BlueTooth

#define SENSOR_COR_ESQUERDA IN_4
#define SENSOR_COR_DIREITA IN_1

#define TOUCH IN_4
#define press Sensor(TOUCH)

#define PRETO 1
#define AZUL 2
#define VERDE 3
#define AMARELO 4
#define VERMELHO 5
#define BRANCO 6
#define FORA 7

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
     }else{
     	RemoteSetInputMode(CONEXAO, porta, SENSOR_TYPE_COLORNONE, SENSOR_MODE_RAW);
     }
}

int get_value_color(char porta)
{
     InputValuesType params;

     params.Port = porta;
     RemoteGetInputValues(CONEXAO, params);

     return params.RawValue;
}

void ligar_sensores(int color)
{
	SetSensorTouch(TOUCH); // Liga o sensor touch
	
	set_sensor_color(SENSOR_COR_DIREITA, color);
	Wait(100);
	set_sensor_color(SENSOR_COR_ESQUERDA, color);
}


void get_two_rgb(int & leftR, int & leftG, int & leftB, int & rightR, int & rightG, int & rightB)
{
	// Essa funcao pega os dois valores simultaneamente, da esquerda e da direita
	int left[3], right[3];
	int CORES[3] = {VERMELHO, VERDE, AZUL};
	int LINE[3] = {LCD_LINE3, LCD_LINE4, LCD_LINE5};
	int Q_LEITURA = 9, DELAY = 30;
	float divide = 1.0/Q_LEITURA;
	int i, j;
	PlayTone(1000, 500);
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

	leftR = left[0];
	leftG = left[1];
	leftB = left[2];
	rightR = right[0];
	rightG = right[1];
	rightB = right[2];

	PlayTone(400, 100);
	Wait(200);
	PlayTone(400, 100);
	Wait(200);
	PlayTone(1000, 100);
}
/*
int trata_leitura(int leitura)
{
	int cor;
	if (leitura <= BLACKUP)
	{
		cor = PRETO;
	}
	else if (leitura <= BLUEUP)
	{
		cor = AZUL;
	}
	else if (leitura <= FORAUP)
	{
		cor = FORA;
	}
	else if (leitura <= GREENUP)
	{
		cor = VERDE;
	}
	else if (leitura <= REDUP)
	{
		cor = VERMELHO;
	}
	else
	{
		cor = BRANCO;
	}
	int wait = 200, duration = 100, frequency = 1000;
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
*/



task main()
{
	int corD, corE;
	int leftR, leftG, leftB;
	int rightR, rightG, rightB;
	int CORES[6] = {VERMELHO, VERDE, AZUL, BRANCO, PRETO, FORA};
	int left[6][3], right[6][3];
	int i, j;
	//BTCheck();
	ligar_sensores(VERMELHO);
	for(i = 0; i < 6; i++)
	{
		ClearScreen();
		TextOut(5, LCD_LINE3, "Coloque em cima da cor:");
		if(CORES[i] == VERMELHO){
			TextOut(25, LCD_LINE6, "VERMELHO");
		}else if(CORES[i] == VERDE){
			TextOut(25, LCD_LINE6, "VERDE");
		}else if(CORES[i] == AZUL){
			TextOut(25, LCD_LINE6, "AZUL");
		}else if(CORES[i] == BRANCO){
			TextOut(25, LCD_LINE6, "BRANCO");
		}else if(CORES[i] == PRETO){
			TextOut(25, LCD_LINE6, "PRETO");
		}else{
			TextOut(25, LCD_LINE6, "FORA");
		}
		while(!press){
			Wait(100);
		}while(press){
			Wait(100);
		}
		ClearScreen();
		TextOut(COL1, LCD_LINE1, "LEFT:");
		TextOut(COL3, LCD_LINE1, "RIGHT:");
		while(1)
		{
			get_two_rgb(leftR, leftG, leftB, rightR, rightG, rightB);
			Wait(300);
			for(j = 0; j < 50 && !press; j++)
			{
				PlayTone(300+(1000-300)*j/50, 100);
				Wait(400-5*i);
			}
			if(j < 50)
			{
				while(press){
					Wait(100);
				}
			}
			else
			{
				PlayTone(1000, 500);
			}
			Wait(300);
		}
		left[i][0] = leftR;
		left[i][1] = leftG;
		left[i][2] = leftB;
		right[i][0] = rightR;
		right[i][1] = rightG;
		right[i][2] = rightB;
	}
}