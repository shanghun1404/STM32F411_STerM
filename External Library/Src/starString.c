#include "main.h"
#include "starString.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


/// @desc : 대소문자 구분 없이 문자열 비교하는 함수
int32_t strcmp2(const char * str1, const char * str2){
	char str1_tmp[STAR_STR_LEN_MAX];
	char str2_tmp[STAR_STR_LEN_MAX];
	strcpy(str1_tmp, str1);
	strcpy(str2_tmp, str2);
	strupper(str1_tmp);
	strupper(str2_tmp);
	return strcmp(str1_tmp, str2_tmp);
}
int32_t strcmp3(const char * str1, const char * str2){
	char str1_tmp[STAR_STR_LEN_MAX];
	char str2_tmp[STAR_STR_LEN_MAX];
	strcpy(str1_tmp, str1);
	strcpy(str2_tmp, str2);
	strlower(str1_tmp);
	strlower(str2_tmp);
	return strcmp(str1_tmp, str2_tmp);
}

/// @desc : 대소문자 구분 없이 문자열 n개를 비교하는 함수
int32_t strncmp2(const char * str1, const char * str2, uint32_t n){
	char str1_tmp[STAR_STR_LEN_MAX];
	char str2_tmp[STAR_STR_LEN_MAX];
	strcpy(str1_tmp, str1);
	strcpy(str2_tmp, str2);
	strupper(str1_tmp);
	strupper(str2_tmp);
	return strncmp(str1_tmp, str2_tmp, n);
}
int32_t strncmp3(const char * str1, const char * str2, uint32_t n){
	char str1_tmp[STAR_STR_LEN_MAX];
	char str2_tmp[STAR_STR_LEN_MAX];
	strcpy(str1_tmp, str1);
	strcpy(str2_tmp, str2);
	strlower(str1_tmp);
	strlower(str2_tmp);
	return strncmp(str1_tmp, str2_tmp, n);
}

/// @desc(신설) : 문자열의 소문자를 대문자로 변경, 나머지는 그대로 둠.
void strupper(char * str1){
	for(uint32_t i = 0; str1[i]; i++){
		if(str1[i] >= 'a' && str1[i] <= 'z'){
			str1[i] -= ('a' - 'A');
		}
	}
}

/// @desc(신설) : 문자열의 대문자를 소문자로 변경, 나머지는 그대로 둠.
void strlower(char * str1){
	for(uint32_t i = 0; str1[i]; i++){
		if(str1[i] >= 'A' && str1[i] <= 'Z'){
			str1[i] += ('a' - 'A');
		}
	}
}

/// @desc : 한 문자열에 특정 위치에 문자열을 삽입합니다.
uint32_t strins(char * str1, const char * str2, uint32_t pos){
	char str[4096];
	strcpy(str, str1 + pos);
	str1[pos] = '\0';
	strcat(str1, str2);
	strcat(str1, str);
	return strlen(str1);
}

/// @desc : 한 문자열에 특정 구간을 지우고 그 위치에 다른 문자열을 삽입합니다.
uint32_t strins2(char* str, uint32_t start, uint32_t end, const char* ins) {
	char tempstr1[4096];
	char tempstr2[4096];
	strncpy(tempstr1, str, start);
	tempstr1[start] = '\0';
	strcpy(tempstr2, str + end + 1);
	strcpy(str, tempstr1);
	strcat(str, ins);
	end = strlen(str);
	strcat(str, tempstr2);
	return end;
}

/// @desc : UTF-8 기준에 따라 문자를 빼내는 함수
uint8_t strpop(char * str1, char * str2, uint32_t cursorEnd){
	uint32_t cursorStart = cursorEnd;

	//커서 위치 보정
	while((str2[cursorStart] & 0xC0) == 0x80){
		cursorStart--;
	}

	while((str2[cursorEnd] & 0xC0) == 0x80 && (str2[cursorEnd + 1] & 0xC0) == 0x80){
		cursorEnd++;
	}
	cursorEnd++;

	if(str1 != NULL){
		uint32_t i = 0;
			for(i = 0; i < cursorEnd - cursorStart; i++){
			str1[i] = str2[cursorStart + i];
		}
		str1[i] = '\0';
	}

	str2[cursorStart] = '\0';
	strcat(str2, str2 + cursorEnd);
	return cursorEnd - cursorStart;
}

/// @desc : 감춰야 할 문자가 있을 경우 감춤. 예: 1234->****
uint32_t strmsk(char * strm, const char * str, char ch){
	uint32_t i = 0;
	uint32_t len = strlen(str);
	for(i = 0; i < len; i++){
		strm[i] = ch;
	}
	strm[i] = '\0';
	return len;
}

/// @desc : 글자 개수 세기, 예: 가나다의 경우 3 출력
uint32_t strletlen(const char * str){
	uint32_t cnt = 0;
	for(uint32_t i = 0; str[i]; i++){
		if(!(str[i] & 0x80)){
			cnt++;
		}
		else if((str[i] & 0xE0) == 0xC0){
			i++;
			cnt++;
		}
		else if((str[i] & 0xF0) == 0xE0){
			i+=2;
			cnt++;
		}
		else if((str[i] & 0xF1) == 0xF0){
			i+=3;
			cnt++;
		}
	}
	return cnt;
}

/// @desc : 유니코드 기준 바이트 수 세기, 예: 가나다의 경우 6 출력
uint32_t strunilen(const char * str){
	uint32_t cnt = 0;
	for(uint32_t i = 0; str[i]; i++){
		if(!(str[i] & 0x80)){
			cnt++;
		}
		else if((str[i] & 0xE0) == 0xC0){
			if((str[i] & 0xFC) == 0xE0){
				i++;
				cnt++;
			}
			else{
				i++;
				cnt+=2;
			}
		}
		else if((str[i] & 0xF0) == 0xE0){
			i+=2;
			cnt+=2;
		}
		else if((str[i] & 0xF1) == 0xF0){
			i+=3;
			cnt+=3;
		}
	}
	return cnt;
}

/// @desc : UTF-8 포맷으로 인코딩된 문자를 문자열 형식으로 입력받아 유니코드 값으로 반환하는 함수
uint32_t UTF8_Val(const char * str, uint8_t * size_char){
	if(str[0] & 0b10000000){
		if(str[0] & 0b01000000){
			if(str[0] & 0b00100000){
				*size_char = 3;
				return ((str[0]&0b1111)<<12) | ((str[1]&0b111111)<<6) | (str[2]&0b111111);
			}
			else{
				*size_char = 2;
				return ((str[0]&0b11111)<<6) | (str[1]&0b111111);
			}
		}
		else{//잘못된 접근, 따라서 size_char과 안맞는게 있을 수 있다.
			*size_char = 1;
			return UTF8_Val(str - 1, size_char);
		}
	}
	else{
		*size_char = 1;
		return str[0];
	}
}


/// @desc : 유니코드를 입력하면 유니코드 형태에 맞게 변형해서 문자열 형식으로 출력하는 함수
uint8_t UTF8_String(char * str, uint16_t unicode){
	if((unicode & 0xFF80) == 0){
		str[0] = unicode;
		str[1] = '\0';
		return 1;
	}
	else if((unicode & 0xF800) == 0){
		str[0] = ((unicode >> 6) & 0x1F) | 0xC0;
		str[1] = (unicode & 0x3F) | 0x80;
		str[2] = '\0';
		return 2;
	}
	else{
		str[0] = ((unicode >> 12) & 0x0F) | 0xE0;
		str[1] = ((unicode >> 6) & 0x3F) | 0x80;
		str[2] = (unicode & 0x3F) | 0x80;
		str[3] = '\0';
		return 3;
	}
}

/// @desc : 16진수를 정수형으로 변경하는 함수
uint32_t xtoi(const char * str){
	uint16_t i = 0;
	uint32_t val = 0;//16진수는 음수를 취급하지 않으므로 unsigned형 사용.
	//0x와 같은 Prefix는 무시합니다.
	for(i = 0; str[i]; i++){
		if(str[i] >= '0' && str[i] <= '9'){
			val <<= 4;
			val |= str[i] - '0';
		}
		else if(str[i] >= 'A' && str[i] <= 'F'){
			val <<= 4;
			val |= str[i] - 'A' + 10;
		}
		else if(str[i] >= 'a' && str[i] <= 'f'){
			val <<= 4;
			val |= str[i] - 'a' + 10;
		}
	}
	return val;
}

/// @desc : 16진수를 64비트 정수형으로 변경하는 함수
uint64_t xtoll(const char * str){
	uint16_t i = 0;
	uint64_t val = 0;//16진수는 음수를 취급하지 않으므로 unsigned형 사용.
	//0x와 같은 Prefix는 무시합니다.
	for(i = 0; str[i]; i++){
		if(str[i] >= '0' && str[i] <= '9'){
			val <<= 4;
			val |= str[i] - '0';
		}
		else if(str[i] >= 'A' && str[i] <= 'F'){
			val <<= 4;
			val |= str[i] - 'A' + 10;
		}
		else if(str[i] >= 'a' && str[i] <= 'f'){
			val <<= 4;
			val |= str[i] - 'a' + 10;
		}
	}
	return val;
}

/// @desc : 문자열 포매팅 함수(sprintf 함수가 오작동하는 경우 이 함수 사용하세요.)
void STAR3_Sprintf(char * buffer, const char * format, ...){
	/*
	 * buffer를 많이 할당받거나 static으로 선언하면 좋겠지만, 보통 프로세스당 4GB의 메모리를 가지는 일반 컴퓨터와 다르게
	 * STM32F411의 경우 전체 메모리 공간이 오직 128kB의 저장용량만 가지므로 메모리 공간을 아껴 써야 한다.
	 */
//	char buffer[512] = { 0 };

	// 가변 인자를 받아서 buffer에 쓴다.
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);


}

/// @desc : 문자열을 특정 문자를 사용해 분해하는 함수. 스타라잇 내에서 문자열 배열을 정의한 후 사용 가능한 Starlit 전용 함수입니다.
uint32_t STAR3_SplitText(const char * text, char splitChar, char * ptr){
	uint32_t j = 0, k=0;
	for(uint32_t i = 0; ; i++){
		if(text[i] == splitChar || text[i] == 0){
			ptr[j * 256 + k] = 0;
			j++;
			k = 0;
			if(text[i] == 0) break;
		}
		else{
			ptr[j * 256 + k] = text[i];
			k++;
		}
	}

	return j;
}

/// @desc : Labview에서 벤치마킹한 함수로, 특정 문자열 앞의 문자열과 뒤의 문자열로 분리하는 함수입니다.
int32_t STAR3_SplitText2(const char * text, const char * splitChar, char * frontStr, char * rearStr){
	//LabView의 문자열 분리 함수를 Starlight화해서 가져왔습니다.
	//단, 예외 객체는 따로 출력하지는 않겠습니다.
	for(uint32_t i = 0; text[i]; i++){
		if(!strncmp(text + i, splitChar, strlen(splitChar))){
			//앞 문자는 text에서 i번째 전까지이다.
			//strncpy를 이용해 복사해 준다.
			strncpy(frontStr, text, i);
			frontStr[i] = 0;

			//뒷 문자는 text에서 i+strlen번째 까지이다.
			strcpy(rearStr, text + i + strlen(splitChar));

			return i;
		}
	}

	//문자열을 나누지 못한 경우 -1을 출력.
	return -1;
}

/// @desc : % 연산자를 사용한 포매팅을 지원할 수 있게 설계된 스타라잇 전용 VSPrintf함수
uint8_t * STAR3_VSPrintf1(char * buf, const char * format, uint8_t * args){
	//%가 2번째로 나올 때까지 잡는다.
	uint32_t percent = 0;
	uint32_t i = 0;
	char st2[256];
	//%가 1번만 들어가게 String을 자른다.
	for (; format[i]; i++) {
		if (format[i] == '%') {
			if(format[i + 1] == '%'){
				i++;
				continue;
			}
			percent++;
			if (percent == 2) {
				break;
			}
		}
	}

	strncpy(st2, format, i);
	st2[i] = '\0';

	uint8_t * result;
	result = STAR3_VSPrintf(buf, st2, args);
	strcat(buf, format + i);
	return result;
}

char * ordinaryInt(int32_t val, uint32_t cap){
	val = val > 0 ? val : -val;
	uint32_t one = val % 10;
	uint32_t teen = (val % 100) >= 10 && (val % 100) <= 19;

	if(!teen && one == 1){
		if(cap) return "ST";
		return "st";
	}
	else if(!teen && one == 2){
		if(cap) return "ND";
		return "nd";
	}
	else if(!teen && one == 3){
		if(cap) return "RD";
		return "rd";
	}
	else{
		if(cap) return "TH";
		return "th";
	}
}

char * ordinarylong(int64_t val, uint32_t cap){
	val = val > 0 ? val : -val;
	uint32_t one = val % 10;
	uint32_t teen = (val % 100) >= 10 && (val % 100) <= 19;

	if(!teen && one == 1){
		if(cap) return "ST";
		return "st";
	}
	else if(!teen && one == 2){
		if(cap) return "ND";
		return "nd";
	}
	else if(!teen && one == 3){
		if(cap) return "RD";
		return "rd";
	}
	else{
		if(cap) return "TH";
		return "th";
	}
}



/// @desc :스타라잇 전용 VSPrintf함수
uint8_t * STAR3_VSPrintf(char * buf, const char * format, uint8_t * args){
	uint32_t percent = 0;
	char str[256];
	uint32_t j = 0;
	strcpy(buf, "");
	char result[256];
	uint32_t argidx = 0;
	uint32_t ppos = 0;
	for(uint32_t i = 0; format[i]; i++){
		if(format[i] == '%' && percent == 0){
			percent = 1;
			ppos = j;
			str[j++] = format[i];
			str[j] = 0;
		}
		else if(percent == 1 && format[i] == '%'){
			percent = 0;//%%인 경우 %만 출력.
			strcat(buf, str);
			j = 0;
			str[j] = 0;
		}
		else if(percent == 1 && format[i] == 'b'){
			percent = 0;
			if(format[i - 1] == 'l' && format[i - 2] == 'l'){
				uint8_t arg[8];
				for(uint32_t i = 0; i < 8; i++) arg[i] = args[argidx++];
				str[j++] = format[i];
				str[j] = 0;
				uint32_t zero = 0;
				uint32_t sharp = 0;
				int32_t dg = 0;
				uint32_t dg1 = 0;
				char temp[256];
				uint64_t val = (*((uint64_t *)arg));
				for(dg = 63; dg>=0; dg--){
					if((val >> dg) & 0x01) break;
				}
				dg+=1;


				uint32_t u = ppos;
				if(str[ppos + 1] == '#'){
					sharp = 1;
					u = 2;
					if(str[u] == '0') {zero = 1; u = 3;}
				}
				else{
					u = 1;
					if(str[u] == '0') {zero = 1; u = 2;}
				}
				for(; str[u]; u++){
					if(str[u] >= '0' && str[u] <= '9'){
						dg1 *= 10;
						dg1 += str[u] - '0';
					}
					else{
						break;
					}
				}

				strcpy(result, "");
				if(zero == 0){
					if(dg1 > dg + 2 * sharp){
						for(uint32_t i = 0; i < dg1 - dg - 2 * sharp; i++){
							strcat(result, " ");
						}
					}
					if(sharp){
						strcat(result, "0b");
					}
					for(int32_t x = dg - 1; x >= 0; x--){
						sprintf(temp, "%d", (int)((val >> x) & 0x01));
						strcat(result, temp);
					}
				}
				else{
					if(sharp){
						strcat(result, "0b");
					}
					for(int32_t x = dg1 > dg + 2 * sharp ? dg1 - 2 * sharp - 1 : dg - 1; x>=0;x--){
						sprintf(temp, "%d", (int)((val >> x) & 0x01));
						strcat(result, temp);
					}
				}


				uint32_t ln = strlen(buf);
				strncat(buf, str, ppos);
				buf[ln + ppos] = 0;

				strcat(buf, result);
				j = 0;
				str[j] = 0;
			}

			else if(format[i - 1] == 'h'){
				uint8_t arg[8];
				for(uint32_t i = 0; i < 2; i++) arg[i] = args[argidx++];
				str[j++] = format[i];
				str[j] = 0;
				uint32_t zero = 0;
				uint32_t sharp = 0;
				int32_t dg = 0;
				uint32_t dg1 = 0;
				char temp[256];
				uint16_t val = (*((uint16_t *)arg));
				for(dg = 15; dg>=0; dg--){
					if((val >> dg) & 0x01) break;
				}
				dg+=1;


				uint32_t u = ppos;
				if(str[ppos + 1] == '#'){
					sharp = 1;
					u = 2;
					if(str[u] == '0') {zero = 1; u = 3;}
				}
				else{
					u = 1;
					if(str[u] == '0') {zero = 1; u = 2;}
				}
				for(; str[u]; u++){
					if(str[u] >= '0' && str[u] <= '9'){
						dg1 *= 10;
						dg1 += str[u] - '0';
					}
					else{
						break;
					}
				}

				strcpy(result, "");
				if(zero == 0){
					if(dg1 > dg + 2 * sharp){
						for(uint32_t i = 0; i < dg1 - dg - 2 * sharp; i++){
							strcat(result, " ");
						}
					}
					if(sharp){
						strcat(result, "0b");
					}
					for(int32_t x = dg - 1; x >= 0; x--){
						sprintf(temp, "%d", (int)((val >> x) & 0x01));
						strcat(result, temp);
					}
				}
				else{
					if(sharp){
						strcat(result, "0b");
					}
					for(int32_t x = dg1 > dg + 2 * sharp ? dg1 - 2 * sharp - 1 : dg - 1; x>=0;x--){
						sprintf(temp, "%d", (int)((val >> x) & 0x01));
						strcat(result, temp);
					}
				}


				uint32_t ln = strlen(buf);
				strncat(buf, str, ppos);
				buf[ln + ppos] = 0;

				strcat(buf, result);
				j = 0;
				str[j] = 0;
			}

			else {
				uint8_t arg[8];
				for(uint32_t i = 0; i < 4; i++) arg[i] = args[argidx++];
				str[j++] = format[i];
				str[j] = 0;
				uint32_t zero = 0;
				uint32_t sharp = 0;
				int32_t dg = 0;
				uint32_t dg1 = 0;
				char temp[256];
				uint32_t val = (*((uint32_t *)arg));
				for(dg = 31; dg>=0; dg--){
					if((val >> dg) & 0x01) break;
				}
				dg+=1;


				uint32_t u = ppos;
				if(str[ppos + 1] == '#'){
					sharp = 1;
					u = 2;
					if(str[u] == '0') {zero = 1; u = 3;}
				}
				else{
					u = 1;
					if(str[u] == '0') {zero = 1; u = 2;}
				}
				for(; str[u]; u++){
					if(str[u] >= '0' && str[u] <= '9'){
						dg1 *= 10;
						dg1 += str[u] - '0';
					}
					else{
						break;
					}
				}

				strcpy(result, "");
				if(zero == 0){
					if(dg1 > dg + 2 * sharp){
						for(uint32_t i = 0; i < dg1 - dg - 2 * sharp; i++){
							strcat(result, " ");
						}
					}
					if(sharp){
						strcat(result, "0b");
					}
					for(int32_t x = dg - 1; x >= 0; x--){
						sprintf(temp, "%d", (int)((val >> x) & 0x01));
						strcat(result, temp);
					}
				}
				else{
					if(sharp){
						strcat(result, "0b");
					}
					for(int32_t x = dg1 > dg + 2 * sharp ? dg1 - 2 * sharp - 1 : dg - 1; x>=0;x--){
						sprintf(temp, "%d", (int)((val >> x) & 0x01));
						strcat(result, temp);
					}
				}

				uint32_t ln = strlen(buf);
				strncat(buf, str, ppos);
				buf[ln + ppos] = 0;
				strcat(buf, result);
				j = 0;
				str[j] = 0;
			}

		}
		else if(percent == 1 && (format[i] == 'd' || format[i] == 'D' || format[i] == 'o' || format[i] == 'x' || format[i] == 'X' || format[i] == 'i' || format[i] == 'u')){
			percent = 0;
			//정수인 경우
			if(format[i - 1] == 'l' && format[i - 2] == 'l'){
				uint8_t arg[8];
				for(uint32_t i = 0; i < 8; i++) arg[i] = args[argidx++];
				str[j++] = format[i];
				str[j] = 0;
				//int64형인 경우
				if(format[i] == 'o' && str[ppos + 1] == '#'){
					uint32_t dg = 0;
					int32_t dg1 = 0;
					uint32_t u = ppos + 2;
					uint32_t zero = 0;
					uint64_t val = (*((uint64_t *)arg));
					char tempstr[256];
					char tempstr2[256];
					if(str[u] == '0'){
						strcpy(tempstr, "0o%0");
						u++;
						zero = 1;
					}
					for(; str[u]; u++){
						if(str[u] >= '0' && str[u] <= '9'){
							dg *= 10;
							dg += str[u] - '0';
						}
						else{
							break;
						}
					}
					if(zero){
						sprintf(tempstr2, "%d", (int)dg-2);
						strcat(tempstr, tempstr2);
						strcat(tempstr, str + u);
					}
					else{
						for(dg1 = 21; dg1>=0; dg1--){
							if((val >> (dg1 * 3)) & 0x07) break;
						}
						dg1 += 1;
						strcpy(tempstr, "");
						for(int32_t x = 0; x < (int)dg - dg1 - 2; x++){
							strcat(tempstr, " ");
						}
						strcat(tempstr, "0o%o");
					}
					strcpy(str + ppos, tempstr);
				}
				else if((format[i] == 'd' || format[i] == 'D') && str[ppos + 1] == '#'){
					uint32_t num = 0;
					uint32_t zero = 0;
					uint32_t plus = 0;
					if(str[ppos + 2] == '+' || str[ppos + 2] == '-'){
						plus = 1;
					}

					if(str[ppos + 2 + plus] == '0'){
						zero = 1;
					}

					if(str[ppos + 2 + zero + plus] >= '1' && str[ppos + 2 + zero + plus] <= '9'){
						for(uint32_t i = 0; str[ppos + 2 + zero + i + plus] >= '0' && str[ppos + 2 + zero + i + plus] <= '9'; i++){
							num *= 10;
							num += str[ppos + 2 + zero + i + plus] - '0';
						}
					}
					char tempstr[256];
					strcpy(tempstr, "%");
					if(plus) {
						uint32_t ln = strlen(tempstr);
						tempstr[ln++] = str[ppos + 2];
						tempstr[ln] = 0;
					}
					if(zero) strcat(tempstr, "0");

					if(num > 2) {
						char tempstr2[256];
						sprintf(tempstr2, "%d", (int)num-2);
						strcat(tempstr, tempstr2);
					}
					strcat(tempstr, "d");
					strcat(tempstr, ordinarylong(*((int64_t *)arg), format[i] == 'D'));

					strcpy(str + ppos, tempstr);

				}
				sprintf(result, str, *((uint64_t *)arg));
				strcat(buf, result);
				j = 0;
				str[j] = 0;
			}
			else if(format[i - 1] == 'h'){
				uint8_t arg[2];
				for(uint32_t i = 0; i < 2; i++) arg[i] = args[argidx++];
				str[j++] = format[i];
				str[j] = 0;
				//int64형인 경우

				if(format[i] == 'o' && str[ppos + 1] == '#'){
					uint32_t dg = 0;
					int32_t dg1 = 0;
					uint32_t u = ppos + 2;
					uint32_t zero = 0;
					uint16_t val = (*((uint16_t *)arg));
					char tempstr[256];
					char tempstr2[256];
					if(str[u] == '0'){
						strcpy(tempstr, "0o%0");
						u++;
						zero = 1;
					}
					for(; str[u]; u++){
						if(str[u] >= '0' && str[u] <= '9'){
							dg *= 10;
							dg += str[u] - '0';
						}
						else{
							break;
						}
					}
					if(zero){
						sprintf(tempstr2, "%d", (int)dg-2);
						strcat(tempstr, tempstr2);
						strcat(tempstr, str + u);
					}
					else{
						for(dg1 = 5; dg1>=0; dg1--){
							if((val >> (dg1 * 3)) & 0x07) break;
						}
						dg1 += 1;
						strcpy(tempstr, "");
						for(int32_t x = 0; x < (int)dg - dg1 - 2; x++){
							strcat(tempstr, " ");
						}
						strcat(tempstr, "0o%o");
					}
					strcpy(str + ppos, tempstr);
				}
				else if((format[i] == 'd' || format[i] == 'D') && str[ppos + 1] == '#'){
					uint32_t num = 0;
					uint32_t zero = 0;
					uint32_t plus = 0;
					if(str[ppos + 2] == '+' || str[ppos + 2] == '-'){
						plus = 1;
					}

					if(str[ppos + 2 + plus] == '0'){
						zero = 1;
					}

					if(str[ppos + 2 + zero + plus] >= '1' && str[ppos + 2 + zero + plus] <= '9'){
						for(uint32_t i = 0; str[ppos + 2 + zero + i + plus] >= '0' && str[ppos + 2 + zero + i + plus] <= '9'; i++){
							num *= 10;
							num += str[ppos + 2 + zero + i + plus] - '0';
						}
					}
					char tempstr[256];
					strcpy(tempstr, "%");
					if(plus) {
						uint32_t ln = strlen(tempstr);
						tempstr[ln++] = str[ppos + 2];
						tempstr[ln] = 0;
					}
					if(zero) strcat(tempstr, "0");

					if(num > 2) {
						char tempstr2[256];
						sprintf(tempstr2, "%d", (int)num-2);
						strcat(tempstr, tempstr2);
					}
					strcat(tempstr, "d");
					strcat(tempstr, ordinaryInt(*((int16_t *)arg), format[i] == 'D'));

					strcpy(str + ppos, tempstr);

				}
				sprintf(result, str, *((uint16_t *)arg));
				strcat(buf, result);
				j = 0;
				str[j] = 0;
			}
			else{
				uint8_t arg[4];
				for(uint32_t i = 0; i < 4; i++) arg[i] = args[argidx++];
				str[j++] = format[i];
				str[j] = 0;
				//int64형인 경우

				if(format[i] == 'o' && str[ppos + 1] == '#'){
					uint32_t dg = 0;
					int32_t dg1 = 0;
					uint32_t u = ppos + 2;
					uint32_t zero = 0;
					uint32_t val = (*((uint32_t *)arg));
					char tempstr[256];
					char tempstr2[256];
					if(str[u] == '0'){
						strcpy(tempstr, "0o%0");
						u++;
						zero = 1;
					}
					for(; str[u]; u++){
						if(str[u] >= '0' && str[u] <= '9'){
							dg *= 10;
							dg += str[u] - '0';
						}
						else{
							break;
						}
					}
					if(zero){
						sprintf(tempstr2, "%d", (int)dg-2);
						strcat(tempstr, tempstr2);
						strcat(tempstr, str + u);
					}
					else{
						for(dg1 = 10; dg1>=0; dg1--){
							if((val >> (dg1 * 3)) & 0x07) break;
						}
						dg1 += 1;
						strcpy(tempstr, "");
						for(int32_t x = 0; x < (int)dg - dg1 - 2; x++){
							strcat(tempstr, " ");
						}
						strcat(tempstr, "0o%o");
					}
					strcpy(str + ppos, tempstr);
				}
				else if((format[i] == 'd' || format[i] == 'D') && str[ppos + 1] == '#'){
					uint32_t num = 0;
					uint32_t zero = 0;
					uint32_t plus = 0;
					if(str[ppos + 2] == '+' || str[ppos + 2] == '-'){
						plus = 1;
					}

					if(str[ppos + 2 + plus] == '0'){
						zero = 1;
					}

					if(str[ppos + 2 + zero + plus] >= '1' && str[ppos + 2 + zero + plus] <= '9'){
						for(uint32_t i = 0; str[ppos + 2 + zero + i + plus] >= '0' && str[ppos + 2 + zero + i + plus] <= '9'; i++){
							num *= 10;
							num += str[ppos + 2 + zero + i + plus] - '0';
						}
					}
					char tempstr[256];
					strcpy(tempstr, "%");
					if(plus) {
						uint32_t ln = strlen(tempstr);
						tempstr[ln++] = str[ppos + 2];
						tempstr[ln] = 0;
					}
					if(zero) strcat(tempstr, "0");

					if(num > 2) {
						char tempstr2[256];
						sprintf(tempstr2, "%d", (int)num-2);
						strcat(tempstr, tempstr2);
					}
					strcat(tempstr, "d");
					strcat(tempstr, ordinaryInt(*((int32_t *)arg), format[i] == 'D'));

					strcpy(str + ppos, tempstr);

				}
				sprintf(result, str, *((uint32_t *)arg));
				strcat(buf, result);
				j = 0;
				str[j] = 0;
			}
		}
		else if(percent == 1 && (format[i] == 'f' || format[i] == 'e' || format[i] == 'E' || format[i] == 'g' || format[i] == 'G' || format[i] == 'm' || format[i] == 'M')){
			percent = 0;
			//소수인 경우
			if(format[i - 1] == 'l' && format[i - 2] == 'l' && format[i] != 'm'){//임베디드에서는 long double 무효
				uint8_t arg[8];
				for(uint32_t i = 0; i < 8; i++) arg[i] = args[argidx++];
				str[j++] = format[i];
				str[j] = 0;
				//int64형인 경우
				sprintf(result, str, *((double *)arg));
				strcat(buf, result);
				j = 0;
				str[j] = 0;
			}
			else if(format[i - 1] == 'l' && format[i] != 'm'){
				uint8_t arg[8];
				for(uint32_t i = 0; i < 8; i++) arg[i] = args[argidx++];
				str[j++] = format[i];
				str[j] = 0;
				//int64형인 경우
				sprintf(result, str, *((double *)arg));
				strcat(buf, result);
				j = 0;
				str[j] = 0;
			}
			else if (format[i] == 'm' || format[i] == 'M') {
				uint8_t arg[4];
				for (uint32_t i = 0; i < 4; i++) arg[i] = args[argidx++];
				float a = (*((float*)arg));
				char units[] = { 'y', 'z', 'a', 'f', 'p', 'n', 'u', 'm', ' ', 'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y' };
				uint32_t r = 0;
				//10의 승수 구하기(반드시 유효숫자는 1보다 크게 한다.
				if (a >= 1.00 && a < 1000.00) {
					if(format[i-1] == ' '){j--;}
					//그냥 출력.
					str[j++] = 'f';
					if (format[i] == 'M') { str[j++] = ' '; }
					if(format[i-1] == ' '){ str[j++] = ' '; }
					str[j] = 0;
					r = 1;
				}
				else if (fabsf(a) >= 1000.00) {
					//a가 1보다 크고 1000보다 작은 위치일 때까지 계속 나눈다.
					uint32_t k = 0;
					while (fabsf(a) >= 1000.00 && k <= 8) {
						a /= 1000.00;
						k++;
					}
					if(format[i-1] == ' '){j--;}
					str[j++] = 'f';
					if(format[i-1] == ' '){ str[j++] = ' '; }
					str[j++] = units[8 + k];
					str[j] = 0;
				}
				else if (fabsf(a) < 1.00) {
					//a가 1보다 크고 1000보다 작은 위치일 때까지 계속 나눈다.
					uint32_t k = 0;
					while (fabsf(a) < 1.00 && k <= 8) {
						a *= 1000.00;
						k++;
					}
					if(format[i-1] == ' '){j--;}
					str[j++] = 'f';
					if(format[i-1] == ' '){ str[j++] = ' '; }
					str[j++] = units[8 - k];
					str[j] = 0;
				}

				//실질적 출력 자리수 줄이기
				uint32_t sp = 0;
				if(format[i-1] == ' '){sp++;}
				if(r == 0 || format[i] == 'M'){sp++;}
				uint32_t dg = 0;
				for(uint32_t u = 1; str[u]; u++){
					if(!(u == 1 && str[u] == '0') && str[u] >= '0' && str[u] <= '9'){
						dg *= 10;
						dg += str[u] - '0';
					}
					else if(str[u] == '.' && u != 1){
						if(str[ppos + 1] == '0'){
							char tempstr[256];
							strcpy(tempstr, str + u);
							sprintf(str + 2, "%u", (unsigned int)(dg - sp > 1 ? dg - sp : 1));
							strcat(str, tempstr);
						}
						else{
							char tempstr[256];
							strcpy(tempstr, str + u);
							sprintf(str + 1, "%u", (unsigned int)(dg - sp > 1 ? dg - sp : 1));
							strcat(str, tempstr);
						}
					}
				}


				sprintf(result, str, a);


				strcat(buf, result);
				j = 0;
				str[j] = 0;

			}
			else{
				uint8_t arg[4];
				for(uint32_t i = 0; i < 4; i++) arg[i] = args[argidx++];
				str[j++] = format[i];
				str[j] = 0;
				//int64형인 경우
				sprintf(result, str, *((float *)arg));
				strcat(buf, result);
				j = 0;
				str[j] = 0;
			}
		}
		else if(percent == 1 && (format[i] == 'c')){
			percent = 0;
			uint8_t arg[1];
			for(uint32_t i = 0; i < 1; i++) arg[i] = args[argidx++];
			str[j++] = format[i];
			str[j] = 0;
			//int64형인 경우
			sprintf(result, str, *((char *)arg));
			strcat(buf, result);
			j = 0;
			str[j] = 0;
		}
		else if(percent == 1 && (format[i] == 'p' || format[i] == 's')){
			percent = 0;
			uint8_t arg[4];
			for(uint32_t i = 0; i < 4; i++) arg[i] = args[argidx++];
			str[j++] = format[i];
			str[j] = 0;
			//int64형인 경우
			sprintf(result, str, *((char **)arg));
			strcat(buf, result);
			j = 0;
			str[j] = 0;
		}
		else if(percent == 1 && (format[i] == 'j' || format[i] == 'I' || format[i] == 'J')){
			//%.3+.3j -> 기본적으로 a+bj꼴로 출력
			//%.3+!.3j -> 기본적으로 a+jb꼴로 출력, ! 위치에 j로 변경
			percent = 0;
			uint8_t arg[8];
			for(uint32_t i = 0; i < 8; i++) arg[i] = args[argidx++];
			str[j++] = format[i];
			str[j] = 0;
			//% 직전까지 전부 출력
			uint32_t p = 0;
			for(p = 0; str[p] != '%'; p++);
			uint32_t r = strlen(buf);
			strncat(buf, str, p);
			buf[r + p] = 0;

			float re, im;
			int sign = 0;
			re = *((float *)arg);
			im = *((float *)(arg + 4));
			sign = im < 0.0f;

			//기본형
			if(str[p + 1] == 'j' || str[p + 1] == 'J' || str[p + 1] == 'I'){
				char str1[256];
				sprintf(str1, "%f", re);
				strcat(buf, str1);
				if(sign){
					strcat(buf, "-");
				}
				else{
					strcat(buf, "+");
				}
				sprintf(str1, "%f", fabsf(im));
				strcat(buf, str1);
				if(str[p+1] == 'I'){
					strcat(buf, "i");
				}
				else{
					strcat(buf, "j");
				}
			}
			else{
				char fmt1[256];
				char fmt2[256];
				fmt1[0] = '%';
				fmt1[1] = '\0';
				fmt2[0] = '%';
				fmt2[1] = '\0';
				uint32_t state = 0;
				uint32_t u = 1;
				for(p++;str[p];p++){
					if(str[p] == '+'){
						//fmt1[u] = 0;
						state = 1;
						if(str[p + 1] == '+'){
							p++;
							state = 2;
						}
						u = 1;
					}
					else if(str[p] == 'j' || str[p] == 'J' || str[p] == 'I'){
						strcat(fmt1, "f");
						strcat(fmt2, "f");
						if(state == 0){
							char str1[256];
							sprintf(str1, fmt1, re);
							strcat(buf, str1);
							if(sign){
								strcat(buf, "-");
							}
							else{
								strcat(buf, "+");
							}
							sprintf(str1, fmt1, fabsf(im));
							strcat(buf, str1);
							strcat(buf, str[p] == 'I' ? "i" : "j");
						}
						else if(state == 1){
							char str1[256];
							sprintf(str1, fmt1, re);
							strcat(buf, str1);
							if(sign){
								strcat(buf, "-");
							}
							else{
								strcat(buf, "+");
							}
							sprintf(str1, fmt2, fabsf(im));
							strcat(buf, str1);
							strcat(buf, str[p] == 'I' ? "i" : "j");
						}
						else if(state == 2){
							char str1[256];
							sprintf(str1, fmt1, re);
							strcat(buf, str1);
							if(sign){
								strcat(buf, str[p] == 'I' ? "-i" : "-j");
							}
							else{
								strcat(buf, str[p] == 'I' ? "+i" : "+j");
							}
							sprintf(str1, fmt2, fabsf(im));
							strcat(buf, str1);
						}
					}
					else{
						if(state){
							fmt2[u++] = str[p];
							fmt2[u] = 0;
						}
						else{
							fmt1[u++] = str[p];
							fmt1[u] = 0;
						}
					}
				}

			}


			//strcat(buf, result);
			j = 0;
			str[j] = 0;
		}
		else{
			str[j++] = format[i];
			str[j] = 0;
		}



	}
	strcat(buf, str);

	return args + argidx;
}

