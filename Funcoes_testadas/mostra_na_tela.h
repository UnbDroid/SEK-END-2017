/* Funcoes passadas pelo Takashi */
// ESTADOS DE CALIBRAÇÃO.
sub ShowRGB(int l_red, int l_green, int l_blue, int r_red, int r_green, int r_blue)
{
     ClearScreen();
     TextOut(40, LCD_LINE2, "LEFT");
     TextOut(70, LCD_LINE2, "RIGHT");
     TextOut(0, LCD_LINE4, "RED:");
     TextOut(0, LCD_LINE6, "GREEN:");
     TextOut(0, LCD_LINE8, "BLUE:");
     NumOut(40, LCD_LINE4, l_red);
     NumOut(40, LCD_LINE6, l_green);
     NumOut(40, LCD_LINE8, l_blue);
     NumOut(70, LCD_LINE4, r_red);
     NumOut(70, LCD_LINE6, r_green);
     NumOut(70, LCD_LINE8, r_blue);
     Wait(100);
}

sub ShowColor(int l_color, int r_color)
{
     ClearScreen();
     TextOut(40, LCD_LINE4, "LEFT");
     TextOut(70, LCD_LINE4, "RIGHT");
     TextOut(0, LCD_LINE6, "COLOR:");
     NumOut(40, LCD_LINE6, l_color);
     NumOut(70, LCD_LINE6, r_color);
     Wait(100);
}

sub ShowUS(int l_us, int r_us)
{
     ClearScreen();
     TextOut(40, LCD_LINE4, "LEFT");
     TextOut(70, LCD_LINE4, "RIGHT");
     TextOut(0, LCD_LINE6, "US:");
     NumOut(40, LCD_LINE6, l_us);
     NumOut(70, LCD_LINE6, r_us);
     Wait(100);
}

sub ShowEncoder(int garra_encoder)
{
     ClearScreen();
     TextOut(40, LCD_LINE4, "ENCODER:");
     NumOut(70, LCD_LINE6, garra_encoder);
     Wait(100);
}

