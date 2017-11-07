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
    if(num_r == 14)
        if(BRANCO_r > 217)
            color_r = BRANCO;
        else
            color_r = OUT;
    else if(num_r == 12)
        if(BRANCO_r > 217)
            color_r = BRANCO;
        else
            color_r = BLACK;
    else if(num_r == 17)
        if(BRANCO_r > 217)
            color_r = BRANCO;
        else if(BRANCO_r < 90)
            color_r = BLACK;
        else
            color_r = OUT;
    else if(num_r == 13)
        color_r = BRANCO;
    else if(num_r == 3 || num_r == 4)
        color_r = VERDE;
    else if(num_r == 5 || num_r == 6)
        color_r = YELLOW;
    else if(num_r == 7)
        if(VERDE_r > 130)
            color_r = YELLOW;
        else
            color_r = VERMELHO;
    else if(num_r > 7 && num_r <= 10)
        color_r = VERMELHO;
    else if(num_r == 2)
        color_r = AZUL;

    if(num_l == 14)
        if(BRANCO_l > 217)
            color_l = BRANCO;
        else
            color_l = OUT;
    else if(num_l == 12)
        if(BRANCO_l > 217)
            color_l = BRANCO;
        else
            color_l = BLACK;
    else if(num_l == 17)
        if(BRANCO_l > 217)
            color_l = BRANCO;
        else if(BRANCO_l < 90)
            color_l = BLACK;
        else
            color_l = OUT;
    else if(num_l == 13)
        color_l = BRANCO;
    else if(num_l == 3 || num_l == 4)
        color_l = VERDE;
    else if(num_l == 5 || num_l == 6)
        color_l = YELLOW;
    else if(num_l == 7)
        if(VERDE_l > 130)
            color_l = YELLOW;
        else
            color_l = VERMELHO;
    else if(num_l > 7 && num_l <= 10)
        color_l = VERMELHO;
    else if(num_l == 2)
        color_l = AZUL;
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
