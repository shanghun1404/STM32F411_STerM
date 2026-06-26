#ifndef __005__STAR__STRING__H__
#define __005__STAR__STRING__H__

/// FTDI 및 PuTTY 관련 함수 라이브러리입니다.
/// Starlit Virtual Machine을 사용하려면 반드시 포함되어야 하는 라이브러리입니다.
/// 개별적으로 사용 가능하도록 설계했습니다.

/// FTDI 관련 라이브러리 - 0050X 0051X 0052X 0053X

/// 문자열 연산처리에 사용되는 지역변수 크기
#define STAR_STR_LEN_MAX 4096

/// @desc : 대소문자 구분 없이 문자열 비교하는 함수
/// @code : 00500-012
/// @param str1 : 비교 문자열 1
/// @param str2 : 비교 문자열 2
/// @return : 문자열이 같으면 0, 다르면 1 또는 -1 출력
int32_t strcmp2(const char * str1, const char * str2);
int32_t strcmp3(const char * str1, const char * str2);

/// @desc : 대소문자 구분 없이 문자열 n개를 비교하는 함수
/// @code : 00502-012
/// @param str1 : 비교 문자열 1
/// @param str2 : 비교 문자열 2
/// @param n : 비교 길이
/// @return : 문자열이 같으면 0, 다르면 1 또는 -1 출력
int32_t strncmp2(const char * str1, const char * str2, uint32_t n);
int32_t strncmp3(const char * str1, const char * str2, uint32_t n);

/// @desc(신설) : 문자열의 소문자를 대문자로 변경, 나머지는 그대로 둠.
/// @code : 00504-012
/// @param str : 변경할 문자열
void strupper(char * str);

/// @desc(신설) : 문자열의 대문자를 소문자로 변경, 나머지는 그대로 둠.
/// @code : 00505-012
/// @param str : 변경할 문자열
void strlower(char * str);


/// @desc : 한 문자열에 특정 위치에 문자열을 삽입합니다.
/// @code : 00506-012
/// @param str1 : 변경할 문자열
/// @param str2 : 삽입할 문자열
/// @param pos : 문자열 삽입할 위치
/// @return : 삽입 후 문자열 최종 길이
uint32_t strins(char * str1, const char * str2, uint32_t pos);

/// @desc : 한 문자열에 특정 구간을 지우고 그 위치에 다른 문자열을 삽입합니다.
/// @code : 00507-012
/// @param str : 변경할 문자열
/// @param start : 다른 문자열 넣을 위치(시작)
/// @param end : 다른 문자열 넣을 위치(끝)
/// @param ins : 삽입할 문자열, "" 넣을 시 구간 삭제로 처리.
/// @return : 삽입 후 문자열 최종 길이
uint32_t strins2(char* str, uint32_t start, uint32_t end, const char* ins);

/// @desc : UTF-8 기준에 따라 문자를 빼내는 함수
/// @code : 00508-012
/// @param str1 : 빼낸 문자를 받는 문자열
/// @param str2 : 특정 문자를 빼낼 문자열
/// @param cursor : 문자열 빼낼 위치
/// @return : 빼낸 문자열 개수
uint8_t strpop(char * str1, char * str2, uint32_t cursor);

/// @desc : 감춰야 할 문자가 있을 경우 감춤. 예: 1234->****
/// @code : 00509-012
/// @param strm : 마스크 출력 문자열
/// @str : 입력 문자열
/// @ch : 마스크 문자
/// @return : 문자열 길이
uint32_t strmsk(char * strm, const char * str, char ch);

/// @desc : 글자 개수 세기, 예: 가나다의 경우 3 출력
/// @code : 0050A-012
/// @param str : 문자열
/// @return : 글자 수
uint32_t strletlen(const char * str);

/// @desc : 유니코드 기준 바이트 수 세기, 예: 가나다의 경우 6 출력
/// @code : 0050B-012
/// @param str : 문자열
/// @return : 글자 수
uint32_t strunilen(const char * str);

/// @desc : UTF-8 포맷으로 인코딩된 문자를 문자열 형식으로 입력받아 유니코드 값으로 반환하는 함수
/// @code : 0050C-012
/// @param str : UTF-8 포맷의 문자열
/// @param size_char : UTF-8 차지하는 크기
/// @return : 유니코드 번호
uint32_t UTF8_Val(const char * str, uint8_t * size_char);

/// @desc : 유니코드를 입력하면 유니코드 형태에 맞게 변형해서 문자열 형식으로 출력하는 함수
/// @code : 0050D-012
/// @param str : 비어있는 문자열
/// @param unicode : 출력할 유니코드
/// @return : 문자열 길이(0~4)
uint8_t UTF8_String(char * str, uint16_t unicode);

/// @desc : 16진수를 정수형으로 변경하는 함수
/// @code : 00510-012
/// @param str : 16진수가 포함되어 있는 문자열
/// @return : 정수, 16진수를 취급할 때는 부호 취급하지 않습니다.
uint32_t xtoi(const char * str);

/// @desc : 16진수를 64비트 정수형으로 변경하는 함수
/// @code : 00511-012
/// @param str : 16진수가 포함되어 있는 문자열
/// @return : 정수, 16진수를 취급할 때는 부호 취급하지 않습니다.
uint64_t xtoll(const char * str);

/// @desc : 문자열 포매팅 함수(sprintf 함수가 오작동하는 경우 이 함수 사용하세요.)
/// @code : NONE
/// @param buffer : 출력할 문자열
/// @param format : 형식 문자가 포함된 문자열
/// @param ... : 형식 문자에 치환될 데이터
void STAR3_Sprintf(char * buffer, const char * format, ...);

/// @desc : 문자열을 특정 문자를 사용해 분해하는 함수. 스타라잇 내에서 문자열 배열을 정의한 후 사용 가능한 Starlit 전용 함수입니다.
/// @code : 00520-012
/// @param text : 분해할 텍스트
/// @param splitchar : 분해 기준이 되는 문자
/// @ptr : 문자열 배열 포인터(C-domain과 호환되지 않는 자료형입니다.)
/// @warning : C언어와 규격상 호환되지 않습니다.
/// @return : 분해된 문자열 수
uint32_t STAR3_SplitText(const char * text, char splitChar, char * ptr);

/// @desc : Labview에서 벤치마킹한 함수로, 특정 문자열 앞의 문자열과 뒤의 문자열로 분리하는 함수입니다.
/// @code : 00521-012
/// @param text : 분해할 텍스트
/// @param splitchar : 기준 문자열
/// @param frontstr : 기준 문자열 앞쪽의 문자열(출력)
/// @param rearstr : 기준 문자열 뒸쪽의 문자열(출력)
/// @return : 문자열 매칭 위치
///           매칭되지 않는 경우 -1 출력
int32_t STAR3_SplitText2(const char * text, const char * splitChar, char * frontStr, char * rearStr);

/// @desc : % 연산자를 사용한 포매팅을 지원할 수 있게 설계된 스타라잇 전용 VSPrintf함수
/// @code : 00522-012
/// @param buffer : 출력할 문자열
/// @param format : 형식 문자가 포함된 문자열
/// @param args : 형식 문자에 치환될 데이터(C언어와 호환되지 않습니다.)
/// @warning : C언어와 규격상 호환되지 않습니다.
/// @return : args + argidx
uint8_t * STAR3_VSPrintf1(char * buf, const char * format, uint8_t * args);

/// @desc :스타라잇 전용 VSPrintf함수
/// @code : 00523-012
/// @param buffer : 출력할 문자열
/// @param format : 형식 문자가 포함된 문자열
/// @param args : 형식 문자에 치환될 데이터(C언어와 호환되지 않습니다.)
/// @warning : C언어와 규격상 호환되지 않습니다.
/// @return : args + argidx
uint8_t * STAR3_VSPrintf(char * buf, const char * format, uint8_t * args);

/// @desc : Arduino 전용 비트 쓰기 함수
/// @code : 스타라잇 자체 매크로 사용
/// @param var : 변수
/// @param pos : 비트 위치(LSB=0)
/// @param dat : 삽입할 값(0 또는 1)
#define bitWrite(var, pos, dat) ((var) = ((uint32_t)(var) & (~(1UL << (pos)))) | (((dat) & 1) << (pos)))

/// @desc : Arduino 전용 비트 읽기 함수
/// @code : 스타라잇 자체 매크로 사용
/// @param var : 변수
/// @param pos : 비트 위치(LSB=0)
/// @return : 비트 값
#define bitRead(var, pos) (((var)>>(pos))&1)

/// @desc : Arduino 전용 4비트 쓰기 함수
/// @code : 스타라잇 자체 매크로 사용
/// @param var : 변수
/// @param pos : 4비트 단위 위치(LSB 포함=0)
/// @param dat : 삽입할 값(0~15)
#define hexWrite(var, pos, dat) ((var) = ((uint32_t)(var) & (~(0xFUL << ((pos)<<2)))) | (((dat) & 0xF) << ((pos)<<2)))

/// @desc : Arduino 전용 4비트 읽기 함수
/// @code : 스타라잇 자체 매크로 사용
/// @param var : 변수
/// @param pos : 4비트 단위 위치(LSB 포함=0)
/// @return : 특정 위치의 4비트 값
#define hexRead(var, pos) (((var)>>((pos)<<2))&0xF)

/// @desc : Arduino 전용 8비트 쓰기 함수
/// @code : 스타라잇 자체 매크로 사용
/// @param var : 변수
/// @param pos : 8비트 단위 위치(LSB 포함=0)
/// @param dat : 삽입할 값(0~255)
#define byteWrite(var, pos, dat) ((var) = ((uint32_t)(var) & (~(0xFFUL << ((pos)<<3)))) | (((dat) & 0xFF) << ((pos)<<3)))

/// @desc : Arduino 전용 8비트 읽기 함수
/// @code : 스타라잇 자체 매크로 사용
/// @param var : 변수
/// @param pos : 8비트 단위 위치(LSB 포함=0)
/// @return : 특정 위치의 8비트 값
#define byteRead(var, pos) (((var)>>((pos)<<3))&0xFF)

#endif
