#ifndef _MC14094B_H_
#define _MC14094B_H_
/*
 * MC14094B��8-Stage Shift/Store  Register with Three-State Outputs
 * ���������ͣ�C51
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

#include <configure.h>


#define MC14094B_MAX_BYTE	1//ÿ��д����ֽ���
#define MC14094B_MSB		1//����ʹ��MSB����ʹ��LSB

//PIN
#ifndef STROBE_MC14094B_PIN
#define STROBE_MC14094B_PIN (P1^0)
#endif

//�������ݽӿ�����
sbit STROBE_MC14094B = STROBE_MC14094B_PIN;

extern void IniMC14094B();
extern void MC14094BWrite(uint8 *dat);



#endif//_MC14094B_H_
