/**
  ******************************************************************************
  * @file    usart.c
  * $Author: wdluo $
  * $Revision: 66 $
  * $Date:: 2012-08-14 19:09:52 +0800 #$
  * @brief   串口相关函数。
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "common.h"

/** @addtogroup USART
  * @brief 串口模块
  * @{
  */

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
USART_Q usart1_cmd;

uint8_t g_RecEn=0;
u8 g_RecBuff[5];

/**
  * @brief  配置串口1，并使能串口1
  * @param  None
  * @retval None
  */
void USART_Configuration(uint32_t bdrate)
{
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_InitStruct.USART_BaudRate = bdrate;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);
	
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//使能接收中断
	USART_Cmd(USART1, ENABLE);//使能串口1
}

void USART1_IRQHandler()
{
	u8 ch;
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE))
	{
		USART_ClearFlag(USART1,USART_IT_RXNE);
		ch=USART_ReceiveData(USART1);
		if(g_RecEn)
		{
			HandleCMD(ch,&usart1_cmd);
		}
	}
}

void HandleCMD(u8 m_cmd,USART_Q *m_cmdq)
{
	switch(m_cmd)
	{
		case 0x0d:
			return;
		case 0x08:		//退格键
			QueueOut(m_cmdq);
//			SerialPutChar(0x08);
//			SerialPutChar(0x20);
//			SerialPutChar(0x08);
			break;
		default:
			QueneIn(m_cmd,m_cmdq);
			//SerialPutChar(m_cmd);
			break;
	}
	GetCmd(g_RecBuff,&m_cmdq,6);
	Serial_PutString(g_RecBuff);
}

void GetCmd(u8 *m_cmdbuff,USART_Q *m_cmdq,u8 m_len)
{
	u8 len,i;
	u8 cnt=0;
	if(m_cmdq->RxEndIndex<m_cmdq->RxStartIndex)	len=m_cmdq->RxEndIndex+20-m_cmdq->RxStartIndex;
	else	len=m_cmdq->RxEndIndex-m_cmdq->RxStartIndex;
	
	for(i=0;i<len;i++)
	{
		m_cmdbuff[i]=QueueOut(m_cmdq);
	}
}

u8 QueneIn(u8 m_cmd,USART_Q *m_cmdq)
{
	m_cmdq->RecBuff[m_cmdq->RxEndIndex]=m_cmd;
	m_cmdq->RxEndIndex=(m_cmdq->RxEndIndex+1)%Q_Len;
}

u8 QueueOut(USART_Q *m_cmdq)
{
	u8 ch;
	if(m_cmdq->RxStartIndex!=m_cmdq->RxEndIndex)
	{
		ch=m_cmdq->RecBuff[m_cmdq->RxStartIndex];
		m_cmdq->RecBuff[m_cmdq->RxStartIndex]=0;
		m_cmdq->RxStartIndex=(m_cmdq->RxStartIndex+1)%Q_Len;
	}
	return ch;
}

void ReceiveStart()
{
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//使能接收中断
	g_RecEn=1;
	//g_RecCnt=0;
}

void ReceiveEnd()
{
	g_RecEn=0;
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//使能接收中断
}


PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1,(uint8_t)ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	return ch;
}

uint8_t USART_GetChar(void)
{
	uint8_t ch=0;
	while(!(USART1->SR & USART_FLAG_RXNE));
	ch = USART_ReceiveData(USART1);
	//USART_ClearFlag(USART1,USART_FLAG_RXNE);
	return ch;
	
}
void USART_SendChar(uint8_t data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
	USART_SendData(USART1,data);	
}
/*********************************END OF FILE**********************************/

