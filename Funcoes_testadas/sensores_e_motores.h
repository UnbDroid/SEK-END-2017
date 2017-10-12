#define MOTOR_DIREITO OUT_A
#define MOTOR_ESQUERDO OUT_C
#define AMBOS_MOTORES OUT_AC
#define MOTOR_GARRA OUT_B

#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_2
#define SENSOR_GYRO IN_1

// #include "Funcoes_testadas/comunicacao.h" // Para as funcoes abaixo e necessario a biblioteca de comunicacao 

void ligar_sensores()
{
	SetSensorHTGyro(SENSOR_GYRO);
	SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	SetSensorUltrasonic(SENSOR_US_DIREITA);
	// SetSensorColorFull(COR_DIREITA);  // Seria se estivesse no cerebro principal
	// SetSensorColorFull(COR_ESQUERDA); // Seria se estivesse no cerebro principal
	// RemoteSetInputMode(CONEXAO, COR_ESQUERDA, SENSOR_TYPE_COLORFULL, SENSOR_MODE_RAW); // Caso nao tenha a biblioteca da comunicacao
	// RemoteSetInputMode(CONEXAO, COR_DIREITA, SENSOR_TYPE_COLORFULL, SENSOR_MODE_RAW);  // Caso nao tenha a biblioteca da comunicacao
	set_sensor_color(COR_ESQUERDA, VERMELHO); /* De ver melhor */
	set_sensor_color(COR_DIREITA, VERMELHO);  /* De ver melhor */
}