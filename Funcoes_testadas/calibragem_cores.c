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

#define WHITEDOWN_R 590
#define REDDOWN_R 550
#define BLACKUP_R 270
#define BLUEDOWN_R 220
#define BLUEUP_R 310
#define FORAUP_R 450
#define FORADOWN_R 350

#define WHITEDOWN 579
#define WHITEUP 595
#define BLACKUP 259
#define BLACKDOWN 221
#define REDUP 419
#define REDDOWN 416
#define GREENUP 378
#define GREENDOWN 351
#define BLUEUP 276
#define BLUEDOWN 273
#define FORAUP 360
#define FORADOWN 304
#define DESVIO 20
#define OFFSET_COLOR 1/9.0

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

/*int sensor_cor(int sensor)
{
	int leitura_r = 0;

	for(int i = 0; i < 1/OFFSET_COLOR; i++)
	{
		leitura += get_value_color(sensor)*OFFSET_COLOR;
	}

	if (sensor == SENSOR_COR_ESQUERDA)
		NumOut(0,0, leitura);
	if(sensor == SENSOR_COR_DIREITA)
		NumOut(0,40, leitura);

	if(leitura >= WHITEDOWN_R){
		return BRANCO;
	}
	else if (leitura >= REDDOWN_R){
		return VERMELHO;
	} else if (leitura <= BLACKUP_R){
		return PRETO;
	} else if (leitura >= BLUEDOWN_R && leitura <= BLUEUP_R){
		return AZUL;
	} else 	if (leitura <= FORAUP_R && leitura >= FORADOWN_R){
		return FORA;
	}
	return FORA;
}*/

int teste_cor(int sensor)
{
	int leitura_r = 0, leitura_g = 0, leitura_b = 0, leitura = 0;
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

	leitura = (4*leitura_r + 2*leitura_g + leitura_b)/7;

	//comentar quando for ver parametros
	if (leitura <= BLACKUP)
		return PRETO;
	if (leitura <= BLUEUP)
		return AZUL;
	if (leitura <= FORAUP)
		return FORA;
	if (leitura <= GREENUP)
		return VERDE;
	if (leitura <= REDUP)
		return VERMELHO;

	return BRANCO;
	//return leitura; /*"descomentar" quando for ver parametros*/
}

task main()
{
	ligar_sensores();
	int valorD, valorE, leit;
	while(1)
	{
		valorD = teste_cor(SENSOR_COR_DIREITA);
		valorE = teste_cor(SENSOR_COR_ESQUERDA);
		ClearLine(LCD_LINE3);
		/*ClearLine(LCD_LINE1);
		ClearLine(LCD_LINE3);
		TextOut(0, LCD_LINE1, "Direita:");
		NumOut(50, LCD_LINE1, valorD);
		TextOut(0, LCD_LINE3, "Esquerda::");
		NumOut(50, LCD_LINE3, valorE);*/
		if(valorD == VERMELHO || valorE == VERMELHO) TextOut(0, LCD_LINE3, "Vermelho");
		else if(valorD == VERDE || valorE == VERDE) TextOut(0, LCD_LINE3, "Verde");
		else if(valorD == AZUL || valorE == AZUL) TextOut(0, LCD_LINE3, "Azul");
		else if(valorD == BRANCO || valorE == BRANCO) TextOut(0, LCD_LINE3, "Branco");
		else if(valorD == PRETO || valorE == PRETO){
			TextOut(0, LCD_LINE3, "Preto");
			PlayTone(400, 100);
		}
		else{
			TextOut(0, LCD_LINE3, "Fora");
			PlayTone(200, 100);
		}
		Wait(100);

	}
}