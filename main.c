/**
 * \file      main.c
 * \brief     Main function
 *
 */
 
#include "main.h"



/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
	
/**
 * \brief     Main function entry
 * \details   
 * \return    \e none.
 */

uint32_t prevCCR = 0;
uint32_t delta;
uint32_t receiveBits;
uint32_t bitsCount;
uint32_t isRecieve;

void inputBits(uint32_t bit)
{
	if (bit == 0)
	{
		receiveBits = receiveBits >> 1; //on ajoute un zéro
	}
	else
	{
		receiveBits = (receiveBits >> 1) | 0x80000000; // on décalle puis on ajoute 1
	}
	bitsCount++;
	if (bitsCount >= 32)
	{
		isRecieve = 1;
	}
	else
	{
		isRecieve = 0;
	}
}


void TIM5_IRQHandler()
{
	if ((TIM5->SR & TIM_SR_CC2IF)!= 0)
	{
		TIM5->SR &=~ TIM_SR_CC2IF;
		if ((TIM5->CCR2 > 2200) && (TIM5->CCR2 <= 2600))
		{
			receiveBits = 0;
			bitsCount = 0;
		}
		if ((TIM5->CCR2 < 400))
		{
			inputBits(0);
		}
		else
		{
			inputBits(1);
		}
	}
	else if ((TIM5->SR & TIM_SR_UIF) != 0)
	{
		TIM5->SR &=~ TIM_SR_UIF;
		receiveBits = 0;
		bitsCount = 0;
	}
}

int main(void)
{

	System_Clock_Init(); // Switch System Clock to maximum
	SysTick_Config(SystemCoreClock/1000);
	
	//init clk
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock on gpiob
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	
	//init GPIO alternante fonction for TIMER
	GPIOA->MODER &=~ GPIO_MODER_MODE1_Msk;
	GPIOA->MODER |= GPIO_MODER_MODE1_1; // Alternante Fonction 
	GPIOA->AFR[0] &=~ GPIO_AFRL_AFRL1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL1_1;
	
	
	
	//init timer
	TIM5->CR1 &=~ (TIM_CR1_CMS_0 | TIM_CR1_CMS_1); //Edge align
	TIM5->CR1 &=~ TIM_CR1_DIR; // upcounter
	TIM5->PSC = 1011; //prescaler for 1MHz
	TIM5->ARR = 99999; //100ms 
	TIM5->CR1 |= TIM_CR1_URS; //only overflow for IT
	TIM5->CCMR1 &=~ TIM_CCMR1_CC2S;
	TIM5->CCMR1 |= TIM_CCMR1_CC2S_0;
	TIM5->CCER &=~ (TIM_CCER_CC2NP | TIM_CCER_CC2P);
	TIM5->CCER |= TIM_CCER_CC2P;
	TIM5->CCER |= TIM_CCER_CC2E;
	TIM5->SMCR &=~ (TIM_SMCR_SMS | TIM_SMCR_TS);
	TIM5->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_1;
	TIM5->SMCR |= TIM_SMCR_SMS_2;
	TIM5->DIER |= TIM_DIER_CC2IE | TIM_DIER_UIE;
	TIM5->EGR |= TIM_EGR_UG;
	TIM5 ->SR = 0;
	
	NVIC_SetPriority(TIM5_IRQn,2);
	NVIC_ClearPendingIRQ(TIM5_IRQn);
	NVIC_EnableIRQ(TIM5_IRQn);
	TIM5->CR1 |= TIM_CR1_CEN;
	
	
	while(1) 
	{
		
	}
}

