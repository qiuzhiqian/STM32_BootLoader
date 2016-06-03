/**
  ******************************************************************************
  * @file    IAP/src/main.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/** @addtogroup IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <string.h>
#include "usart.h"
#include "timer.h"
#include "ymodem.h"
#include "common.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern pFunction Jump_To_Application;
extern uint32_t g_AutoToAppDelay;

extern USART_Q usart1_cmd;
extern u8 g_RecBuff[];
/* Private function prototypes -----------------------------------------------*/
void JumpToAddress(uint32_t addr);
/* Private functions ---------------------------------------------------------*/
void Close_UsedIRQ(void);
void NVIC_Configuration(void);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Flash unlock */
  FLASH_Unlock();
	NVIC_Configuration();
	TIM3_Configuration(1000,72);
	USART_Configuration(115200);
	
	ReceiveStart();
	g_AutoToAppDelay=5000;
	while(1)
	{
		
		if(strncmp((char *)g_RecBuff,"xml",3)==0)	break;
	}

  /* Test if Key push-button on STM3210X-EVAL Board is pressed */
  if (GetKey()  == 0x0d)
  {
    /* If Key is pressed */
    /* Execute the IAP driver in order to re-program the Flash */
    SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n=              (C) COPYRIGHT 2010 STMicroelectronics                 =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n=     In-Application Programming Application  (Version 3.3.0)        =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n=                                   By MCD Application Team          =");
    SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n\r\n");
    Main_Menu ();
  }
  /* Keep the user application running */
  else
  {
		SerialPutString("\r\nRun user proogram!!");
    /* Test if user code is programmed starting from address "ApplicationAddress" */
    JumpToAddress(ApplicationAddress);
  }

  while (1)
  {}
}

void JumpToAddress(uint32_t addr)
{
	if (((*(__IO uint32_t*)addr) & 0x2FFE0000 ) == 0x20000000)
  {
		Close_UsedIRQ();
		/* Jump to user application */
		Jump_To_Application = (pFunction) (*(__IO uint32_t*) (addr + 4));
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) addr);
		Jump_To_Application();
  }
}

void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Close_UsedIRQ()
{
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
