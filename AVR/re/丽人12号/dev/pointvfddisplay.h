#ifndef _POINTVFDDISPLAY_H_
#define _POINTVFDDISPLAY_H_
#include <configure.h>
#ifndef POINTVFDDISPLAY_CFG
//-----------------POINTVFDDISPLAY����---------------------//
#define POINTVFDDISPLAY_CFG
#define CHARIMAGE_W 8//�ַ����
#define POINTVFDDISPLAY_DISMEM_SIZE	63
#define POINTVFDDISPLAY_X_P	72
#define POINTVFDDISPLAY_X_BYTE	9
#define POINTVFDDISPLAY_Y_P	7
#define POINTVFDDISPLAY_X_C	(POINTVFDDISPLAY_X_P/8)	//ˮƽ�������������ʾλ
#define POINTVFDDISPLAY_KEY1_PORT	B
#define POINTVFDDISPLAY_KEY2_PORT	B
#define POINTVFDDISPLAY_KEY1_P		1
#define POINTVFDDISPLAY_KEY2_P		2
typedef int8 IndexScreenLine;
//--------------POINTVFDDISPLAY���ý���--------------------//
#endif
#define DISPOINT_COUNT	(POINTVFDDISPLAY_X_P*POINTVFDDISPLAY_Y_P)
#define	pointvfddisplayKEY1PORT GPORT(POINTVFDDISPLAY_KEY1_PORT)
#define	pointvfddisplayKEY1PIN GPIN(POINTVFDDISPLAY_KEY1_PORT)
#define	pointvfddisplayKEY1DDR GDDR(POINTVFDDISPLAY_KEY1_PORT)
#define	pointvfddisplayKEY2PORT GPORT(POINTVFDDISPLAY_KEY2_PORT)
#define	pointvfddisplayKEY2PIN GPIN(POINTVFDDISPLAY_KEY2_PORT)
#define	pointvfddisplayKEY2DDR GDDR(POINTVFDDISPLAY_KEY2_PORT)
enum WCHAR
{
	//��
	WC_huan='Z'+1,
	//ӭ
	WC_ying,
	//ʹ
	WC_shi,
	//��
	WC_yong,
	//��
	WC_zhong,
	//��
	WC_wen,
	//��
	WC_yin,
	//��
	WC_xiang,
	//����
	WC_jianhao,
	//
	WC_kongge,
	
}__attribute__ ((packed));
extern uint8 KeyCode;
extern bool DrawColor;
extern uint8 dismem[POINTVFDDISPLAY_DISMEM_SIZE];
void DISClean();
void Pollingpointvfddisplay();//1ms
void ShowString(char const*s,uint8 lcX,uint8 charCount);
//ֻ֧�ִ�д��ĸ
void ShowString_P(const prog_char *s,uint8 lcX,uint8 charCount);
//ֻ֧�ִ�д��ĸ
void ShowStringALL_P(const prog_char *s);
void DrawDot(IndexScreenLine x,IndexScreenLine y);
void DrawLine(IndexScreenLine xa, IndexScreenLine ya, IndexScreenLine xb, IndexScreenLine yb);
void DrawRectangle(IndexScreenLine x1,IndexScreenLine y1,IndexScreenLine x2,IndexScreenLine y2);
void FillRectangle(IndexScreenLine x1,IndexScreenLine y1,IndexScreenLine x2,IndexScreenLine y2);
//void IniKEYInput();
#define IniKEYInput()
static inline void InitDisplay()
{
	//����
	pointvfddisplayKEY1PORT |= _BV(POINTVFDDISPLAY_KEY1_P);
#ifdef POINTVFDDISPLAY_KEY2_P
	pointvfddisplayKEY2PORT |= _BV(POINTVFDDISPLAY_KEY2_P);
#endif
	//OCR2  = F_CPU/1000/128;//1ms
	OCR2  = F_CPU/1000/128/2;//0.5ms
	TCCR2 = _BV(WGM21)|_BV(CS22)|_BV(CS20);//CTCģʽ//128��Ƶ
	TIMSK|=_BV(OCF2);
}
//void ShowINT8(i8 sd);
void ShowStringAndI8_P2(const prog_char* s1,const prog_char* s2, i8 d);
void ShowString_P2(const prog_char* s1,const prog_char* s2,u8 charCount);
#endif //_POINTDISPLAY_H_
