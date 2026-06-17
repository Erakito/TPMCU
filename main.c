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
 
 typedef enum State {
	 STATE_GEN,
	 STATE_REC,
 } State;
 
 State currentState = STATE_REC;
 
 uint32_t indexTime = 0;
 uint32_t nbrUn = 0;
 float distance = 0;
 
 float computeDistance(int nb)
 {
	 float dist = 0;
	 dist = (float)nb * 100;
	 dist = dist / 58.235;
	 return dist;
 }
 
 void UltraSoundmgt () {
		switch(currentState) {
			case STATE_GEN :
				GPIOA->ODR |= GPIO_ODR_OD12; // Trig à 1
				indexTime ++;
				nbrUn = 0;
			
				if(indexTime >= 1) {
					currentState = STATE_REC;
				}
				break;
			
			case STATE_REC :
				GPIOA->ODR &= ~GPIO_ODR_OD12; // Trig à 0
				indexTime++;
			
				if ((GPIOA->IDR & GPIO_IDR_ID8) != 0) {
					nbrUn ++;
					distance = computeDistance(nbrUn);
				}	
					
			
				if (indexTime >= 500) {
					indexTime = 0;
					currentState = STATE_GEN;
				}
				break;
			}
			
 }

 
void TIM2_IRQHandler()
 {
	 if ((TIM2->SR & TIM_SR_UIF) != 0)
	 {
		 TIM2->SR &=~ TIM_SR_UIF;
		 GPIOA->ODR ^= GPIO_ODR_OD5;
		 
		 UltraSoundmgt();
	 }
 }
 
int main(void)
{

	System_Clock_Init(); // Switch System Clock to maximum
	SysTick_Config(SystemCoreClock/1000);
	
	//init clk
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock on gpiob
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	//init GPIO alternante fonction for TIMER
	GPIOA->MODER &=~ GPIO_MODER_MODE1_Msk;
	GPIOA->MODER |= GPIO_MODER_MODE1_1; // Alternante Fonction 
	GPIOA->AFR[0] &=~ GPIO_AFRL_AFSEL1_Msk;
	GPIOA->AFR[0] |= (GPIO_AF1_TIM2 << GPIO_AFRL_AFSEL1_Pos);
	
	//init GPIO alternante fonction for interruption
	GPIOA->MODER &=~ (GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE12_Msk | GPIO_MODER_MODE8_Msk); // init PA5 
	GPIOA->MODER |= GPIO_MODER_MODE5_0 | GPIO_MODER_MODE12_0; // PA5 PA12 output
	
	
	//inti timer
	TIM2->CR1 &=~ (TIM_CR1_CMS_0 | TIM_CR1_CMS_1); //EDGE ALIGNED	
	TIM2->CR1 &=~ (1<<TIM_CR1_DIR_Pos); // upcounter
	TIM2->PSC = 179; //prescaler for 1MHz
	TIM2->ARR = 100; //50us
	TIM2->CCMR1 &=~ (TIM_CCMR1_CC2S_Msk); //channel is output
	TIM2->CCMR1 |= (TIM_CCMR1_OC2M_2  | TIM_CCMR1_OC2M_1); //OUTPUT COMPARE PWM MODE 1
	TIM2->CCR2 = 50; //duty cycle 1/2
	TIM2->CCER |= TIM_CCER_CC2E;
	TIM2->CR1 |= TIM_CR1_CEN;
	// init interrupt
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2 ->SR &=~ TIM_SR_UIF;
	
	NVIC_EnableIRQ(TIM2_IRQn);
	
	
	while(1) 
	{
		
	}
}

