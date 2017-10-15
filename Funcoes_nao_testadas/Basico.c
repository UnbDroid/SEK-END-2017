
#define PRETO 1
#define AZUL 2
#define VERDE 3
#define AMARELO 4
#define VERMELHO 5
#define BRANCO 6

#define RETO 0
#define ESQUERDA 1
#define DIREITA 2
#define NADA 3


task main()
{
	int direcao[6] = {NADA, NADA, NADA, NADA, NADA, NADA};
	int chute = 0, cor_achada;
	inicia_sensores();
	if(existe_arquivo_info())
	{
		/* Codigo para pegar do arquivo e gravar nas direcoes */
	}
	liga_motores();
	while(1)
	{
		cor_left	= cor_esquerda();
		cor_right	= cor_direita();
		us_left		= US_esquerda();
		us_right	= US_direita();
		if(US_achou_algo(us_left, us_right) && passageiro < 4)
		{
			/* Aqui e necessario colocar um tratamento de caso para a cor, para ele nao cair quando tentar girar */
			para();
			gira(decide_direcao_US(us_left, us_right));
			pega_boneco();
			re();
			gira(sentido_contrario(us_left, us_right));
		}
		else if(cor_left != BRANCO || cor_right != BRANCO) /* Entra aqui caso nao esteja no caminho certo ou ache alguma esquina */
		{
			if(cor_left == FORA_PISTA)
			{
				consertar_rota_esquerda();
			}
			else if(cor_right == FORA_PISTA)
			{
				consertar_rota_direita();
			}
			else
			{
				/* Aqui algum dos sensores achou vermelho, azul ou verde */
				if(cor_left != BRANCO)
				{
					cor_achada = cor_left;
				}
				else /* Que e o mesmo que else if(cor_right != BRANCO)*/
				{
					cor_achada = cor_right;
				}
				if(chute == 1 && cor_achada != PRETO && cor_achada != BRANCO) /* Estava no chute e achou uma esquina */
				{
					/* Ele acertou o chute*/
				}
				else if(chute == 1 && cor_achada == PRETO)
				{
					/* O chute foi errado */
				}
				if(direcao[cor_achada] != NADA) /* Se ainda ja tem gravado a direcao de uma determinada cor */
				{
					gira(direcao[cor_achada]) /* Ele vai girar para a cor ja descoberta*/
					sair_da_cor();
				}
				else /* Se nao, ele vai tentar descobrir a cor atraves de chutes */
				{
					if(cor_achada == VERMELHO && (direcao[AZUL] == NADA || direcao[VERDE] == NADA) )
					{
						chute = 1; /* Indica que vai chutar */
					}
					else if(cor_left == VERDE && (direcao[AZUL] == NADA || direcao[VERMELHO] == NADA) )
					{
						chute = 1; /* Indica que vai chutar */
					}
					else if(cor_left == AZUL && (direcao[VERDE] == NADA || direcao[VERMELHO] == NADA) )
					{
						chute = 1; /* Indica que vai chutar */
					}
				}
			}
		}
		Wait(100);
	}
}