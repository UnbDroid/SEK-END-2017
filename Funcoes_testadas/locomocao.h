#define MOTOR_ESQUERDA OUT_A
#define MOTOR_DIREITA OUT_C
#define AMBOS_MOTORES OUT_AC

#define VELOCIDADE_BAIXINHA 15
#define VELOCIDADE_BAIXA 35
#define VELOCIDADE_MEDIA 50
#define VELOCIDADE_ALTA 65

void reto() //testada
{

	OnFwdSync(AMBOS_MOTORES, -VELOCIDADE_MEDIA, 0);
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
	OnRevSync(AMBOS_MOTORES, -VELOCIDADE_ALTA, 0);
}

void distancia_reto(int low_speed, int high_speed, int distancia){
	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	OnRev(AMBOS_MOTORES, high_speed);
	do{
		if (count_A - MotorRotationCount(MOTOR_ESQUERDA) > count_C - MotorRotationCount(MOTOR_DIREITA))
		{
			OnRev(MOTOR_ESQUERDA, low_speed);
			until ((count_C - MotorRotationCount(MOTOR_DIREITA)) >  (count_A - MotorRotationCount(MOTOR_ESQUERDA)));
			OnRev(MOTOR_ESQUERDA, high_speed);

		}
		else
		{
			OnRev(MOTOR_DIREITA, low_speed);
			until ( (count_A - MotorRotationCount(MOTOR_ESQUERDA)) > (count_C - MotorRotationCount(MOTOR_DIREITA)));
			OnRev(MOTOR_DIREITA, high_speed);
		}
	}while((count_A - MotorRotationCount(MOTOR_ESQUERDA))*6*PI/360 <= distancia);
	Off(AMBOS_MOTORES);
}

void distancia_re(int low_speed, int high_speed, int distancia){
	int count_A =  MotorRotationCount(MOTOR_ESQUERDA);
	int count_C =  MotorRotationCount(MOTOR_DIREITA);
	OnFwd(AMBOS_MOTORES, high_speed);
	do{
		if (MotorRotationCount(MOTOR_ESQUERDA) - count_A > MotorRotationCount(MOTOR_DIREITA) - count_C)
		{
			OnFwd(MOTOR_ESQUERDA, low_speed);
			until ((MotorRotationCount(MOTOR_DIREITA) - count_C) >  (MotorRotationCount(MOTOR_ESQUERDA) - count_A));
			OnFwd(MOTOR_ESQUERDA, high_speed);

		}
		else
		{
			OnFwd(MOTOR_DIREITA, low_speed);
			until ( (MotorRotationCount(MOTOR_ESQUERDA) - count_A) > (MotorRotationCount(MOTOR_DIREITA) - count_C));
			OnFwd(MOTOR_DIREITA, high_speed);
		}
	}while((MotorRotationCount(MOTOR_ESQUERDA) - count_A)*6*PI/360 <= distancia);
	Off(AMBOS_MOTORES);
}