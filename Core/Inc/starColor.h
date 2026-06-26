#ifndef __006__STAR__COLOR__H__
#define __006__STAR__COLOR__H__

/// Part 1. OLED/LED/PuTTY 글자색 설정 구간

/// @desc : 컬러 팔레트 배열 포인터 출력 함수.
/// @code : 00600-012
/// @param palleteIdx : 0 : LED
///                     1 : LED(Dark)
///                     2 : OLED
///                     3 : OLED(Dark)
///                     4 : PuTTY
///                     5 : PuTTY(Dark)
/// @return : 팔레트 배열 포인터
#ifndef __STAR__COLOR__C__
extern const char COLOR_keys[];
extern const uint32_t * COLOR_palletes[];
#define colorPallete(palleteIdx) (COLOR_palletes[palleteIdx])

extern const uint32_t LED_colors[];
extern const uint32_t LED_darkcolors[];
extern const uint32_t OLED_colors[];
extern const uint32_t OLED_darkcolors[];
extern const uint32_t PuTTY_colors[];
extern const uint32_t PuTTY_darkcolors[];
#endif

/// @desc : 컬러 팔레트 입력하여 색상 출력하는 함수
/// @code : 00601-012
/// @param key : 색상을 표현하는 문자열, r, o, y, l, g, t, c, s, b, v, m, p, w, k 중 어느 하나.
/// @pallete : 팔레트 배열 포인터
/// @return : 색상 코드(HEX)
///           키를 잘못 입력한 경우 -1 출력.
int32_t FindColor(char key, const uint32_t * pallete);

/// LED Color(Linear)
#define COLOR_LED_LIN_R 0xFF0000
#define COLOR_LED_LIN_O 0xFF4000
#define COLOR_LED_LIN_Y 0xFFFF00
#define COLOR_LED_LIN_L 0x40FF00
#define COLOR_LED_LIN_G 0x00FF00
#define COLOR_LED_LIN_T 0x00FF40
#define COLOR_LED_LIN_C 0x00FFFF
#define COLOR_LED_LIN_S 0x0040FF
#define COLOR_LED_LIN_B 0x0000FF
#define COLOR_LED_LIN_V 0x4000FF
#define COLOR_LED_LIN_M 0xFF00FF
#define COLOR_LED_LIN_P 0xFF0040
#define COLOR_LED_LIN_W 0xFFFFFF
#define COLOR_LED_LIN_K 0x000000

#define COLOR_LED_LIN_R1 0xFF4040
#define COLOR_LED_LIN_O1 0xFF8040
#define COLOR_LED_LIN_Y1 0xFFFF40
#define COLOR_LED_LIN_L1 0x80FF40
#define COLOR_LED_LIN_G1 0x40FF40
#define COLOR_LED_LIN_T1 0x40FF80
#define COLOR_LED_LIN_C1 0x40FFFF
#define COLOR_LED_LIN_S1 0x4080FF
#define COLOR_LED_LIN_B1 0x4040FF
#define COLOR_LED_LIN_V1 0x8040FF
#define COLOR_LED_LIN_M1 0xFF40FF
#define COLOR_LED_LIN_P1 0xFF4080
#define COLOR_LED_LIN_W1 0x808080
#define COLOR_LED_LIN_K1 0x404040

#define COLOR_LED_LIN_DR 0x400000
#define COLOR_LED_LIN_DO 0x401000
#define COLOR_LED_LIN_DY 0x404000
#define COLOR_LED_LIN_DL 0x104000
#define COLOR_LED_LIN_DG 0x004000
#define COLOR_LED_LIN_DT 0x004010
#define COLOR_LED_LIN_DC 0x004040
#define COLOR_LED_LIN_DS 0x001040
#define COLOR_LED_LIN_DB 0x000040
#define COLOR_LED_LIN_DV 0x100040
#define COLOR_LED_LIN_DM 0x400040
#define COLOR_LED_LIN_DP 0x400010
#define COLOR_LED_LIN_DW 0x404040
#define COLOR_LED_LIN_DK 0x000000

#define COLOR_LED_LIN_DR1 0x401010
#define COLOR_LED_LIN_DO1 0x402010
#define COLOR_LED_LIN_DY1 0x404010
#define COLOR_LED_LIN_DL1 0x204010
#define COLOR_LED_LIN_DG1 0x104010
#define COLOR_LED_LIN_DT1 0x104020
#define COLOR_LED_LIN_DC1 0x104040
#define COLOR_LED_LIN_DS1 0x102040
#define COLOR_LED_LIN_DB1 0x101040
#define COLOR_LED_LIN_DV1 0x201040
#define COLOR_LED_LIN_DM1 0x401040
#define COLOR_LED_LIN_DP1 0x401020
#define COLOR_LED_LIN_DW1 0x202020
#define COLOR_LED_LIN_DK1 0x101010


#ifndef __STAR__COLOR__C__
#define LED_findColor(key) FindColor(key, LED_colors)
#define LED_findDarkColor(key) FindColor(key, LED_darkcolors)
#endif


/// OLED Color(Linear)
#define COLOR_OLED_LIN_R 0xFF0000
#define COLOR_OLED_LIN_O 0xFF8000
#define COLOR_OLED_LIN_Y 0xFFFF00
#define COLOR_OLED_LIN_L 0x80FF00
#define COLOR_OLED_LIN_G 0x00FF00
#define COLOR_OLED_LIN_T 0x00FF80
#define COLOR_OLED_LIN_C 0x00FFFF
#define COLOR_OLED_LIN_S 0x0080FF
#define COLOR_OLED_LIN_B 0x0000FF
#define COLOR_OLED_LIN_V 0x8000FF
#define COLOR_OLED_LIN_M 0xFF00FF
#define COLOR_OLED_LIN_P 0xFF0080
#define COLOR_OLED_LIN_W 0xFFFFFF
#define COLOR_OLED_LIN_K 0x010101

#define COLOR_OLED_LIN_R1 0xFF5555
#define COLOR_OLED_LIN_O1 0xFFAA55
#define COLOR_OLED_LIN_Y1 0xFFFF55
#define COLOR_OLED_LIN_L1 0xAAFF55
#define COLOR_OLED_LIN_G1 0x55FF55
#define COLOR_OLED_LIN_T1 0x55FFAA
#define COLOR_OLED_LIN_C1 0x55FFFF
#define COLOR_OLED_LIN_S1 0x55AAFF
#define COLOR_OLED_LIN_B1 0x5555FF
#define COLOR_OLED_LIN_V1 0xAA55FF
#define COLOR_OLED_LIN_M1 0xFF55FF
#define COLOR_OLED_LIN_P1 0xFF55AA
#define COLOR_OLED_LIN_W1 0xAAAAAA
#define COLOR_OLED_LIN_K1 0x555555

#define COLOR_OLED_LIN_DR 0x330000
#define COLOR_OLED_LIN_DO 0x331900
#define COLOR_OLED_LIN_DY 0x333300
#define COLOR_OLED_LIN_DL 0x193300
#define COLOR_OLED_LIN_DG 0x003300
#define COLOR_OLED_LIN_DT 0x003319
#define COLOR_OLED_LIN_DC 0x003333
#define COLOR_OLED_LIN_DS 0x001933
#define COLOR_OLED_LIN_DB 0x000033
#define COLOR_OLED_LIN_DV 0x190033
#define COLOR_OLED_LIN_DM 0x330033
#define COLOR_OLED_LIN_DP 0x330019
#define COLOR_OLED_LIN_DW 0x333333
#define COLOR_OLED_LIN_DK 0x010101

#define COLOR_OLED_LIN_DR1 0x331111
#define COLOR_OLED_LIN_DO1 0x332211
#define COLOR_OLED_LIN_DY1 0x333311
#define COLOR_OLED_LIN_DL1 0x223311
#define COLOR_OLED_LIN_DG1 0x113311
#define COLOR_OLED_LIN_DT1 0x113322
#define COLOR_OLED_LIN_DC1 0x113333
#define COLOR_OLED_LIN_DS1 0x112233
#define COLOR_OLED_LIN_DB1 0x111133
#define COLOR_OLED_LIN_DV1 0x221133
#define COLOR_OLED_LIN_DM1 0x331133
#define COLOR_OLED_LIN_DP1 0x331122
#define COLOR_OLED_LIN_DW1 0x222222
#define COLOR_OLED_LIN_DK1 0x111111

#ifndef __STAR__COLOR__C__
#define OLED_findColor(key) FindColor(key, OLED_colors)
#define OLED_findDarkColor(key) FindColor(key, OLED_darkcolors)
#endif


/// PuTTY Color(Non-Linear)
#define COLOR_PuTTY_R 0xFF0000
#define COLOR_PuTTY_O 0xFF8000
#define COLOR_PuTTY_Y 0xFFFF00
#define COLOR_PuTTY_L 0x80FF00
#define COLOR_PuTTY_G 0x00FF00
#define COLOR_PuTTY_T 0x00FF80
#define COLOR_PuTTY_C 0x00FFFF
#define COLOR_PuTTY_S 0x0080FF
#define COLOR_PuTTY_B 0x0000FF
#define COLOR_PuTTY_V 0x8000FF
#define COLOR_PuTTY_M 0xFF00FF
#define COLOR_PuTTY_P 0xFF0080
#define COLOR_PuTTY_W 0xFFFFFF
#define COLOR_PuTTY_K 0x000000

#define COLOR_PuTTY_R1 0xFF5555
#define COLOR_PuTTY_O1 0xFFAA55
#define COLOR_PuTTY_Y1 0xFFFF55
#define COLOR_PuTTY_L1 0xAAFF55
#define COLOR_PuTTY_G1 0x55FF55
#define COLOR_PuTTY_T1 0x55FFAA
#define COLOR_PuTTY_C1 0x55FFFF
#define COLOR_PuTTY_S1 0x55AAFF
#define COLOR_PuTTY_B1 0x5555FF
#define COLOR_PuTTY_V1 0xAA55FF
#define COLOR_PuTTY_M1 0xFF55FF
#define COLOR_PuTTY_P1 0xFF55AA
#define COLOR_PuTTY_W1 0xAAAAAA
#define COLOR_PuTTY_K1 0x555555

#define COLOR_PuTTY_DR 0x600000
#define COLOR_PuTTY_DO 0x603000
#define COLOR_PuTTY_DY 0x606000
#define COLOR_PuTTY_DL 0x306000
#define COLOR_PuTTY_DG 0x006000
#define COLOR_PuTTY_DT 0x006030
#define COLOR_PuTTY_DC 0x006060
#define COLOR_PuTTY_DS 0x003060
#define COLOR_PuTTY_DB 0x000060
#define COLOR_PuTTY_DV 0x300060
#define COLOR_PuTTY_DM 0x600060
#define COLOR_PuTTY_DP 0x600030
#define COLOR_PuTTY_DW 0x606060
#define COLOR_PuTTY_DK 0x000000

#define COLOR_PuTTY_DR1 0x602020
#define COLOR_PuTTY_DO1 0x604020
#define COLOR_PuTTY_DY1 0x606020
#define COLOR_PuTTY_DL1 0x406020
#define COLOR_PuTTY_DG1 0x206020
#define COLOR_PuTTY_DT1 0x206040
#define COLOR_PuTTY_DC1 0x206060
#define COLOR_PuTTY_DS1 0x204060
#define COLOR_PuTTY_DB1 0x202060
#define COLOR_PuTTY_DV1 0x402060
#define COLOR_PuTTY_DM1 0x602060
#define COLOR_PuTTY_DP1 0x602040
#define COLOR_PuTTY_DW1 0x404040
#define COLOR_PuTTY_DK1 0x202020

#ifndef __STAR__COLOR__C__
#define PuTTY_findColor(key) FindColor(key, PuTTY_colors)
#define PuTTY_findDarkColor(key) FindColor(key, PuTTY_darkcolors)
#endif

/// Part 2. Default Colors
/// MicroSD 삽입하는 경우 MicroSD에서 Default Color 불러오면 됨.
#define COLOR_DEF_1 0xFFFFFF
#define COLOR_DEF_2 0xFF0000
#define COLOR_DEF_3 0x00FF00
#define COLOR_DEF_4 0x0000FF
#ifndef __STAR__COLOR__C__
extern uint32_t COLOR_default[4];
#define COLOR_DEFAULT1 (COLOR_default[0])
#define COLOR_DEFAULT2 (COLOR_default[1])
#define COLOR_DEFAULT3 (COLOR_default[2])
#define COLOR_DEFAULT4 (COLOR_default[3])
#endif








#endif
