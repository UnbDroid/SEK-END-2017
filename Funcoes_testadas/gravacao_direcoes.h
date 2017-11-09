
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

int open_for_write(int &handle)
{
	unsigned int file_size = SIZE;
	handle = 0;
	// Start with the assumptions the file doesn't exist and needs to be created.
	unsigned int rtn_code = CreateFile(NAME, file_size, handle);
	// If the file already exists, open it with the intent of adding to the data
	// that is already there.
	if (rtn_code == LDR_FILEEXISTS)
	{
		rtn_code = DeleteFile(NAME);
		if(rtn_code == LDR_SUCCESS)
		{
			rtn_code = CreateFile(NAME, file_size, handle);
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
			return 1;
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

int write_direcoes(int &handle, int direcoes[]) // Retorna 1 caso consiga gravar
{
	byte CORES[i] = {VERMELHO, VERDE, AZUL};
	unsigned int p; //Problem
	int i;

	for(i = 0; i < 3; i++) // Pois sao 3 cores para gravar as direcoes
	{
		p = Write(handle, direcoes[CORES[i]]);
		if (p != LDR_SUCCESS)
		{
			return 0;
		}
	}
	return 1;
}



int open_for_read(int &handle)
{
   unsigned int file_size = SIZE;
   handle = 0;
   unsigned int rtn_code = OpenFileRead(NAME, file_size, handle);
   // Return code handling
   if (rtn_code != LDR_SUCCESS)
   {
      return 0;
   }
   return 1;
}

int read_direcoes(int &handle, int &d1, int &d2, int &d3)
{
	int i;
	int d;
	unsigned int p;
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
		else if(i == 2)
			d3 = d;
	}
	return 1;
}

task main()
{
	byte CORES[3] = {VERMELHO, VERDE, AZUL};
	int direcoes[6] = {NADA, NADA, NADA, NADA, NADA, NADA};
	int d1, d2, d3;
	int handle = 0;
	int r;
	// Comeca a leitura das direcoes
	r = open_for_read(handle);
	if(r == 1) // Conseguiu abrir o arquivo
	{
		r = read_direcoes(handle, d1, d2, d3);
		direcoes[CORES[0]] = d1;
		direcoes[CORES[1]] = d2;
		direcoes[CORES[2]] = d3;
	}
	// Fim para a leitura das direcoes
	// Caso nao consiga abrir o arquivo, ou ler corretamente, ele dará o erro e sera considerado
	// como se nao existisse.
	// Mesmo se tiver descoberto apenas a primeira cor, sera armazenado as outras duas como NADA.
	// As tentativas sao: esquerda, direita, reto.
	// Se o robô estiver em uma tentativa e achar um preto, ele será armazenado também e será gravado no arquivo
	// Se por exemplo a primeira cor for vermelha, e achar preto, entao:
	// direcoes[VERMELHO] = 3.
	// Onde 3 significa que nao eh esquerda. Apos descobrir o 3, entao sera armazenado no arquivo.
	// Apos isso, ele fara novamente uma tentativa

}