#define FILE_NAME "cores.dat"
#define RECORDS 2*6 // Pois sao 6 intervalos de cores para cada sensor
#define RECORD_SIZE 12 // Pois sao 3 inteiros(4 bytes)
#define FILE_SIZE (RECORD_SIZE * RECORDS)

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


struct type_value
{
	int min, med, max;
};

struct type_cor
{
   type_value r, g, b, t; // red, green, blue
};

struct type_lado
{
	type_cor red, green, blue;
	type_cor white, black, out;
};

type_value init_value(int minimo, int media, int maximo)
{
	type_value color;
	color.min = minimo;
	color.med = media;
	color.max = maximo;
	return color;
}

type_cor init_cor(type_value red, type_value green, type_value blue)
{
	type_cor color;
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

void shutdown(int &handle, const int delay)
{
   if (handle) CloseFile(handle);
   // Get user's attention.
   // Give the user time to read screen messages.
   Wait(delay);
   Stop(true);
}

int open_for_read(int &handle)
{
   unsigned int file_size = FILE_SIZE;
   handle = 0;
   unsigned int rtn_code = OpenFileRead(FILE_NAME, file_size, handle);
   // Return code handling
   if (rtn_code != LDR_SUCCESS)
   {
      return 0;
   }
   return 1;
}

int read_cor(int &handle, type_cor &color)
{
	unsigned int p;
	type_value temp;
	p = Read(handle, temp);
	if (p != LDR_SUCCESS)
		return 0;
	color.r = temp;
	p = Read(handle, temp);
	if (p != LDR_SUCCESS)
		return 0;
	color.g = temp;
	p = Read(handle, temp);
	if (p != LDR_SUCCESS)
		return 0;
	color.b = temp;
	return 1;
}

void imprime_texto_cor(int x, int y, int color)
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

void imprime_value(int LINE, type_value color)
{
	NumOut( 10, LINE, color.min);
	NumOut( 30, LINE, color.med);
	NumOut( 50, LINE, color.max);
}

int le_lado(type_lado &lado)
{
	byte CORES[6] = {VERMELHO, VERDE, AZUL, BRANCO, PRETO, FORA};
	int i, p;
	for(i = 0; i < 6; i++)
	{
		if(CORES[i] == VERMELHO)
			p = read_cor(lado.red);
		else if(CORES[i] == VERDE)
			p = read_cor(lado.green);
		else if(CORES[i] == AZUL)
			p = read_cor(lado.blue);
		else if(CORES[i] == BRANCO)
			p = read_cor(lado.white);
		else if(CORES[i] == PRETO)
			p = read_cor(lado.black);
		else if(CORES[i] == FORA)
			p = read_cor(lado.out);
		if(p == 0)
			return 0;
		PlayTone(500, 100);
		Wait(500);
	}
	Wait(2000);
	return 1;

}

void calcula_cor(type_cor &color)
{
	const int pesos[3] = {5, 3, 1};
	const int soma = pesos[0]+pesos[1]+pesos[2];
	color.t = init_value((pesos[0]*color.r.min+pesos[1]*color.g.min+pesos[2]*color.b.min)/soma,
						 (pesos[0]*color.r.med+pesos[1]*color.g.med+pesos[2]*color.b.med)/soma,
						 (pesos[0]*color.r.max+pesos[1]*color.g.max+pesos[2]*color.b.max)/soma);
}
void calcula_lado(type_lado &lado)
{
	byte CORES[6] = {VERMELHO, VERDE, AZUL, BRANCO, PRETO, FORA};
	int i;
	for(i = 0; i < 6; i++)
	{
		if(CORES[i] == VERMELHO)
			calcula_cor(lado.red);
		else if(CORES[i] == VERDE)
			calcula_cor(lado.green);
		else if(CORES[i] == AZUL)
			calcula_cor(lado.blue);
		else if(CORES[i] == BRANCO)
			calcula_cor(lado.white);
		else if(CORES[i] == PRETO)
			calcula_cor(lado.black);
		else if(CORES[i] == FORA)
			calcula_cor(lado.out);
	}
}

void imprime_valores_cor(type_cor &valores)
{
	int LINE[4] = {LCD_LINE5, LCD_LINE6, LCD_LINE7, LCD_LINE8};
	int j;
	for(j = 0; j < 4; j++)
	{
		if(j == 0){
			TextOut(COL1, LINE[j], "R");
			imprime_value(LINE[j], valores.r);
		}else if(j == 1){
			TextOut(COL1, LINE[j], "G");
			imprime_value(LINE[j], valores.g);
		}else if(j == 2){
			TextOut(COL1, LINE[j], "B");
			imprime_value(LINE[j], valores.b);
		}else{
			TextOut(COL1, LINE[j], "T");
			imprime_value(LINE[j], valores.t);
		}
	}
}

task main()
{
	int i, j, p;
	type_lado L, R;
	int handle = 0;
	byte CORES[6] = {VERMELHO, VERDE, AZUL, BRANCO, PRETO, FORA};
	// type_lado, type_cor, type_value
	p = open_for_read(handle);
	if(p == 1)
	{
		p = le_lado(handle, L);
		if(p == 0)
		{
			alerta(1000);
			shutdown(SEC_1);
		}
		PlayTone(1000, 500);
		calcula_lado(L);
		p = le_lado(handle, R);
		if(p == 0)
		{
			alerta(1000);
			shutdown(SEC_1);
		}
		calcula_lado(R);
		while(1)
		{
			for(i = 0; i < 12; i++)
			{
				ClearScreen();
				imprime_texto_cor(35, LCD_LINE1, CORES[i/2]);
				if(i%2)
				{
					TextOut(COL1, LCD_LINE1, "RIGHT:");
					if(CORES[i/2] == VERMELHO)
						imprime_valores_cor(L.red);
					else if(CORES[i/2] == VERDE)
						imprime_valores_cor(L.green);
					else if(CORES[i/2] == AZUL)
						imprime_valores_cor(L.blue);
					else if(CORES[i/2] == BRANCO)
						imprime_valores_cor(L.white);
					else if(CORES[i/2] == PRETO)
						imprime_valores_cor(L.black);
					else if(CORES[i/2] == FORA)
						imprime_valores_cor(L.out);
				}
				else
				{
					TextOut(COL1, LCD_LINE1, "LEFT:");
					if(CORES[i/2] == VERMELHO)
						imprime_valores_cor(R.red);
					else if(CORES[i/2] == VERDE)
						imprime_valores_cor(R.green);
					else if(CORES[i/2] == AZUL)
						imprime_valores_cor(R.blue);
					else if(CORES[i/2] == BRANCO)
						imprime_valores_cor(R.white);
					else if(CORES[i/2] == PRETO)
						imprime_valores_cor(R.black);
					else if(CORES[i/2] == FORA)
						imprime_valores_cor(R.out);
				}
				while(!ButtonPressed(BTNCENTER, false)){
					Wait(100);
				}while(ButtonPressed(BTNCENTER, false)){
					Wait(100);
				}
			}
		}
	}
}
