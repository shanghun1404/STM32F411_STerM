#ifndef __008__FTDI__H__
#define __008__FTDI__H__

/// FTDI 및 PuTTY 관련 함수 라이브러리입니다.
/// 개별적으로 사용 가능하도록 설계했습니다.

/// 반드시 포함되어야 하는 라이브러리
#include "starString.h"
///#include "starColor.h"

/// FTDI 관련 라이브러리 - 0080X 0081X 0082X

/// @desc : FTDI 인터럽트 발생 시 실행하는 함수
/// @code : None(Interrupt Service Routine)
void FTDI_IRQHandler();

/// @desc : FTDI 활성화 함수
/// @code : 00800-012
/// @param bRate : 전송 속도
void FTDI_Begin(uint32_t bRate);

/// @desc : FTDI와 연결되는 UART가 사용 가능한지 확인합니다.
/// @code : 00801-012
/// @return : UART가 활성화된 경우 1, 비활성화된 경우 0 출력
uint32_t FTDI_IsBegin();

/// @desc : FTDI 비활성화 함수
/// @code : 00802-012
void FTDI_End();

/// @desc : FTDI 출력(전송) 함수
/// @code : 00803-012
/// @param str : 출력 문자열(포인터 방식)
void FTDI_Print(const char * str);

/// @desc : FTDI 포매팅 함수
/// @code : None(가변인자 Starlit 미지원)
/// @param format : 포매팅 문자열
/// @param ... : %d, %f 등에 받을 데이터
/// Starlit Domain에서는 내부적으로 sprintf를 작동시킨다.
void FTDI_Printf(const char * format, ...);

/// @desc : FTDI 바이트 단위 읽기 함수
/// @code : 00804-012
/// @return : 버퍼에 문자열이 없는 경우 : -1
///         : 버퍼에 문자열이 있는 경우 : 가장 끝에 있는 데이터
int32_t FTDI_Read();

/// @desc : FTDI 바이트 단위 쓰기 함수
/// @code : 00805-012
/// @param data : FTDI에 전송할 글자
void FTDI_Write(uint8_t data);

/// @desc : FTDI 입력 버퍼에 들어온 문자열 개수 반환하는 함수
/// @code : 00806-012
/// @return : 버퍼에 있는 문자열 수(0~255)
uint8_t FTDI_Available();

/// @desc : FTDI에 들어온 문자열 인식 함수(Delay 방식)
/// @code : 00807-012
/// @param str : 스캔받을 문자열 배열(포인터)
/// @param delay : 타임아웃, 스캔되지 않은 상태에서 최대한 기다리는 시간(ms)
/// @return : 스캔이 완료될 때까지 기다린 후 스캔한 문자열 길이 출력.
uint8_t FTDI_Scan(char * str, uint32_t delay);

/// @desc : FTDI로부터 데이터가 들어오거나 정해진 시간이 지날때까지 기다리는 함수
/// @code : 00808-012
/// @param delay : 타임아웃, 데이터가 들어오지 않은 상태에서 최대한 기다리는 시간(ms)
void FTDI_Delay(uint32_t delay);

/// @desc : FTDI 버퍼 초기화하는 함수로, 이 상태에서는 데이터가 읽히지 않음.
/// @code : 00809-012
void FTDI_Flush();

/// @desc : FTDI에 들어온 데이터를 미리 보는 함수로, Read와 달리 버퍼 길이가 변하지 않음.
/// @code : 0080A-012
/// @param pos(신설) : 미리보기 위치
/// @return : 버퍼에 저장된 끝값
uint32_t FTDI_Peek(uint32_t pos);



/// PuTTY 관련 라이브러리 - 0083X 0084X 0085X

/// @desc : 최근 명령어 리스트를 초기화하는 함수
/// @code : 00830-012
void PuTTY_Init();

/// @desc : PuTTY 스캔 시작
/// @code : 00831-012
/// @param mode : 0 : 스캔 안함
///               1 : 일반 모드
///               3 : 출력 없이 스캔
#define PuTTY_MODE_OFF 0
#define PuTTY_MODE_ECHO_ON 1
#define PuTTY_MODE_ECHO_OFF 3
#define PuTTY_MODE_ON 3
// 줄바꿈 출력 규칙
#define PuTTY_MODE_PRINT_CRLF 4
#define PuTTY_MODE_PRINT_LF 0
// 줄바꿈 스캔 규칙
#define PuTTY_SCAN_LF 8
#define PuTTY_SCAN_NO_LF 0

void PuTTY_StartScan(uint32_t mode);

/// @desc : 실시간 스캔 진행
/// @code : 00832-012
/// @param str : 스캔 받을 문자열
/// @return : 스캔받지 않는 경우 : -1
///           스캔 받은 문자열 길이 : 엔터만 바로 친 경우 0 출력
int32_t PuTTY_Scan(char * str);

/// @desc : PuTTY를 통해 스캔중인지 확인하는 함수
/// @code : 00833-012
/// @return : 스캔하지 않는 경우 : 0
///           일반모드 : 1
///           출력 없는 모드 : 3
uint32_t PuTTY_IsScanning();

/// @desc : FTDI를 통해 PuTTY(Teraterm) 또는 CMD 창 Format으로 변환해 출력.
/// @code : 00834-012
/// @param str : 출력할 문자열
void PuTTY_Print(const char * str);

void PuTTY_Printf(const char * format, ...);

/// @desc : PuTTY 창을 지우는 함수
/// @code : 00835-012
void PuTTY_Erase();


#endif
