
int write_direcoes(int direcoes[]) // Retorna 1 caso consiga gravar
{
	byte CORES[3] = {VERMELHO, VERDE, AZUL};
	int i, d, p, handle = 0;
	unsigned int file_size = SIZE;
	// Start with the assumptions the file doesn't exist and needs to be created.
	p = CreateFile(NAME, file_size, handle);
	// If the file already exists, open it with the intent of adding to the data
	// that is already there.
	if (p == LDR_FILEEXISTS)
	{
		p = DeleteFile(NAME);
		if(p == LDR_SUCCESS)
		{
			p = CreateFile(NAME, file_size, handle);
		}
		else
		{
			// Deu algum erro, acho que nunca vai entrar aqui
			return 0;
		}
	}
	// Return code handling
	if(p != LDR_SUCCESS)
	{
		alerta(800);
		return 0;
	}
	// Se chegou aqui, conseguiu abrir o novo arquivo


	for(i = 0; i < 3; i++) // Pois sao 3 cores para gravar as direcoes
	{
		d = direcoes[CORES[i]];
		p = Write(handle, d);
		if (p != LDR_SUCCESS)
			break;
	}
	if (handle)
		CloseFile(handle);
	if(p == LDR_SUCCESS)
		return 1;
	return 0;
}

int read_direcoes(int &d1, int &d2, int &d3)
{
	int handle = 0;
	unsigned int file_size = SIZE;
	int p, i, d;
	
	p = OpenFileRead(NAME, file_size, handle);
	// Return code handling
	if (p != LDR_SUCCESS)
	{
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
		else if(i == 2)
			d3 = d;
	}
	return 1;
}