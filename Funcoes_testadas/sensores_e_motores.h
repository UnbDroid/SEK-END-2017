#define MOTOR_DIREITO OUT_A
#define MOTOR_ESQUERDO OUT_C
#define AMBOS_MOTORES OUT_AC
#define MOTOR_GARRA OUT_B
#define MOTOR_PORTA OUT_B

#define SENSOR_US_ESQUERDA IN_3
#define SENSOR_US_DIREITA IN_4
#define SENSOR_GYRO IN_4

void ligar_sensores()
{
	SetSensorHTGyro(SENSOR_GYRO);
	//SetSensorUltrasonic(SENSOR_US_ESQUERDA);
	//SetSensorUltrasonic(SENSOR_US_DIREITA);
	SetSensorColorRed(SENSOR_COR_DIREITA);
	SetSensorColorRed(SENSOR_COR_ESQUERDA);
	//SetSensorUltrasonic(SENSOR_US_GARRA);
}
