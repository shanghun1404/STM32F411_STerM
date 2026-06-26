///FTDI.c
///최초 생성일 : 2025년 10월 9일
///FTDI UART 관련 함수입니다.

///Part 1.
///FTDI 관련 함수

/// 기본 라이브러리 인클루드
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

/// 스타라잇 전용 라이브러리 인클루드
#include "Custom_UART_PuTTY.h"
#include "starColor.h"

/// FTDI 관련 변수
static uint8_t FTDI_rx = 0;
static uint8_t FTDI_buffer[256] = {0, };
static uint8_t FTDI_startQ = 0;
static uint8_t FTDI_endQ = 0;
static uint32_t FTDI_bRate = 115200;


extern void * PROC_refer[32];

/// 스타라잇 이벤트 발생(자동 업로드를 위해 필요)
#define REMOTE_CONTROL_CHAR 0x14
#define FTDI_PAUSE 0x11

uint32_t FTDI_Event = 0;
uint32_t FTDI_EventCnt = 0;

uint32_t FTDI_Ebegin = 0;
uint32_t FTDI_Rmode = 0;
uint32_t FTDI_Gmode = 0;
uint32_t FTDI_Bmode = 0;
uint32_t FTDI_Rval = 0;
uint32_t FTDI_Gval = 0;
uint32_t FTDI_Bval = 0;

uint32_t FTDI_EventAllow = 1;


extern UART_HandleTypeDef huart2;
#define FTDI_UART huart2





/// @desc : FTDI 인터럽트 발생 시 실행하는 함수(Remote 방식 변경으로 많이 뜯어 고칠거임)
void FTDI_IRQHandler(){
//	HAL_UART_Receive(&FTDI_UART, &FTDI_rx, 1, HAL_MAX_DELAY);

//	while(!LL_USART_IsActiveFlag_RXNE(FTDI_UART));
//	FTDI_rx = LL_USART_ReceiveData8(FTDI_UART);
	FTDI_buffer[FTDI_endQ++] = FTDI_rx;

	HAL_UART_Receive_IT(&FTDI_UART, &FTDI_rx, 1);
}

/// @desc : FTDI 활성화 함수
void FTDI_Begin(uint32_t bRate){
	FTDI_bRate = bRate;
//	LL_RCC_ClocksTypeDef rcc_clocks;
//    LL_RCC_GetSystemClocksFreq(&rcc_clocks);
//	LL_USART_SetBaudRate(FTDI_UART, rcc_clocks.PCLK1_Frequency, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_16, bRate);
//	LL_USART_Enable(FTDI_UART);
//	LL_USART_EnableIT_RXNE(FTDI_UART);
	HAL_UART_Init(&FTDI_UART);
	HAL_UART_Receive_IT(&FTDI_UART, &FTDI_rx, 1);
}

/// @desc : FTDI와 연결되는 UART가 사용 가능한지 확인합니다.
uint32_t FTDI_IsBegin(){
	if(FTDI_UART.Instance->CR1 & USART_CR1_UE){
		return 1;
	}
	return 0;

	//return LL_USART_IsEnabled(FTDI_UART);
}

/// @desc : FTDI 비활성화 함수
void FTDI_End(){
	HAL_UART_DeInit(&FTDI_UART);

	//LL_USART_Disable(FTDI_UART);
	//LL_USART_DisableIT_RXNE(FTDI_UART);
}

/// @desc : FTDI 출력(전송) 함수
void FTDI_Print(const char * str){
	uint8_t str1[512];
	strcpy((char*)str1, str);

	HAL_UART_Transmit(&FTDI_UART, str1, strlen(str), 10);

//	LL_USART_DisableIT_RXNE(FTDI_UART);
//	for(uint32_t i = 0; str[i]; i++){
//		while(!LL_USART_IsActiveFlag_TXE(FTDI_UART));
//		LL_USART_TransmitData8(FTDI_UART, (uint8_t)str[i]);
//	}
//	LL_USART_EnableIT_RXNE(FTDI_UART);
}

/// @desc : FTDI 포매팅 함수
void FTDI_Printf(const char * format, ...){
	/*
	 * buffer를 많이 할당받거나 static으로 선언하면 좋겠지만, 보통 프로세스당 4GB의 메모리를 가지는 일반 컴퓨터와 다르게
	 * STM32F411의 경우 전체 메모리 공간이 오직 128kB의 저장용량만 가지므로 메모리 공간을 아껴 써야 한다.
	 */
	char buffer[512] = { 0 };

	// 가변 인자를 받아서 buffer에 쓴다.
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	FTDI_Print(buffer);
}

/// @desc : FTDI 바이트 단위 읽기 함수
int32_t FTDI_Read(){
	if(FTDI_startQ == FTDI_endQ){
		return -1;
	}
	FTDI_startQ ++;
	return FTDI_buffer[(FTDI_startQ - 1) & 0xFF];
}

/// @desc : FTDI 바이트 단위 쓰기 함수
void FTDI_Write(uint8_t data){
	HAL_UART_Transmit(&FTDI_UART, &data, 1, 10);

//	LL_USART_DisableIT_RXNE(FTDI_UART);
//	while(!LL_USART_IsActiveFlag_TXE(FTDI_UART));
//	LL_USART_TransmitData8(FTDI_UART, data);
//	LL_USART_EnableIT_RXNE(FTDI_UART);
}

/// @desc : FTDI 입력 버퍼에 들어온 문자열 개수 반환하는 함수
uint8_t FTDI_Available(){
	return FTDI_endQ - FTDI_startQ;
}

/// @desc : FTDI에 들어온 문자열 인식 함수(Delay 방식)
uint8_t FTDI_Scan(char * str, uint32_t delay){
	uint8_t i = 0;
	while(FTDI_Available()){
		str[i++] = FTDI_Read();
		FTDI_Delay(delay);
	}
	str[i] = '\0';
	return i;
}

/// @desc : FTDI로부터 데이터가 들어오거나 정해진 시간이 지날때까지 기다리는 함수
void FTDI_Delay(uint32_t delay){
	uint32_t time = HAL_GetTick();
	while(!FTDI_Available()){
		if(HAL_GetTick() - time > delay) break;
	}
}

/// @desc : FTDI 버퍼 초기화하는 함수로, 이 상태에서는 데이터가 읽히지 않음.
void FTDI_Flush(){
	FTDI_endQ = FTDI_startQ;
}

/// @desc : FTDI에 들어온 데이터를 미리 보는 함수로, Read와 달리 버퍼 길이가 변하지 않음.
uint32_t FTDI_Peek(uint32_t pos){
	return(FTDI_buffer[FTDI_startQ + pos]);
}

///Part 2.
///PuTTY 관련 함수

///PuTTY 관련 변수
static uint32_t PuTTY_isScan = 0;
static uint32_t PuTTY_println = 0;
static char PuTTY_str[256] = "";
static uint32_t PuTTY_idx = 0;
static char PuTTY_stack[8][256];//최근 명령어 8개 저장. 2048바이트 소요.
static char PuTTY_stackCnt = 0;
static uint32_t PuTTY_stackCur = 0;

/// @desc : 최근 명령어 리스트를 초기화하는 함수
void PuTTY_Init(){
	PuTTY_stackCnt = 0;
	PuTTY_stackCur = 0;
	for(uint32_t i = 0; i < 8; i++)
		PuTTY_stack[i][0] = 0;
	PuTTY_str[0] = 0;
}

/// @desc : PuTTY 스캔 시작
void PuTTY_StartScan(uint32_t mode){
	PuTTY_isScan = mode & 0x03;
	PuTTY_println = (mode >> 2) & 0x03;
	PuTTY_idx = 0;
	FTDI_Flush();
}

/// @desc : 실시간 스캔 진행
static uint8_t PuTTY_PrevUTF8(char * str, uint8_t idx){
	if(idx == 0){
		return 0;
	}
	if(!(str[idx-1] & 0x80)){
		return idx-1;
	}
	else if((str[idx-2] & 0xE0) == 0xC0){
		return idx-2;
	}
	else if((str[idx-3] & 0xF0) == 0xE0){
		return idx-3;
	}
	else if((str[idx-3] & 0xF0) == 0xE0){
		return idx-3;
	}
		return idx-4;
}
static uint8_t PuTTY_NextUTF8(char * str, uint8_t idx){
	if(idx == strlen(str)){
		return idx;
	}
	if(!(str[idx] & 0x80)){
		return idx + 1;
	}
	if((str[idx] & 0xE0) == 0xC0){
		return idx + 2;
	}
	if((str[idx] & 0xF0) == 0xE0){
		return idx + 3;
	}
		return idx + 4;
}
static uint32_t PuTTY_UTF8Scan(char * str, uint8_t idx){
	uint8_t sc;
	uint32_t val = UTF8_Val(str + idx, &sc);
	return val;
}
int32_t PuTTY_Scan(char * str){
	if(PuTTY_isScan){
		if(FTDI_Available()){
			char re = FTDI_Read();
			if(re == '\r'){//엔터키
				if((PuTTY_isScan == 1 || PuTTY_isScan == 3) && (PuTTY_println & 0x02)){
					FTDI_Printf("\r\n");
				}
				uint32_t idx = PuTTY_idx;
				PuTTY_idx = 0;
				PuTTY_isScan = 0;
				strcpy(str, PuTTY_stack[0]);

				//명령어 저장 단계
				uint32_t i = 0;
				for(i = 1; i < 8; i++){
					if(!strcmp(str, PuTTY_stack[i]))break;
				}

				for(uint32_t j = 0; j < i; j++){
					if(i - j < 8)
						strcpy(PuTTY_stack[i - j], PuTTY_stack[i - j - 1]);
				}

				if(i == 8 && PuTTY_stackCnt < 7){
					PuTTY_stackCnt++;
				}
				PuTTY_stackCur = 0;


				PuTTY_stack[0][0] = 0;


				return idx;//입력된 글자 수
			}
			else if(re == 127){//백스페이스키
				if(PuTTY_idx > 0){
					uint32_t len = strlen(PuTTY_stack[0]);
					//uint32_t back = len - PuTTY_idx;
					uint32_t prev = PuTTY_idx - PuTTY_PrevUTF8(PuTTY_stack[0], PuTTY_idx);
					uint32_t unilen = 1;
					uint32_t unival = PuTTY_UTF8Scan(PuTTY_stack[0], PuTTY_idx-prev);
					for(uint32_t i = PuTTY_idx; i <= len; i++){
						PuTTY_stack[0][i - prev] = PuTTY_stack[0][i];
					}
					PuTTY_idx-=prev;





					if(PuTTY_isScan == 1){
						uint32_t after = strunilen(PuTTY_stack[0] + PuTTY_idx);
						if(unival & 0xFFFF0000){
							unilen = 3;
						}
						else if(unival & 0x0000FF00){
							unilen = 2;
						}
						for(uint32_t i = 0; i < unilen; i++){
							FTDI_Printf("%c[D", 0x1B);
						}
						FTDI_Printf(PuTTY_stack[0] + PuTTY_idx);
						for(uint32_t i = 0; i < unilen; i++){
							FTDI_Printf(" ", 0x1B);
						}
						for(uint32_t i = 0; i < after + unilen; i++){
							FTDI_Printf("%c[D", 0x1B);
						}
					}
					PuTTY_stackCur = 0;
				}
			}
			else if(re == 0x1B){
				FTDI_Delay(5);
				if(FTDI_Available()){
					re = FTDI_Read();
					while(!FTDI_Available());
					re = FTDI_Read();
					if(re == 'A'){
						if(PuTTY_stackCur < PuTTY_stackCnt){
	//						uint32_t num = strlen(PuTTY_stack[0]);
							if(PuTTY_stackCur == 0){
								strcpy(PuTTY_str, PuTTY_stack[0]);
							}
							PuTTY_stackCur++;
							strcpy(PuTTY_stack[0], PuTTY_stack[PuTTY_stackCur]);
							//글자수만큼 지우기
							for(uint32_t i = 0; i < PuTTY_idx; i++){
								FTDI_Printf("%c %c", 127, 127);
							}


							//FTDI_Printf("\r%c[2K\r", 0x1B);
							FTDI_Printf(PuTTY_stack[0]);
							PuTTY_idx = strlen(PuTTY_stack[0]);
						}
					}
					else if(re == 'B'){
						if(PuTTY_stackCur > 0){
	//						uint32_t num = strlen(PuTTY_stack[0]);
							PuTTY_stackCur--;
							if(PuTTY_stackCur == 0){
								strcpy(PuTTY_stack[0], PuTTY_str);
							}
							else{
								strcpy(PuTTY_stack[0], PuTTY_stack[PuTTY_stackCur]);
							}
							//글자수만큼 지우기
							for(uint32_t i = 0; i < PuTTY_idx; i++){
								FTDI_Printf("%c %c", 127, 127);
							}
							FTDI_Printf(PuTTY_stack[0]);
							PuTTY_idx = strlen(PuTTY_stack[0]);
						}
					}
					else if(re == 'C' || re == 'D'){
						if(re == 'C'){
							if(PuTTY_idx < strlen(PuTTY_stack[0])){
								uint32_t unilen = 1;
								uint32_t unival = PuTTY_UTF8Scan(PuTTY_stack[0], PuTTY_idx);
								PuTTY_idx = PuTTY_NextUTF8(PuTTY_stack[0], PuTTY_idx);
								if(unival & 0xFFFF0000){
									unilen = 3;
								}
								else if(unival & 0x0000FF00){
									unilen = 2;
								}
								for(uint32_t i = 0; i < unilen; i++){
									FTDI_Printf("%c[C", 0x1B);
								}
							}
						}
						else if(re == 'D'){
							if(PuTTY_idx > 0){
								PuTTY_idx = PuTTY_PrevUTF8(PuTTY_stack[0], PuTTY_idx);
								uint32_t unilen = 1;
								uint32_t unival = PuTTY_UTF8Scan(PuTTY_stack[0], PuTTY_idx);
								if(unival & 0xFFFF0000){
									unilen = 3;
								}
								else if(unival & 0x0000FF00){
									unilen = 2;
								}
								for(uint32_t i = 0; i < unilen; i++){
									FTDI_Printf("%c[D", 0x1B);
								}
							}
						}
					}
				}
			}
			else if(re != '\r' && re != '\n'){
				if(re & 0x80){
					if((re & 0xE0) == 0xC0){
						uint32_t len = strlen(PuTTY_stack[0]);
						uint32_t back = strunilen(PuTTY_stack[0] + PuTTY_idx);
						for(uint32_t i = len+1; i > PuTTY_idx; i--){
							PuTTY_stack[0][i] = PuTTY_stack[0][i-2];
						}
						PuTTY_stack[0][PuTTY_idx] = re;
						while(!FTDI_Available());
						re = FTDI_Read();
						PuTTY_stack[0][PuTTY_idx + 1] = re;
						PuTTY_stack[0][len + 2] = 0;
						if(PuTTY_isScan == 1){
							FTDI_Printf(PuTTY_stack[0] + PuTTY_idx);
							for(uint32_t i = 0; i < back; i++){
								FTDI_Printf("%c[D", 0x1B);
							}
						}
						PuTTY_idx+=2;
						PuTTY_stackCur = 0;
					}
					else if((re & 0xF0) == 0xE0){
						uint32_t len = strlen(PuTTY_stack[0]);
						uint32_t back = strunilen(PuTTY_stack[0] + PuTTY_idx);
						for(uint32_t i = len+2; i > PuTTY_idx; i--){
							PuTTY_stack[0][i] = PuTTY_stack[0][i-3];
						}
						PuTTY_stack[0][PuTTY_idx] = re;
						while(!FTDI_Available());
						re = FTDI_Read();
						PuTTY_stack[0][PuTTY_idx + 1] = re;
						while(!FTDI_Available());
						re = FTDI_Read();
						PuTTY_stack[0][PuTTY_idx + 2] = re;
						PuTTY_stack[0][len + 3] = 0;
						if(PuTTY_isScan == 1){
							FTDI_Printf(PuTTY_stack[0] + PuTTY_idx);
							for(uint32_t i = 0; i < back; i++){
								FTDI_Printf("%c[D", 0x1B);
							}
						}
						PuTTY_idx+=3;
						PuTTY_stackCur = 0;
					}
					else if((re & 0xF1) == 0xF0){
						uint32_t len = strlen(PuTTY_stack[0]);
						uint32_t back = strunilen(PuTTY_stack[0] + PuTTY_idx);
						for(uint32_t i = len+3; i > PuTTY_idx; i--){
							PuTTY_stack[0][i] = PuTTY_stack[0][i-4];
						}
						PuTTY_stack[0][PuTTY_idx] = re;
						while(!FTDI_Available());
						re = FTDI_Read();
						PuTTY_stack[0][PuTTY_idx + 1] = re;
						while(!FTDI_Available());
						re = FTDI_Read();
						PuTTY_stack[0][PuTTY_idx + 2] = re;
						while(!FTDI_Available());
						re = FTDI_Read();
						PuTTY_stack[0][PuTTY_idx + 3] = re;
						PuTTY_stack[0][len + 4] = 0;
						if(PuTTY_isScan == 1){
							FTDI_Printf(PuTTY_stack[0] + PuTTY_idx);
							for(uint32_t i = 0; i < back; i++){
								FTDI_Printf("%c[D", 0x1B);
							}
						}
						PuTTY_idx+=4;
						PuTTY_stackCur = 0;
					}
				}
				else{
					uint32_t len = strlen(PuTTY_stack[0]);
					uint32_t back = strunilen(PuTTY_stack[0] + PuTTY_idx);
					for(uint32_t i = len; i > PuTTY_idx; i--){
						PuTTY_stack[0][i] = PuTTY_stack[0][i-1];
					}
					PuTTY_stack[0][PuTTY_idx] = re;
					PuTTY_stack[0][len + 1] = 0;
					if(PuTTY_isScan == 1){
						FTDI_Printf(PuTTY_stack[0] + PuTTY_idx);
						for(uint32_t i = 0; i < back; i++){
							FTDI_Printf("%c[D", 0x1B);
						}
					}
					PuTTY_idx++;
					PuTTY_stackCur = 0;
				}
			}
		}
	}
	return -1;
}

/// @desc : PuTTY를 통해 스캔중인지 확인하는 함수
uint32_t PuTTY_IsScanning(){
	if(PuTTY_isScan){
		return PuTTY_isScan | (PuTTY_println << 2);
	}
	else return 0;
}

/// @desc : FTDI를 통해 PuTTY(Teraterm) 또는 CMD 창 Format으로 변환해 출력.
void PuTTY_Print(const char * str){
	//Starlit 규격을 ANSI 규격으로 환산합니다.

	char colorStr[64] = "";
	uint32_t colorScan = 0;
	char puttyStr[256] = "";

	//스캔
	for(uint32_t i = 0; str[i]; i++){
		//'/' 감지
		if(str[i] == '\n' && !(PuTTY_println & 0x01)){
			strcat(puttyStr, "\r\n");
		}
		else if(str[i] == '/'){
			uint32_t color = 0xFFFFFF;
			if(str[i+1] == '/'){
				i++;
				char ss[2] = {'/', 0};
				strcat(puttyStr, ss);
				continue;
			}
			else if(str[i+1] >= '0' && str[i+1] <= '9'){
				int y = str[i+1] - '0';

				char posstr[50];
				sprintf(posstr, "%c[%d;%dH", 0x1B, y+1, 1);
				FTDI_Print(posstr);

				i++;
				continue;
			}
			else if(str[i+1] == '['){
				//`,`, `]`가 나타나는 위치 찾기
				int32_t commaPos = -1;
				int32_t closePos = -1;
				char comma = 0;
				for(uint32_t j = i + 2; str[j]; j++){
					if(str[j] == ']'){
						closePos = j;
						break;
					}
					else if(str[j] == ','){
						commaPos = j;
						comma = str[j];
					}
				}

				if(comma == ','){//컴마 사용 시 - 컴마 종류에 따라 위치선정
					char xpos[20];
					char ypos[20];
					strncpy(xpos, str + i + 2, commaPos - i - 2);
					xpos[commaPos - i - 2] = 0;
					int x = atoi(xpos);
					strncpy(ypos, str + commaPos + 1, closePos - commaPos - 1);
					ypos[closePos - commaPos - 1] = 0;
					int y = atoi(ypos);

					char posstr[50];
					sprintf(posstr, "%c[%d;%dH", 0x1B, y+1, x+1);
					FTDI_Print(posstr);

					i = closePos;
					continue;
				}
				else if(comma == 0){
					char ypos[20];
					strncpy(ypos, str + i + 2, closePos - i - 2);
					ypos[closePos - i - 2] = 0;
					int y = atoi(ypos);

					char posstr[50];
					sprintf(posstr, "%c[%d;%dH", 0x1B, y+1, 1);
					FTDI_Print(posstr);


					i = closePos;
					continue;
				}
				else{
					i += 1;
					continue;
				}

			}
			else if(str[i+1] == '#'){
				char strHex[7];
				i += 2;
				strncpy(strHex, str + i, 6);
				strHex[6] = '\0';
				color = xtoi(strHex);
				i += 5;
			}
			else if(str[i+1] == 'd'){
				switch(str[i+1]){
				case '1':
					color = COLOR_DEFAULT1;
					break;
				case '2':
					color = COLOR_DEFAULT2;
					break;
				case '3':
					color = COLOR_DEFAULT3;
					break;
				case '4':
					color = COLOR_DEFAULT4;
					break;
				case '0':
					if(colorScan == 1){
						sprintf(colorStr, "%c[0m", 0x1B);
						colorScan = 0;
					}
					i += 2;
					continue;
				default:
					color = PuTTY_findDarkColor(str[i+2]) & 0xFFFFFF;
					break;
				}

				i+=2;
			}
			else{
				color = PuTTY_findColor(str[i+1]) & 0xFFFFFF;
				i++;
			}

//			if(colorScan == 1){
//				sprintf(colorStr, "%c[0m", 0x1B);
//				strcat(puttyStr, colorStr);
//			}

			colorScan = 1;
			sprintf(colorStr, "%c[38;2;%ld;%ld;%ldm", 0x1B, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
			strcat(puttyStr, colorStr);
		}
		else if(str[i] == '$'){
			uint32_t color = 0xFFFFFF;
			if(str[i+1] == '$'){
				i++;
				char ss[2] = {'$', 0};
				strcat(puttyStr, ss);
				continue;
			}

			else if(str[i+1] == '#'){
				char strHex[7];
				i += 2;
				strncpy(strHex, str + i, 6);
				strHex[6] = '\0';
				color = xtoi(strHex);
				i += 5;
			}
			else if(str[i+1] == 'd'){
				switch(str[i+1]){
				case '1':
					color = COLOR_DEFAULT1;
					break;
				case '2':
					color = COLOR_DEFAULT2;
					break;
				case '3':
					color = COLOR_DEFAULT3;
					break;
				case '4':
					color = COLOR_DEFAULT4;
					break;
				default:
					color = PuTTY_findDarkColor(str[i+2]) & 0xFFFFFF;
					break;
				}

				i+=2;
			}
			else{
				color = PuTTY_findColor(str[i+1]) & 0xFFFFFF;
				i++;
			}
			colorScan = 1;
			sprintf(colorStr, "%c[48;2;%ld;%ld;%ldm", 0x1B, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
			strcat(puttyStr, colorStr);
		}
		else{
			char ss[2] = {str[i], 0};
			strcat(puttyStr, ss);
		}

	}

	if(colorScan == 1){
		sprintf(colorStr, "%c[0m", 0x1B);
		strcat(puttyStr, colorStr);
	}

	FTDI_Print(puttyStr);
}


void PuTTY_Printf(const char * format, ...){
	/*
	 * buffer를 많이 할당받거나 static으로 선언하면 좋겠지만, 보통 프로세스당 4GB의 메모리를 가지는 일반 컴퓨터와 다르게
	 * STM32F411의 경우 전체 메모리 공간이 오직 128kB의 저장용량만 가지므로 메모리 공간을 아껴 써야 한다.
	 */
	char buffer[512] = { 0 };

	// 가변 인자를 받아서 buffer에 쓴다.
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	PuTTY_Print(buffer);
}

/// @desc : PuTTY 창을 지우는 함수
void PuTTY_Erase(){
	FTDI_Printf("%c[2J%c[H", 0x1B, 0x1B);
}


