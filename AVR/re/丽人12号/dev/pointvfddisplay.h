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
void DISClean();
void Pollingpointvfddisplay();//1ms
void ShowString(char const*s,uint8 lcX,uint8 charCount);
//ֻ֧�ִ�д��ĸ
void ShowString_P(const prog_char *s,uint8 lcX,uint8 charCount);
void inline DrawDot(IndexScreenLine x,IndexScreenLine y);
void DrawLine(IndexScreenLine xa, IndexScreenLine ya, IndexScreenLine xb, IndexScreenLine yb);
void DrawRectangle(IndexScreenLine x1,IndexScreenLine y1,IndexScreenLine x2,IndexScreenLine y2);
void FillRectangle(IndexScreenLine x1,IndexScreenLine y1,IndexScreenLine x2,IndexScreenLine y2);
//void IniKEYInput();
#define IniKEYInput()
void InitDisplay();
void ShowINT8(i8 sd);
#endif //_POINTDISPLAY_H_
