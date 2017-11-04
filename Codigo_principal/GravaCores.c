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


// Utilizado para os arquivos
int handle = 0;

#define FILE_NAME "cores.dat"
#define VALUE_SIZE 3*4 // Pois sao 3 valores inteiros(4 bytes): min, med e max
#define COR_SIZE 3*VALUE_SIZE // Pois sao 3 cores: red, green, blue
#define QUANT_CORES 6 // Pois existe: red, green, blue, white, black and out
#define FILE_SIZE 2*QUANT_CORES*COR_SIZE // Esquerda e direita

struct type_value
{
	int min, med, max;
};

struct type_COR
{
   type_value r, g, b; // red, green, blue
};

type_value init_value(int minimo, int media, int maximo)
{
	type_value color;
	color.min = minimo;
	color.med = media;
	color.max = maximo;
	return color;
}

type_COR init_COR(type_value red, type_value green, type_value blue)
{
	type_COR color;
	color.r = red;
	color.g = green;
	color.b = blue;
	return color;
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

void shutdown(const int delay)
{
   if (handle) CloseFile(handle);
   // Get user's attention.
   // Give the user time to read screen messages.
   Wait(delay);
   Stop(true);
}

void open_for_write()
{
	unsigned int file_size = FILE_SIZE;
	handle = 0;
	// Start with the assumptions the file doesn't exist and needs to be created.
	unsigned int rtn_code = CreateFile(FILE_NAME, file_size, handle);
	// If the file already exists, open it with the intent of adding to the data
	// that is already there.
	if (rtn_code == LDR_FILEEXISTS)
	{
		rtn_code = DeleteFile(FILE_NAME);
		if(rtn_code == LDR_SUCCESS)
		{
			rtn_code = CreateFile(FILE_NAME, file_size, handle);
		}
		else
		{
			// Deu algum erro, acho que nunca vai entrar aqui
		}
	}
	// Return code handling
	switch (rtn_code)
	{
		case LDR_SUCCESS:
			return;
		case LDR_FILEISFULL:
			// Acho que nunca vai entrar nesse caso, pois o arquivo sempre é criado novamente
			TextOut(0, LCD_LINE1, "file is full    ");
			break;
		default:
			// Unanticipated exception.
			TextOut(0, LCD_LINE1, "write open      ");
			break;
	}
	alerta(800);
	shutdown(SEC_1);
}

int write_colors(type_COR left[], type_COR right[]) // Retorna 1 caso consiga gravar
{
	type_COR aux1, aux2;
	unsigned int p[2]; //Problem
	int i;

	for(i = 0; i < 6; i++) // Pois sao 6 cores
	{
		aux1 = left[i];
		aux2 = right[i];
		p[0] = Write(handle, aux1);
		p[1] = Write(handle, aux2);
		// Return code handling
		if (p[0] != LDR_SUCCESS || p[1] != LDR_SUCCESS)
			return 0;
	}
	return 1;
}


// Captura de cores 
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


void get_two_rgb(type_COR & left, type_COR & right)
{
	// Essa funcao pega os dois valores simultaneamente, da esquerda e da direita
	type_value l[3], r[3];
	int CORES[3] = {VERMELHO, VERDE, AZUL};
	int LINE[3] = {LCD_LINE5, LCD_LINE6, LCD_LINE7};
	int Q_LEITURA = 9, DELAY = 30, temp;
	float divide = 1.0/Q_LEITURA;
	int i, j;
	PlayTone(1000, 500);
	for(i = 0; i < 3; i++)
	{
		set_sensor_color(SENSOR_COR_ESQUERDA, CORES[i]);
		Wait(20);
		set_sensor_color(SENSOR_COR_DIREITA, CORES[i]);

		temp = get_value_color(SENSOR_COR_ESQUERDA);
		l[i] = init_value(temp, temp, temp);
		temp = get_value_color(SENSOR_COR_DIREITA);
		r[i] = init_value(temp, temp, temp);
		Wait(DELAY);
		
		for(j = 1; j < Q_LEITURA; j++)
		{
			temp = get_value_color(SENSOR_COR_ESQUERDA);
			l[i].med += temp;
			if(l[i].min > temp)
				l[i].min = temp;
			else if(l[i].max < temp)
				l[i].max = temp;

			temp = get_value_color(SENSOR_COR_DIREITA);
			r[i].med += temp;
			if(r[i].min > temp)
				r[i].min = temp;
			else if(r[i].max < temp)
				r[i].max = temp;		

			Wait(DELAY);
		}

		l[i].med *= divide;
		r[i].med *= divide;
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
		NumOut( COL2, LINE[i], l[i].med);
		NumOut( COL4, LINE[i], r[i].med);
	}

	left  = init_COR(l[0], l[1], l[2]);
	right = init_COR(r[0], r[1], r[2]);


	PlayTone(400, 100);
	Wait(200);
	PlayTone(400, 100);
	Wait(200);
	PlayTone(1000, 100);
}

void imprime_cor(int x, int y, int color)
{
	if(color == VERMELHO){
		TextOut(x, y, "VERMELHO");
	}else if(color == VERDE){
		TextOut(x, y, "VERDE");
	}else if(color == AZUL){
		TextOut(x, y, "AZUL");
	}else if(color == BRANCO){
		TextOut(x, y, "BRANCO");
	}else if(color == PRETO){
		TextOut(x, y, "PRETO");
	}else{
		TextOut(x, y, "FORA");
	}
}

task main()
{
	int CORES[6] = {VERMELHO, VERDE, AZUL, BRANCO, PRETO, FORA};
	type_COR left[6], right[6], aux1, aux2;
	int i, j, p;
	//BTCheck();
	ligar_sensores(VERMELHO);
	for(i = 0; i < 6; i++)
	{
		ClearScreen();
		TextOut(5, LCD_LINE3, "Coloque em cima da cor:");
		imprime_cor(25, LCD_LINE6, CORES[i]);
		while(!press){
			Wait(100);
		}while(press){
			Wait(100);
		}
		ClearScreen();
		imprime_cor(35, LCD_LINE1, CORES[i]);
		TextOut(COL1, LCD_LINE3, "LEFT:");
		TextOut(COL3, LCD_LINE3, "RIGHT:");
		while(1)
		{
			get_two_rgb(aux1, aux2);
			left[i] = aux1;
			right[i] = aux2;
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
				break;
			}
			else
			{
				PlayTone(1000, 500);
			}
			Wait(300);
		}
	}
	open_for_write();
	p = write_colors(left, right);
	if(!p)
	{
		alerta(400);
		p = DeleteFile(FILE_NAME);
	}
	shutdown(1000);
}