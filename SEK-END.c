#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define SENSOR_ESQUERDA IN_1
#define SENSOR_DIREITA IN_2
#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_4
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_ALTA 65
#define PRETO 1
#define VERDE 3
#define BRANCO 6
#define AZUL 2
#define VERMELHO 5
#define AMARELO 4

void abrir_porta (int *porta)
{
	const float RAIO = 1.1, CREMALHEIRA = 4.8, PI = 3.1415;
	float rotacao;

	if (!porta)
	{
		rotacao = (360*CREMALHEIRA)/2*PI*RAIO;
		RotateMotor(MOTOR_ESQUERDA, 60, rotacao);
		*porta = 1
	}
}

task main ()
{
  int porta = 0;

	abrir_porta(&porta);

}
