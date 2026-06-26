/*
 * sensor.h
 */

void Sensor_TIM5_IRQ();

void Sensor_Start();
void Sensor_Stop();

void Sensor_Calibration();

void Sensor_Test_Raw();
void Sensor_Test_Normalized();
void Sensor_Test_State();
void position_test();

extern uint16_t sensor_value[8];
extern uint8_t sensor_raw[8];
extern uint8_t sw;
extern uint32_t whiteMax[8];
extern uint32_t blackMax[8];
extern int32_t normalized_value;
extern int32_t normalized[8];
extern uint8_t sensorState;
extern int32_t position[6];
extern int32_t position_value;
extern float sensorThreshold;
extern float th;
