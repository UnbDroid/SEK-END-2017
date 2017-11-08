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

		else
		{
			
			valor = valor * SENSIBILIDADE + aux * (1-SENSIBILIDADE); // Algoritimo passado pelo B.Andreguetti da aula de SisMed

		}
	}
	return valor;
}

int verifica_boneco(int sensor)
{
	float distancia;

	distancia = ultrassom_filtrado_ponderado(sensor);

	if (distancia < 17)
	{
		return TEM_BONECO;
	}
	else
	{
		return NAO_TEM_BONECO;
	}
}

task main()
{

	int verificar = NAO_TEM_BONECO;
	SetSensorUltrasonic(SENSOR_US);

	while(true)
	{
		NumOut(48, LCD_LINE1, verificar);

		verificar = verifica_boneco(SENSOR_US);

		if (verificar == TEM_BONECO)
		{
			TextOut(0, LCD_LINE5, "ACHEI !!!");
		}
	}
}