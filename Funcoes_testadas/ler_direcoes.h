
#define PRETO 1
#define AZUL 2
#define VERDE 3
#define AMARELO 4
#define VERMELHO 5
#define BRANCO 6
#define FORA 7

#define ESQUERDA 1
#define DIREITA -1
#define FRENTE 0
#define NADA 2
#define NAO_ESQUERDA 3
#define QUE 4

#define SIZE 3*4 // 3 inteiros
#define NAME "direcoes.dat"

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
   
}

int read_direct(int &d1, int &d2, int &d3)
{
	int p, i, d, handle = 0;
	unsigned int file_size = SIZE;
	p = OpenFileRead(NAME, file_size, handle);
	// Return code handling
	if (p != LDR_SUCCESS)
	{
		d1 = QUE;
		d2 = QUE;
		d3 = QUE;
		alerta(1000);
		return 0;
	}
	for(i = 0; i < 3; i++)
	{
		p = Read(handle, d);
		if (p != LDR_SUCCESS)
		{
			d1 = NADA;
			d2 = NADA;
			d3 = NADA;
			return 0;	
		}
		if(i == 0)
			d1 = d;
		else if(i == 1)
			d2 = d;
		else
			d3 = d;
	}
	return 1;
}



task main()
{
	int CORES[3] = {VERMELHO, VERDE, AZUL};
	int LINES[3] = {LCD_LINE3, LCD_LINE5, LCD_LINE7};
	int direcoes[3];
	int i, p, d1, d2, d3;
	string s;
	ClearScreen();
	p = read_direct(d1, d2, d3);
	direcoes[0] = d1;
	direcoes[1] = d2;
	direcoes[2] = d3;
	TextOut(25, LCD_LINE1, "ARQUIVO");
	TextOut(10, LCD_LINE2, "VERMELHO:");
	TextOut(10, LCD_LINE4, "VERDE:");
	TextOut(10, LCD_LINE6, "AZUL:");
	for(i = 0; i < 3; i++)
	{
		if(direcoes[i] == NADA)
			s = "NADA";
		else if(direcoes[i] == ESQUERDA)
			s = "LEFT";
		else if(direcoes[i] == DIREITA)
			s = "RIGHT";
		else if(direcoes[i] == FRENTE)
			s = "FRENTE";
		else if(direcoes[i] == NAO_ESQUERDA)
			s = "NOT LEFT";
		else if(direcoes[i] == QUE)
			s = "QUE";
		TextOut(25, LINES[i], s);
	}
	while(true){
		Wait(100);
	}
	Stop(true);

}