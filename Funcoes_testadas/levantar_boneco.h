#define MOTOR_GARRA OUT_B
#define MOTOR_DIREITO OUT_A
#define MOTOR_ESQUERDO OUT_C
#define AMBOS_MOTORES OUT_AC

#define VELOCIDADE_BAIXA 45

void levantar_garra()
{
	int prev_motor = MotorRotationCount(MOTOR_GARRA);
	OnRev(MOTOR_GARRA, VELOCIDADE_BAIXA);
	Wait(50);
	while(prev_motor != MotorRotationCount(MOTOR_GARRA)) // a garra irá se movimentar até travar na estrutura
	{
		prev_motor = MotorRotationCount(MOTOR_GARRA);
		Wait(50);
	}
	Off(MOTOR_GARRA);
}