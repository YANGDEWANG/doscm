#ifndef _AT24C02_H_
#define _AT24C02_H_

/*
* AT24C02 Two-wire Serial EEPROM
* ���������ͣ�C51
* �汾��1 ���ڣ�2006-12-31

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



//#define AT24C02_READBYTE_U
//#define AT24C02_WRITEBYTE_U 
//#define MULTI_AT24C02			//���AT24C02����ʹ��24C04����

//PAGE WRITE: The 1K/2K EEPROM is capable of an 8-byte page write, 
//and the 4K, 8K and 16K devices are capable of 16-byte page writes.
#ifndef AT24C02_PAGE_SIZE
#define AT24C02_PAGE_SIZE 8	
#endif//AT24C02_PAGE_SIZE

#ifdef MULTI_AT24C02
extern uint8 At24c02Choice;
#endif//MULTI_AT24C02

//ע�����ζԡ�д24c02�������ĵ��õ�ʱ���������5ms
/*****************************************
д���ݵ�24c02
ע�⣺	����ҳд�����24c02�ĵ�
datAdd:	���ݵ�ַ
Dat:	����ָ��
Count:	�����ֽ���
return:	�ɹ�д���ֽ���
******************************************/
uint8 Write24c02(uint8 datAdd,uint8* Dat,uint8 Count);


/*****************************************
�����ݴ�24c02
ע�⣺	�����Ķ�ȡͬд��һ����ʱ������
		��Ϊ�ڶ�ǰִ����д���������õ�ַ
datAdd:	���ݵ�ַ
dat:	����ָ��
count:	�����ֽ���
return:	�ɹ������ֽ���
******************************************/
uint8 Read24c02(uint8 datAdd,uint8* dat,uint8 count);

/*****************************************
���������ݴ�24c02
dat:	����ָ��
count:	�����ֽ���
return:	�ɹ������ֽ���
******************************************/
uint8 Read24c02Stream(uint8* dat,uint8 count);

//ע�����ζԡ�д24c02�������ĵ��õ�ʱ���������5ms
#ifdef AT24C02_WRITEBYTE_U
bool WriteByte24c02(uint8 Add,uint8 dat);//дһ�ֽڵ�24c02//Add:��ַ��dat:���ݣ�re=true д��ɹ�
#endif//AT24C02_WRITEBYTE_U

#ifdef AT24C02_READBYTE_U
extern bool ReadByte24c02(uint8 Add,uint8 *dat);//��һ�ֽڴ�24c02//Add:��ַ��dat:����ָ�룻re=true ����ɹ�
#endif //AT24C02_READBYTE_U

#endif//_AT24C02_H_
