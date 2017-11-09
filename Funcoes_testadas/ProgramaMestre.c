//MASTER
#define CONEXAO 1
#define INBOX 1
#define OUTBOX 5
sub BTCheck(int conn){
	if (!BluetoothStatus(conn)==NO_ERR){
		TextOut(5,LCD_LINE2,"Error");
		Wait(1000);
		Stop(true);
	}
}

int achou_boneco()
{
	// Retorna 0 caso nao tenha achado nenhum boneco nas laterais
	// Retorna 1 caso ache o boneco pela esquerda
	// Retorna 2 caso ache o boneco pela direita
	// Retorna 3 caso ache o boneco nos dois lados
	int found, num = 0;
    found = -2; /* Significa que nao existe entrada, esse valor porque eu quis */
	SendRemoteNumber(CONEXAO, OUTBOX, 0xFF); // Esse valor porque eu vi na internet
	until(found != -2)
	{
		until(ReceiveRemoteNumber(INBOX,true,found) == NO_ERR); // Deve retornar entre 0 e 3
		num += 1;
		NumOut(25, LCD_LINE2, num);
	}
	NumOut(25, LCD_LINE2, found);

	return found;
}

task main(){
	
	int found;
	BTCheck(CONEXAO); //check slave connection
	while(true){
		TextOut(10,LCD_LINE1,"Acha bonecos");
		found = achou_boneco();
		TextOut(0, LCD_LINE4, "Left:");
		TextOut(0, LCD_LINE6, "Right:");
		if(found%2 == 1)
			TextOut(10, LCD_LINE5, "SIM!");
		else
			TextOut(10, LCD_LINE5, "NAO!");
		if(found/2 == 1)
			TextOut(10, LCD_LINE7, "SIM!");
		else
			TextOut(10, LCD_LINE7, "NAO!");
		
		Wait(30);
	}
}
