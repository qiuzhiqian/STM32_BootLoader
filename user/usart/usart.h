/**
  ******************************************************************************
  * @file    usart.h
  * $Author: wdluo $
  * $Revision: 66 $
  * $Date:: 2012-08-14 19:09:52 +0800 #$
  * @brief   串口操作相关函数声明.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USART_H
#define _USART_H
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stm32f10x.h>

#define Q_Len 20

typedef struct
{
	uint8_t RecBuff[Q_Len];
	u8 RxStartIndex;
	u8 RxEndIndex;
}USART_Q;

/* Exported Functions --------------------------------------------------------*/
void USART_Configuration(uint32_t bdrate);
uint8_t USART_GetChar(void);
void USART_SendChar(uint8_t data);

void HandleCMD(u8 m_cmd,USART_Q *m_cmdq);
void GetCmd(u8 *m_cmdbuff,USART_Q *m_cmdq,u8 m_len);
u8 QueneIn(u8 m_cmd,USART_Q *m_cmdq);
u8 QueueOut(USART_Q *m_cmdq);

void ReceiveStart(void);
void ReceiveEnd(void);

#endif /*_usart_H*/

/*********************************END OF FILE**********************************/
