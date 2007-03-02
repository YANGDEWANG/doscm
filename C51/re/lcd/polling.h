#ifndef _POLLING_H_
#define _POLLING_H_
/*
* polling����ѯ����ϵͳ����
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
#include "configure.h"

extern uint8 UserEventExitCount;
extern void PollingMain();
extern void ShowState();
#define InUserEvent() (UserEventExitCount=0)
/*enum//work state
{
	WS_IDLE,
	WS_MUTE,
};*/
#endif//_POLLING_H_
