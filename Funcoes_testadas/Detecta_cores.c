/*********************************************************/
/*                                                       */
/*   Esse codigo foi feito pela equipe DROID-SEK-SABB    */
/*                                                       */
/*********************************************************/


#define R_COLOR IN_2
#define L_COLOR IN_3

#define AZUL     2
#define FORA     7
#define BRANCO   6
#define PRETO    1
#define VERDE    3
#define AMARELO  4
#define VERMELHO 5

byte VERMELHO_r, VERDE_r, AZUL_r, BRANCO_r, num_r, VERMELHO_l, VERDE_l, AZUL_l, BRANCO_l, num_l, color_r, color_l;

// Função que tabela a saída dos sensores de luz para as cores lidas
sub detect_colors(){ // Tabela a saída dos sensores de luz para as cores lidas
    ReadSensorHTColor2Active(R_COLOR, num_r, VERMELHO_r, VERDE_r, AZUL_r, BRANCO_r);
    ReadSensorHTColor2Active(L_COLOR, num_l, VERMELHO_l, VERDE_l, AZUL_l, BRANCO_l);

    /*Achei melhor if em vez de switch pois os valores lidos dependem da calibração*/
if (num_l >= 1 && num_l <= 4)
{
	if (AZUL_l > 150){
		color_l = AZUL;
	} 
	else{
		color_l = VERDE;
	} 	
}  
else if (num_l >= 5 && num_l <= 8)
{
	if (VERDE_l > 140){
		color_l = AMARELO;
	}
	else{
		color_l = VERMELHO;
	} 
}
else if (num_l >= 9 && num_l <= 12 )
{
	color_l = VERMELHO;
}
else if (num_l >= 13 && num_l <= 15)
{
	color_l = BRANCO;
}
else if (num_l == 17)
{
	if (BRANCO_l > 110){
		color_l = PRETO;
	}
	else{
		color_l = FORA;
	} 		
} 
else
{
	color_l = FORA;
}

if (num_r >= 1 && num_r <= 4)
{
	if (AZUL_r > 120){
		color_r = AZUL;
	} 
	else{
		color_r = VERDE;
	}
}
else if (num_r >= 5 && num_r <= 8)
{
	if (VERDE_r > 110){
		color_r = AMARELO;
	}
	else{
		color_r = VERMELHO;
	}
}
else if (num_r >= 9 && num_r <= 12 )
{
	color_r = VERMELHO;
}
else if (num_r >= 13 && num_r <= 15)
{
	color_r = BRANCO;
}
else if (num_r == 17)
{
	if (BRANCO_r > 60){ //SE DER MERDA E AQUI, calibrar melhor
		color_r = PRETO;
	}
	else{
		color_r = FORA;
    }
}
else{
	color_r = FORA;
}


}

task main(){
    SetSensorLowspeed(L_COLOR);
    SetSensorLowspeed(R_COLOR);
    SetHTColor2Mode(L_COLOR, HT_CMD_COLOR2_ACTIVE);
    SetHTColor2Mode(R_COLOR, HT_CMD_COLOR2_ACTIVE);

    while(1){
        detect_colors();
        ClearLine(LCD_LINE1);
        TextOut(0, LCD_LINE1, "COLOR_R:");
        NumOut(50, LCD_LINE1, color_r);
        ClearLine(LCD_LINE3);
        TextOut(0, LCD_LINE3, "COLOR_L:");
        NumOut(50, LCD_LINE3, color_l);
    }
}
