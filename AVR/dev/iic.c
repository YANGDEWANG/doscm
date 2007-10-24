/*
* I2C Э��
* ���������ͣ�AVR
* �汾��1 ���ڣ�2007-1-23
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
#include "dev\iic.h"
#include <util/twi.h>
#include <avr/interrupt.h>
//#define IIC_DEBUG
//struct IIC_STATE IICStatebits;

#define S_start()	{TWCR =(1<<TWINT)|(1<<TWEN)|(1<<TWIE)|(1<<TWSTA);IICState.Error = false;}
#define S_stop()	{TWCR =(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);IICState.cStop = false;IICState.IICBusy = false;}
#define S_ack()		(TWCR =(1<<TWINT)|(1<<TWEN)|(1<<TWIE)|(1<<TWEA))
#define S_nack()	(TWCR =(1<<TWINT)|(1<<TWEN)|(1<<TWIE))
#define WaitIICIdle() {while(IICState.IICBusy);}

//TWCRֻ��IN/OUT,ֱ�Ӹ�ֵ���߼�����(|= &=)����ʡ�ռ�


//Ԥ��Ƶϵ��=1(TWPS=0)
#if ((F_CPU/F_I2CSCL-16)/8)<10
#define TWBR_SET 			(10)					//TWBR������ڵ���10
#else
#define TWBR_SET  			((F_CPU/F_I2CSCL-16)/8)//����TWBRֵ
#endif

#define TW_START				0x08	//�ѷ���START,����SLA+W
#define TW_RESTART				0x10	//�ظ�START �ѷ���
#define TW_MT_ARB_LOST			0x38	//SLA+W��SLA+R �����ݵ��ٲ�ʧ��

//�������ͻ�
#define TW_MT_SLA_W_ACK			0x18	//�ѷ���SLA_W,���յ�ACK����������( �ֽ�)
#define TW_MT_SLA_W_NACK		0x20	//�ѷ���SLA_W,���յ�NACK
#define TW_MT_DATA_ACK			0x28	//�����ѷ���,���յ�ACK
#define TW_MT_DATA_NACK			0x30	//�����ѷ���,���յ�NACK
//�������ջ�
#define TW_MR_SLA_R_ACK			0x40	//�ѷ���SLA_R,���յ�ACK����������( �ֽ�)
#define TW_MR_SLA_R_NACK		0x48	//�ѷ���SLA_R,���յ�NACK
#define TW_MR_DATA_ACK			0x50	//��������,ACK �ѷ���
#define TW_MR_DATA_NACK			0x58	//��������,NACK �ѷ���
//�ڴӻ�����ģʽ	
#define TW_SR_SLA_ACK           0x60	//�Լ���SLA+W �Ѿ�������ACK �ѷ���
#define TW_SR_ARB_LOST_SLA_ACK  0x68	//SLA+R/W ��Ϊ�������ٲ�ʧ�ܣ��Լ���SLA+W �Ѿ�������ACK �ѷ���
#define TW_SR_GCALL_ACK         0x70	//���յ��㲥��ַ ACK �ѷ���
#define TW_SR_ARB_LOST_GCALL_ACK 0x78	//SLA+R/W ��Ϊ�������ٲ�ʧ�ܣ����յ��㲥��ַACK �ѷ���
#define TW_SR_DATA_ACK          0x80	//��ǰ���Լ��� SLA+W ��Ѱַ�������Ѿ�������ACK �ѷ���
#define TW_SR_DATA_NACK         0x88	//��ǰ���Լ��� SLA+W ��Ѱַ�����Ѿ�������NOT ACK �ѷ���
#define TW_SR_GCALL_DATA_ACK    0x90	//��ǰ�Թ㲥��ʽ��Ѱַ�������Ѿ�������ACK �ѷ���
#define TW_SR_GCALL_DATA_NACK   0x98	//��ǰ�Թ㲥��ʽ��Ѱַ�������Ѿ�������NOT ACK �ѷ���
#define TW_SR_STOP              0xA0	//���Դӻ�����ʱ���յ�STOP���ظ�START
//�ӻ�����ģʽ
#define TW_ST_SLA_ACK           0xA8	//�Լ���SLA+R �Ѿ�������ACK �ѷ���
#define TW_ST_ARB_LOST_SLA_ACK  0xB0	//SLA+R/W ��Ϊ�������ٲ�ʧ�ܣ��Լ���SLA+R �Ѿ�������ACK �ѷ���
#define TW_ST_DATA_ACK          0xB8	//TWDR �������Ѿ����ͽ��յ�ACK
#define TW_ST_DATA_NACK         0xC0	//TWDR �������Ѿ����ͽ��յ�NOT ACK
#define TW_ST_LAST_DATA         0xC8	//TWDR ��һ�ֽ������Ѿ�����(TWAE = ��0��);���յ�ACK
/* Misc */
#define TW_NO_INFO              0xF8	//û����ص�״̬��Ϣ��TWINT = ��0��
#define TW_BUS_ERROR            0x00	//���ڷǷ���START ��STOP ��������ߴ���

#define TW_MT_TWSR_STATUS_MASK	0xF8
#define TW_MT_TWCR_CMD_MASK		0x0F

#define IICPORT GPORT(I2C_POPT)
#define IICPIN  GPIN(I2C_POPT)
#define IICDDR  GDDR(I2C_POPT)

volatile IICStatebits IICState;
//bool volatile IICBusy;
static uint8* dataPtr;
static uint8  srAdd;
#define dataCount IICState.CountWaitToSend
//volatile static uint8  dataCount;
void IniIIC()
{
	IICDDR  &= ~(_BV(I2C_SCK)|_BV(I2C_SDA));
#ifdef I2C_ENABLE_PULL
	IICPORT |= _BV(I2C_SCK)|_BV(I2C_SDA);
#else
	IICPORT &= ~(_BV(I2C_SCK)|_BV(I2C_SDA));
#endif

	TWSR=0;									//Ԥ��Ƶ=0^4=1
	TWAR=0x00;									//����ģʽ���õ�ַ��Ч
	TWCR=0x00;									//�ر�TWIģ��
	IICState.IICBusy = false;
	TWBR=TWBR_SET;
}
#ifdef I2C_SHARE_SDA_PIN
/***************************************************
���ȵȴ�IIC�����������Ȼ��ر�IIC�ͷ�DAT����
����Ҫ����DAT����ʹ�ô˷���
���÷�����SCK�ű���Ϊ����͵�ƽ����I2C�ϲ��������ź�
Ҫ����ʹ��IIC�����IniIIC
ע�����ʹ��I2CWriteStream���������Զ�����IniIIC
*****************************************************/
void IICDisable()
{
#ifndef AVRSIMULATOR
	while(IICBusy);
#endif//AVRSIMULATOR
	IICPORT &= ~(_BV(I2C_SCK));
	IICDDR  |= _BV(I2C_SCK);
	TWCR=0x00;		//�ر�TWIģ��
}
#endif//I2C_SHARE_SDA_PIN
/***************************************************
дһ���ֽڴ���ָ���豸 
add:	�豸��ַ 
dat:	�������ֽ�ָ�� 
count:	������ 
*****************************************************/
void I2CWriteStream(uint8 devAdd,uint8* dat,uint8 count)
{
#ifndef AVRSIMULATOR
	WaitIICIdle();
#endif//AVRSIMULATOR
#ifdef I2C_SHARE_SDA_PIN
	IniIIC();
#endif//I2C_SHARE_SDA_PIN
	IICState.IICBusy = true;
	srAdd	= devAdd<<1;
	dataPtr = dat;
	dataCount = count;
	//if(TWCR&(1<<TWEN))
	//{
	//	while(!(TWCR&(1<<TWINT)));//�ȴ�stop�������
	//}
	S_start();
}
#if 0

/***************************************************
дһ���ֽڴ���ָ���豸 
add:	�豸��ַ 
datAdd:	���ݵ�ַ
dat:	�������ֽ�ָ�� 
count:	������ 
return:	�ɹ�д���ֽ���
*****************************************************/
uint8 I2CWriteBunch(uint8 add,uint8 datAdd,uint8* dat,uint8 count)
{
	return 0;
}




#endif//0
/***************************************************
��һ���ֽڴ����豸
�˷�����Բ����ṩ���ݵ�ַ���豸��������ȡ
devAdd:	�豸��ַ 
dat:	�������ֽ�ָ�� 
count:	������ 
return:	�ɹ������ֽ���
*****************************************************/
uint8 I2CReadStream(uint8 devAdd,uint8* dat,uint8 count)
{
#ifndef AVRSIMULATOR
	WaitIICIdle();
#endif//AVRSIMULATOR
#ifdef I2C_SHARE_SDA_PIN
	IniIIC();
#endif//I2C_SHARE_SDA_PIN
	IICState.IICBusy = true;
	srAdd	= (devAdd<<1)+1;
	dataPtr = dat;
	dataCount = count;
	S_start();
#ifndef AVRSIMULATOR
	WaitIICIdle();
#endif//AVRSIMULATOR
	return count-dataCount;
}
/***************************************************
��һ���ֽڴ����豸 
devAdd:	�豸��ַ 
datAdd:	���ݵ�ַ
dat:	�������ֽ�ָ�� 
count:	������ 
return:	�ɹ������ֽ���
*****************************************************/
uint8 I2CReadBunch(uint8 devAdd,uint8 datAdd,uint8* dat,uint8 count)
{
#ifdef I2C_SHARE_SDA_PIN
	IniIIC();
#endif//I2C_SHARE_SDA_PIN
	IICState.cStop=true;
	I2CWriteStream(devAdd,&datAdd,1);

#ifndef AVRSIMULATOR
	WaitIICIdle();
#endif//AVRSIMULATOR
	if(IICState.Error)
		return 0;
	IICState.IICBusy = true;
	srAdd	= (devAdd<<1)+1;
	dataPtr = dat;
	dataCount = count;
	S_start();
#ifndef AVRSIMULATOR
	WaitIICIdle();
#endif//AVRSIMULATOR
	return count-dataCount;
}
ISR(SIG_2WIRE_SERIAL)
{

	switch(TWSR & TW_MT_TWSR_STATUS_MASK)
	{
	case TW_MT_ARB_LOST://SLA+W��SLA+R �����ݵ��ٲ�ʧ��
		{
			S_start();
			break;
		}
	case TW_RESTART:	//�ظ�START �ѷ���		
	case TW_START:		//�ѷ���START,����SLA+W
		{
			TWDR = srAdd;
			TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
			break;
		}
#ifdef IIC_DEBUG
	case TW_MT_DATA_NACK:	//�����ѷ���,���յ�NACK		
	case TW_MT_SLA_W_NACK:	//�ѷ���SLA_W,���յ�NACK
#endif//IIC_DEBUG
	case TW_MT_DATA_ACK:	//�����ѷ���,���յ�ACK		
	case TW_MT_SLA_W_ACK:	//�ѷ���SLA_W,���յ�ACK����������( �ֽ�)
		{
			if(dataCount)
			{
				dataCount--;
				TWDR = *dataPtr++;
				TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
			}
			else
			{
				if(!IICState.cStop)
				{
					S_stop();
				}
				else
				{
					//S_nack();
				}
				IICState.cStop = false;
				IICState.IICBusy = false;//TODO:

			}
			break;
		}
	case TW_MR_SLA_R_ACK://�ѷ���SLA_R,���յ�ACK����������( �ֽ�)
		{
			if(dataCount>1)
			{
				S_ack();
			}
			else
			{
				S_nack();
			}
			break;
		}
	case TW_MR_DATA_ACK://��������,ACK �ѷ���
		{
			dataCount--;
			*dataPtr++=TWDR;
			if(dataCount>1)
			{
				S_ack();
			}
			else
			{
				S_nack();
			}
			break;
		}
	case TW_MR_DATA_NACK://��������,NACK �ѷ���
		{
			dataCount--;
			*dataPtr++=TWDR;
			S_stop();
			//IICState.IICBusy = false;//TODO:
			break;
		}
#ifndef IIC_DEBUG

	case TW_MR_SLA_R_NACK://�ѷ���SLA_R,���յ�NACK
	case TW_MT_DATA_NACK:	//�����ѷ���,���յ�NACK		
	case TW_MT_SLA_W_NACK:	//�ѷ���SLA_W,���յ�NACK	
		{
			S_stop();
			IICState.Error = true;
			//IICState.IICBusy = false;
#ifdef ErrorCodeShow
			ShowErrorCode(ErrorCode_TW_MT_NACK);
#endif
			break;
		}
#endif//IIC_DEBUG
	case TW_BUS_ERROR:	//���ڷǷ���START ��STOP ��������ߴ���
		{
			S_stop();
			IICState.Error = true;
			//IICState.IICBusy = false;
			break;
		}
		/*default:
		{
		S_stop();
		IICBusy = false;
		break;
		}*/
	}
}
