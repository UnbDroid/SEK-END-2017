#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define MOTORES OUT_AC
#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B /*conexão com o outro cérebro*/
#define SENSOR_COR_ESQUERDA
#define SENSOR_COR_DIREITA
#define SENSOR_US_ESQUERDA IN_1
#define SENSOR_US_DIREITA IN_3
#define SENSOR_GYRO IN_4 /*teste*/

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 60
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65

void reto() //testada
{

	OnFwdSync(MOTORES, -VELOCIDADE_MEDIA, 0);
	//este OnFwdSync ficava dentro de um while, mas é melhor que ele fique de fora para que tenhamos melhor controle dos laços de repetição
	//ou seja, o laço de repetição deve ficar na main
	// o último valor da função corrige a diferença entre os motores, que acontece devido a diferença de peso em cada um
	//o último valor foi modificado para 0, já que os pesos foram ajustados para que não fossem necessária nenhuma compensação

}

void virar_esquerda() //testada
{
	OnFwd(MOTOR_DIREITA, VELOCIDADE_BAIXA);
	OnRev(MOTOR_ESQUERDA, VELOCIDADE_BAIXA);
}

void virar_direita() //testada
{
	OnFwd(MOTOR_ESQUERDA, VELOCIDADE_BAIXA);
	OnRev(MOTOR_DIREITA, VELOCIDADE_BAIXA);
}

void re() //testada
{
	OnRevSync(MOTORES, -VELOCIDADE_ALTA, 0);
}

task main()
{
	reto();
	Wait(20000);
	Off(MOTORES);

}