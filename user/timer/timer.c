#include "timer.h"

uint32_t g_RecTimeOut=0;
uint32_t g_CommonDelay=0;
uint32_t g_AutoToAppDelay=0;

void TIM3_Configuration(uint32_t arr,uint32_t psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM3->CNT=0;
  TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
	  TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
		if(g_RecTimeOut>0)	g_RecTimeOut--;
		if(g_CommonDelay>0)	g_CommonDelay--;
		if(g_AutoToAppDelay>0)	g_AutoToAppDelay--;
	}
}

void delay_ms(uint32_t nus)
{
	g_CommonDelay=nus;
	while(g_CommonDelay);
}
