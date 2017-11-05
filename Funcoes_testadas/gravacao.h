#define PRETO 1
#define AZUL 2
#define VERDE 3
#define AMARELO 4
#define VERMELHO 5
#define BRANCO 6
#define FORA 7

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

int open_for_write(int &handle, int SIZE, char NAME[])
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

int write_colors(int &handle, type_COR left[], type_COR right[]) // Retorna 1 caso consiga gravar
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

int write_direcoes(int &handle, byte direcoes[]) // Retorna 1 caso consiga gravar
{
	byte CORES[3] = [VERMELHO, VERDE, AZUL];
	unsigned int p; //Problem
	int i;

	for(i = 0; i < 3; i++) // Pois sao 3 direcoes apenas
	{
		p = Write(handle, direcoes[CORES[i]]);
		// Return code handling
		if (p != LDR_SUCCESS)
			return 0;
	}
	return 1;
}

int open_for_read(int &handle, int SIZE, char NAME[])
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

int read_direct(int &handle, byte &direct)
{

	unsigned int p;
	p = Read(handle, direct);
	// rtn_code_out(rtn_code);
	// Return code handling
	if (p == LDR_SUCCESS)
		return 1;
	return 0;
}