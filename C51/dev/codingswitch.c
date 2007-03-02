/*
 * codingswitch,���뿪��
 * ���������ͣ�C51
 * �汾��1
 * ���ڣ�2006-12-5
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
#include <global.h>
#include "codingswitch.h"

int8 CodingsWitchPolling()//return 0=���䣻-1=����1=��//a�ȸ�=+
{
	static bool aold,bold;	//������������������������һ�ε��ô˷����Ǳ��뿪�������ŵĵ�ƽ
	static bool st;			//������һ����������������ǰ�Ƿ�������������Ŷ�Ϊ�ߵ�ƽ��״̬
	int8 tmp = 0;
	if(CodingsWitch_A&&CodingsWitch_A) st = 1;		//����������Ŷ�Ϊ�ߵ�ƽ���st��1
	if(st)					//���stΪ1ִ������Ĳ���
	{
		if(CodingsWitch_A==0&&CodingsWitch_A==0)		//�����ǰ���뿪�ص��������Ŷ�Ϊ�׵�ƽִ������Ĳ���
		{
			if(bold)		//�����һ�ε���ʱ���뿪��b����Ϊ��˵�����뿪������Ӵ�ķ���ת
			{
				st = 0;		
				tmp++;		//�践��ֵΪ1
			}
			if(aold)		//�����һ�ε���ʱ���뿪��a����Ϊ��˵�����뿪�������С�ķ���ת
			{
				st = 0;
				tmp--;		//�践��ֵΪ-1
			}
		}
	}
	aold = CodingsWitch_A;				//����a��aold
	bold = CodingsWitch_A;				//����b��bold
	return tmp;				//���� 0=���䣻-1=����1=��
}
