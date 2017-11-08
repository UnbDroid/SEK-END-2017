#define SENSOR_US IN_1

#define SENSIBILIDADE 0.9

#define TEM_BONECO 1
#define NAO_TEM_BONECO 0

float ultrassom_filtrado_ponderado(int sensor) //testada
{
	float valor = SensorUS(sensor);
	float aux;
	for (int i = 0; i < 5; ++i)
	{
		aux = SensorUS(sensor);

		if (aux < 19)
		{
			valor = (valor * SENSIBILIDADE * 0.3) + (aux * (1-SENSIBILIDADE) * 0.7); //ponderando para medidas proximas aparecerem mais rapido
		}

		//else if (aux > 100)
		//{
		//	valor = (valor * SENSIBILIDADE * 0.7) + (aux * (1-SENSIBILIDADE) * 0.3);
		//}

		else
		{
			
			valor = valor * SENSIBILIDADE + aux * (1-SENSIBILIDADE); // Algoritimo passado pelo B.Andreguetti da aula de SisMed

		}
	}
	return valor;
}

float ultrassom_filtrado(int sensor) //testada
{
	float valor = SensorUS(sensor);
	float aux;
	for (int i = 0; i < 5; ++i)
	{
		aux = SensorUS(sensor);
		valor = valor * SENSIBILIDADE + aux * (1-SENSIBILIDADE); // Algoritimo passado pelo B.Andreguetti da aula de SisMed
	}
	return valor;
}

int verifica_bonecos(int sensor)
{
	float distancia;

	distancia = ultrassom_filtrado_ponderado(sensor);
	if (distancia <= 15)
	{
		return TEM_BONECO;
	}
	else if (distancia > 15 && distancia <= 17)
	{
		distancia = ultrassom_filtrado(sensor);
		if (distancia > 15 && distancia <= 17)
		{
			return TEM_BONECO;
		}
	}
	else
	{
		return NAO_TEM_BONECO;
	}
}

task main()
{
	float distancia1;
	float distancia2;
	SetSensorUltrasonic(SENSOR_US);

	while(true)
	{
		distancia1 = ultrassom_filtrado(SENSOR_US);
		distancia2 = ultrassom_filtrado_ponderado(SENSOR_US);
		ClearLine(LCD_LINE1);
		TextOut(0, LCD_LINE1, "DIST_N:");
		NumOut(55, LCD_LINE1, distancia1);
		ClearLine(LCD_LINE2);
		TextOut(0, LCD_LINE2, "DIST_P:");
		NumOut(55, LCD_LINE2, distancia2);
		if (verifica_bonecos(SENSOR_US) == TEM_BONECO)
		{
			TextOut(0, LCD_LINE5, "ACHEI !!!");
		}
		Wait(100);
	}
}