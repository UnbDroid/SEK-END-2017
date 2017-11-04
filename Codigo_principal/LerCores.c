#define FILE_NAME "cores.dat"
#define RECORDS 2*6 // Pois sao 6 intervalos de cores para cada sensor
#define RECORD_SIZE 12 // Pois sao 3 inteiros(4 bytes) 
#define FILE_SIZE (RECORD_SIZE * RECORDS)

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


int handle = 0;

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

int open_for_read()
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

int read_two_colors(type_COR & left, type_COR & right)
{

	unsigned int p[2];
	p[0] = Read(handle, left);
	p[1] = Read(handle, right);
	// rtn_code_out(rtn_code);
	// Return code handling
	if (p[0] == LDR_SUCCESS && p[1] == LDR_SUCCESS)
		return 1;
	return 0;
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

void imprime_value(int LINE, type_value color)
{
	NumOut( 10, LINE, color.min);
	NumOut( 30, LINE, color.med);
	NumOut( 50, LINE, color.max);
}

task main()
{
	int i, j, p;
	int CORES[6] = {VERMELHO, VERDE, AZUL, BRANCO, PRETO, FORA};
	int LINE[4] = {LCD_LINE5, LCD_LINE6, LCD_LINE7, LCD_LINE8};
	type_COR left[6], right[6], aux1, aux2;
	type_value l[6], r[6];
	SetSensorTouch(TOUCH); // Liga o sensor touch
	p = open_for_read();
	if(p == 1)
	{
		for(i = 0; i < 6; i++)
		{
			p = read_two_colors(aux1, aux2);
			if(p == 0)
			{
				alerta(1000);
				shutdown(SEC_1);
			}
			left[i] = aux1;
			right[i] = aux2;
			l[i] = init_value((4*aux1.r.min+aux1.g.min+2*aux1.b.min)/7,
							  (4*aux1.r.med+aux1.g.med+2*aux1.b.med)/7,
							  (4*aux1.r.max+aux1.g.max+2*aux1.b.max)/7);
			r[i] = init_value((4*aux2.r.min+aux2.g.min+2*aux2.b.min)/7,
							  (4*aux2.r.med+aux2.g.med+2*aux2.b.med)/7,
							  (4*aux2.r.max+aux2.g.max+2*aux2.b.max)/7);
		}
		while(1)
		{
			for(i = 0; i < 12; i++)
			{
				ClearScreen();
				if( i % 2)
					TextOut(COL1, LCD_LINE1, "RIGHT:");
				else
					TextOut(COL1, LCD_LINE1, "LEFT:");
				imprime_cor(35, LCD_LINE1, CORES[i/2]);
				for(j = 0; j < 4; j++)
				{
					if(j == 0){
						TextOut(COL1, LINE[j], "R");
						if( i % 2)
							imprime_value(LINE[j], right[i/2].r);
						else
							imprime_value(LINE[j], left[i/2].r);	
					}else if(j == 1){
						TextOut(COL1, LINE[j], "G");
						if( i % 2)
							imprime_value(LINE[j], right[i/2].g);
						else
							imprime_value(LINE[j], left[i/2].g);
					}else if(j == 2){
						TextOut(COL1, LINE[j], "B");
						if( i % 2)
							imprime_value(LINE[j], right[i/2].b);
						else
							imprime_value(LINE[j], left[i/2].b);
					}else{
						TextOut(COL1, LINE[j], "T");
						if( i % 2)
							imprime_value(LINE[j], l[i/2]);
						else
							imprime_value(LINE[j], r[i/2]);
					}
				}
				while(!press){
					Wait(100);
				}while(press){
					Wait(100);
				}
			}
		}
	}
}