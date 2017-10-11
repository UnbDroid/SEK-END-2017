#define MOTOR_DIREITO OUT_A
#define MOTOR_ESQUERDO OUT_C
#define AMBOS_MOTORES OUT_AC
#define MOTOR_GARRA OUT_B

#define MOTOR_PORTA OUT_B
#define COR_ESQUERDA IN_4
#define COR_DIREITA IN_1

#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_2
#define SENSOR_GYRO IN_1

void ligar_sensores()
{
	SetSensorHTGyro(SENSOR_GYRO);
	SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	SetSensorUltrasonic(SENSOR_US_DIREITA);
	SetSensorColorFull(COR_DIREITA);
	SetSensorColorFull(COR_ESQUERDA);
	//SetSensorUltrasonic(SENSOR_US_GARRA);
}
