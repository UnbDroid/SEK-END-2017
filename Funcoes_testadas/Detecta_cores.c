/*********************************************************/
/*                                                       */
/*   Esse codigo foi feito pela equipe DROID-SEK-SABB    */
/*                                                       */
/*********************************************************/


#define R_COLOR IN_2

#define AZUL     2
#define FORA     7
#define BRANCO   6
#define PRETO    1
#define VERDE    3
#define AMARELO  4
#define VERMELHO 5

byte VERMELHO_r, VERDE_r, AZUL_r, BRANCO_r, num_r, color_r;

// Função que tabela a saída dos sensores de luz para as cores lidas
int detect_colors(){ // Tabela a saída dos sensores de luz para as cores lidas
    ReadSensorHTColor2Active(R_COLOR, num_r, VERMELHO_r, VERDE_r, AZUL_r, BRANCO_r);

    /*Achei melhor if em vez de switch pois os valores lidos dependem da calibração*/
    if(num_r == 14)
        if(BRANCO_r > 217)
            color_r = BRANCO;
        else
            color_r = FORA;
    else if(num_r == 12)
        if(BRANCO_r > 217)
            color_r = BRANCO;
        else
            color_r = PRETO;
    else if(num_r == 17)
        if(BRANCO_r > 217)
            color_r = BRANCO;
        else if(BRANCO_r < 90)
            color_r = PRETO;
        else
            color_r = FORA;
    else if(num_r == 13)
        color_r = BRANCO;
    else if(num_r == 3 || num_r == 4)
        color_r = VERDE;
    else if(num_r == 5 || num_r == 6)
        color_r = AMARELO;
    else if(num_r == 7)
        if(VERDE_r > 130)
            color_r = AMARELO;
        else
            color_r = VERMELHO;
    else if(num_r > 7 && num_r <= 10)
        color_r = VERMELHO;
    else if(num_r == 2)
        color_r = AZUL;

    return color_r;

}

task main(){
    SetSensorLowspeed(R_COLOR);
    SetHTColor2Mode(R_COLOR, HT_CMD_COLOR2_ACTIVE);

	while(1){
        detect_colors();
        ClearLine(LCD_LINE1);
        TextOut(0, LCD_LINE1, "COLOR_R:");
        NumOut(50, LCD_LINE1, color_r);
    }
}
