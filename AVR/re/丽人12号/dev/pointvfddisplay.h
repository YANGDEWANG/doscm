#ifndef _POINTVFDDISPLAY_H_
#define _POINTVFDDISPLAY_H_
#include <configure.h>
#ifndef POINTVFDDISPLAY_CFG
//-----------------POINTVFDDISPLAY配置---------------------//
#define POINTVFDDISPLAY_CFG
#define CHARIMAGE_W 8//字符宽度
#define POINTVFDDISPLAY_DISMEM_SIZE	63
#define POINTVFDDISPLAY_X_P	72
#define POINTVFDDISPLAY_X_BYTE	9
#define POINTVFDDISPLAY_Y_P	7
#define POINTVFDDISPLAY_X_C	(POINTVFDDISPLAY_X_P/8)	//水平方向可用数字显示位
#define POINTVFDDISPLAY_KEY1_PORT	B
#define POINTVFDDISPLAY_KEY2_PORT	B
#define POINTVFDDISPLAY_KEY1_P		1
#define POINTVFDDISPLAY_KEY2_P		2
typedef int8 IndexScreenLine;
//--------------POINTVFDDISPLAY配置结束--------------------//
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
	//欢
	WC_huan='Z'+1,
	//迎
	WC_ying,
	//使
	WC_shi,
	//用
	WC_yong,
	//中
	WC_zhong,
	//文
	WC_wen,
	//音
	WC_yin,
	//响
	WC_xiang,
	//减号
	WC_jianhao,
	//
	WC_kongge,
	
}__attribute__ ((packed));
extern uint8 KeyCode;
extern bool DrawColor;
void DISClean();
void Pollingpointvfddisplay();//1ms
void ShowString(char const*s,uint8 lcX,uint8 charCount);
//只支持大写字母
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
