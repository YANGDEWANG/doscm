#ifndef _M62429P_H_
#define	_M62429P_H_ 
/*
* M62429P��2ͨ������������
* ���������ͣ�avr
* �汾��1
* ���ڣ�2006-12-3
* ���ߣ������<yangdewang@gmail.com>
***********************************************************
* Symbol	Parameter							Min		  *
* tcr		Cycle time of clock					4��s	  *
* tWHC		Pulse width of clock ("H" level)	1.6��s	  *
* tWLC		Pulse width of clock ("L" level)	1.6��s	  *
* tSD		Data setup time						0.8��s	  *
* tHD		Data hold time						0.8��s	  *
***********************************************************
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
#include "..\system\eeprom_ex.h"
#ifndef M62429P_CFG
//-------------------M62429P����---------------------//
#define M62429P_CFG
//Ҫ�������������λ��1
//ע��ESL_VolumeSL��EepromBuffer�б���ΪM62429P���ݵĿ�ʼ
#define M62429P_PORT		D
#define M62429P_DATP		0
#define M62429P_SCK1PORT	D
#define M62429P_SCK1P		1
#define M62429P_SCK2PORT	D
#define M62429P_SCK2P		1
//������Ա���M62429P���õ�EEPROM
#define SAVE_M62429P_SETTING
#if !defined(SAVE_M62429P_SETTING)
extern uint8 M62429PVolumeA[];
#else//SAVE_M62429P_SETTING
#define M62429PVolumeA		(EepromBuffer+ESL_VolumeSL)
#endif//SAVE_M62429P_SETTING
//-----------------M62429P���ý���-------------------//

#endif//M62429P_CFG

#define m62429pPORT	GPORT(M62429P_PORT)
#define m62429pDDR	GDDR(M62429P_PORT)
#define m62429pSCK1PORT	GPORT(M62429P_SCK1PORT)
#define m62429pSCK1DDR	GDDR(M62429P_SCK1PORT)
#define m62429pSCK2PORT	GPORT(M62429P_SCK2PORT)
#define m62429pSCK2DDR	GDDR(M62429P_SCK2PORT)

#define M62429P_MAXVOLUME				(83-MAINVOLUME_MAX)
#define M62429P_MINVOLUME				0//���Ϊ0����




extern void M62429PUpdateAll();
//extern void M62429PUpdate(uint8 flag);
#define IniM62429P() M62429PUpdateAll()

#endif//_M62429P_H_ 
