/*
 * sensor.c
 */

#include "main.h"
#include "sensor.h"
#include "custom_delay.h"
#include "custom_gpio.h"
#include "custom_oled.h"
#include "custom_switch.h"
#include "custom_filesystem.h"
#include "custom_exception.h"

uint16_t sensor_value[8];
uint8_t sensor_raw[8];
uint8_t sw;
uint32_t whiteMax[8];
uint32_t blackMax[8];
int32_t normalized_value;
int32_t normalized[8];
uint8_t sensorState;
int32_t position[6] = {-10000,-6000,-2000,2000,6000,10000};
int32_t position_value;
float sensorThreshold =70;
float th = 0.40f;


void Sensor_Start() {
	LL_ADC_Enable(ADC1);
	Custom_Delay_ms(10); // ADC를 켜고 난 후, ADC 변환을 하기 전 내부 아날로그 안정화 작업을 위해 딜레이를 준다.

	LL_TIM_EnableCounter(TIM5); // TIM5의 타이머 카운터가 증가하도록 설정한다.
	LL_TIM_EnableIT_UPDATE(TIM5); // TIM5의 인터럽트가 동작하도록 설정한다. 인터럽트가 발생하면 Core/Src/stm32f4xx_it.c 파일 내부에 있는 인터럽트 핸들러 함수가 호출된다.
	Custom_Delay_ms(10);
}

void Sensor_Stop() {
	LL_ADC_Disable(ADC1);
	LL_TIM_DisableCounter(TIM5);
	LL_TIM_DisableIT_UPDATE(TIM5);
}

__STATIC_INLINE uint16_t Sensor_ADC_Read() {
	__disable_irq();
	LL_ADC_ClearFlag_EOCS(ADC1);
	LL_ADC_REG_StartConversionSWStart(ADC1);
	while (!LL_ADC_IsActiveFlag_EOCS(ADC1));
	uint16_t adcValue = LL_ADC_REG_ReadConversionData12(ADC1);
	LL_ADC_ClearFlag_EOCS(ADC1);
	__enable_irq();
	return adcValue;
}

void Sensor_TIM5_IRQ() {
	static uint32_t i = 0; // 현재 값을 읽을 센서 인덱스

	// Mux를 사용하여 IR LED 및 수광 센서 선택
	// --- 코드 작성 ---
	GPIOC->ODR = (GPIOC->ODR & 0xFFFFFFF8) | i;

	// 선택한 IR LED 켜기
	// --- 코드 작성 ---
	GPIOC->ODR |= (1 << 3); //! = i;

	// ADC 읽기
	// --- 코드 작성 ---
	uint8_t adc_value1 = Sensor_ADC_Read() >> 4;
	uint8_t adc_value2 = Sensor_ADC_Read() >> 4;
	uint8_t adc_value3 = Sensor_ADC_Read() >> 4;
	uint8_t adc_value = 0;
//	uint8_t adc_value = Sensor_ADC_Read() >> 4;


	if((adc_value1>=adc_value2)&&(adc_value2>=adc_value3)){
		adc_value = adc_value2;
	}
	else if((adc_value1>=adc_value3)&&(adc_value3>=adc_value2)){
		adc_value = adc_value3;
	}
	else if((adc_value2>=adc_value1)&&(adc_value1>=adc_value3)){
		adc_value = adc_value1;
	}
	else if((adc_value2>=adc_value3)&&(adc_value3>=adc_value1)){
		adc_value = adc_value3;
	}
	else if((adc_value3>=adc_value1)&&(adc_value1>=adc_value2)){
		adc_value = adc_value1;
	}
//	else if((adc_value3>=adc_value2)&&(adc_value2>=adc_value1)){
	else {
		adc_value = adc_value2;
	}


		// 선택한 IR LED 끄기
	// --- 코드 작성 ---
	GPIOC->ODR &= ~(1 << 3);

	// normalized value 계산
	// --- 코드 작성 ---
	sensor_raw[i] = adc_value;
	if (sensor_raw[i] < blackMax[i])
	normalized[i] = 0;
	else if (sensor_raw[i] > whiteMax[i])
	normalized[i] = 255;
	else
	normalized[i] = 255 * (sensor_raw[i] - blackMax[i]) / (whiteMax[i]
			- blackMax[i]);

	// sensor state 계산
	// --- 코드 작성 ---
	sensorState = (sensorState & ~(0x01 << (i)))
			| ((normalized[i] > sensorThreshold) << (i));

	// 다른 곳에서 쓸 수 있게 전역 배열에 할당
	// --- 코드 작성 ---
	// 인덱스 증가
	i = (i + 1) & 0x07;
}

void Sensor_Test_Raw() {
	Sensor_Start();
	// 센서의 Raw 값을 디스플레이에 출력해 확인하기
	// --- 코드 작성 ---
	for (;;) {
		Custom_OLED_Printf(
				"/g%02x/w%02x/g%02x/w%02x/g%02x/w%02x/g%02x/w%02x",
				sensor_raw[0], sensor_raw[1], sensor_raw[2], sensor_raw[3],
				sensor_raw[4], sensor_raw[5], sensor_raw[6], sensor_raw[7]);
		if (CUSTOM_SW_BOTH == (sw = Custom_Switch_Read())) break;
	}

	Sensor_Stop();
}

void Sensor_Calibration() {
	Sensor_Start();

	// Get whiteMax
	// --- 코드 작성 ---
	Custom_OLED_Clear();
	Custom_OLED_Printf("WhiteMax");
	while (CUSTOM_SW_BOTH != Custom_Switch_Read());
	for (int i = 0;; i++) {
		i &= 0x07;
		if (sensor_raw[i] > whiteMax[i]) whiteMax[i] = sensor_raw[i];
		Custom_OLED_Printf(
				"/g%02x/w%02x/g%02x/w%02x/g%02x/w%02x/g%02x/w%02x",
				whiteMax[0], whiteMax[1], whiteMax[2], whiteMax[3], whiteMax[4],
				whiteMax[5], whiteMax[6], whiteMax[7]);
		if (Custom_Switch_Read()) break;
	}

	// Get blackMax
	// --- 코드 작성 ---
	Custom_OLED_Clear();
	Custom_OLED_Printf("BlackMax");
	while (CUSTOM_SW_BOTH != Custom_Switch_Read());
	for (int i = 0;; i++) {
		i &= 0x07;
		if (sensor_raw[i] > blackMax[i]) blackMax[i] = sensor_raw[i];
		Custom_OLED_Printf(
				"/g%02x/w%02x/g%02x/w%02x/g%02x/w%02x/g%02x/w%02x",
				blackMax[0], blackMax[1], blackMax[2], blackMax[3], blackMax[4],
				blackMax[5], blackMax[6], blackMax[7]);
		if (Custom_Switch_Read()) break;
	}

	// Calculate ADC coefficients
	// --- 코드 작성 ---

	Sensor_Stop();
}

void Sensor_Test_Normalized() {
	Sensor_Start();
	// 센서의 Normalized 값을 디스플레이에 출력해 확인하기
	// --- 코드 작성 ---
	for (;;) {
		Custom_OLED_Printf(
				"/g%02x/w%02x/g%02x/w%02x/g%02x/w%02x/g%02x/w%02x",
				normalized[0], normalized[1], normalized[2], normalized[3],
				normalized[4], normalized[5], normalized[6], normalized[7]);

		if (CUSTOM_SW_BOTH == (sw = Custom_Switch_Read())) break;
	}
	Sensor_Stop();
}

void Sensor_Test_State() {
	Sensor_Start();
	// 센서의 State 값을 디스플레이에 출력해 확인하기
	// --- 코드 작성 ---
	Custom_OLED_Clear();
	char stateStr[9] = "00000000";
	for (int i = 0;; i++) {
		i &= 0x07;
		stateStr[i] = (sensorState & (0x01 << i)) ? '1' : '0';
		Custom_OLED_Printf("/0%s/1Th=%1.2f", stateStr, sensorThreshold );
		int sw = Custom_Switch_Read();
		if (sw == CUSTOM_SW_1) {
			th -=0.1;
			sensorThreshold = 255*th;
		}
		else if (sw == CUSTOM_SW_2) {
			th +=0.1;
			sensorThreshold = 255*th;
		}
		else if (sw == CUSTOM_SW_BOTH) {
			break;
		}
	}
	Sensor_Stop();
}

void position_test(){
	Sensor_Start();
	Custom_OLED_Clear();

	while(Custom_Switch_Read() != CUSTOM_SW_BOTH){
		int32_t sigma_p = 0;
		for(int i = 0; i<6;i++){
			sigma_p += position[i] * normalized[i + 1];
		}
		normalized_value=0;
		for(int i = 0;i<6;i++){
			normalized_value+=normalized[i+1];
		}
		if(normalized_value == 0) position_value = 0;
		else position_value = sigma_p/normalized_value;
		Custom_OLED_Printf("%6d", position_value);
	}

	Sensor_Stop();
}


