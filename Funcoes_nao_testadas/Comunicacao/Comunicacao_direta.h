/* Falta testar esse codigo ainda
 */
#define BT_CONN 1 // Numero da porta BlueTooth

#define SENSOR_COR_ESQUERDA IN_1
#define SENSOR_COR_DIREITA IN_2

#define PRETO 1
#define AZUL 2
#define VERDE 3
#define AMARELO 4
#define VERMELHO 5
#define BRANCO 6


/*
#define WHITEUP 630
#define WHITEDOWN 575
#define BLUEUP 340
#define	BLUEDOWN 300
#define	BLACKUP 220
#define BLACKDOWN 160
#define	REDUP 575
#define REDDOWN 545
#define GREENUP 330
#define GREENDOWN 440
*/


sub BTCheck(int conn){
	if (!BluetoothStatus(conn)==NO_ERR){
		ClearScreen();
		TextOut(5,LCD_LINE2,"Erro");
		Wait(1000);	
		Stop(true);
	 }
}
/* Deu errado, colocar cada linha separadamente no codigo
void set_sensor_color(char porta, char color)
{
	if(color == VERMELHO){
		RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORRED, SENSOR_MODE_RAW);
	}else if(color == VERDE){
		RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORGREEN, SENSOR_MODE_RAW);
	}else if(color == AZUL){
		RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORBLUE, SENSOR_MODE_RAW);
	}else if(color == BRANCO){
		RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORFULL, SENSOR_MODE_RAW);
	}
}*/


int get_value_color(char porta)
{
	InputValuesType params;
	
	params.Port = porta;	
	RemoteGetInputValues(BT_CONN, params);
	
	return params.RawValue;	
}

/* 

Exemplo de uso na main

task main()
{
	int leitura;	
	int i, j;
	char cores[4] = {VERMELHO, VERDE, AZUL, BRANCO};
	char color, porta;
	BTCheck(BT_CONN);
	
	while(true)
	{
		//RemoteKeepAlive(1);
		for(i = 0; i < 4; i++) // Para trocar de cor 
		{
			//set_sensor_color(SENSOR_COR_ESQUERDA, cores[i]);
			color = cores[i];
			porta = SENSOR_COR_ESQUERDA;
			// Seria a funcao, mas a funcao nao da certo 
			if(color == VERMELHO){
				RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORRED, SENSOR_MODE_RAW);
			}else if(color == VERDE){
				RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORGREEN, SENSOR_MODE_RAW);
			}else if(color == AZUL){
				RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORBLUE, SENSOR_MODE_RAW);
			}else if(color == BRANCO){
				RemoteSetInputMode(BT_CONN, porta, SENSOR_TYPE_COLORFULL, SENSOR_MODE_RAW);
			}
			// Aqui encerra a funcao 

			for(j = 0; j < 20; j++) // Para manter os valores atualizados
			{
				leitura = get_value_color(SENSOR_COR_ESQUERDA);
				ClearLine(LCD_LINE1);
				NumOut(0, LCD_LINE1, leitura);
				if(leitura > 300)
				{
					RemoteSetOutputState(CONN_BT1, OUT_B, 60, OUT_MODE_MOTORON, OUT_REGMODE_IDLE, 0, OUT_RUNSTATE_RUNNING, 0);
				} else {
					RemoteSetOutputState(CONN_BT1, OUT_B, -60, OUT_MODE_MOTORON, OUT_REGMODE_IDLE, 0, OUT_RUNSTATE_RUNNING, 0);
				}		
				Wait(100);
			}
		}
	}

*/