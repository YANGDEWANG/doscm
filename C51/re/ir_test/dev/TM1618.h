#ifndef _TM1618_H_
#define _TM1618_H_
/*
* TM1618��TM1618 ��һ�ִ�����ɨ��ӿڵ�LED�������������ʾ������������ר�õ�·
* �ڲ�������MCU ���ֽӿڡ�������������LED�ߵ�������������ɨ��ȵ�·
* ���������ͣ�C51,avr
* �汾��1
* ���ڣ�2006-12-3
* ���ߣ������<yangdewang@gmail.com>

* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
* 02111-1307, USA.  Or, point your browser to
* http://www.gnu.org/copyleft/gpl.html
*/

#include "configure.h"

#define TM1618DISPLAY_MODE	(0)//������ʾģʽ0:4λ��2:5λ��3:6λ��4:7λ��

typedef void (*tm1628_stbf)();
#if 0
#ifndef TM1618_STB_PIN
#define TM1618_STB_PIN P1^1
#endif
sbit TM1618_STB=TM1618_STB_PIN;
#endif
extern tm1628_stbf tm1628_setSTB;
extern tm1628_stbf tm1628_clSTB;
uint8 TM1618ReadKey(void);
#define IniTM1618() TM1618UpdateAll()
extern void  TM1618UpdateAll(uint8 *dat);


#endif//#define _TM1618_H_
