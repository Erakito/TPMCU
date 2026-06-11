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
			
				if ((GPIOA->IDR & GPIO_IDR_ID8) == 1) {
					nbrUn ++;
				}	
					
			
				if (indexTime >= 500) {
					indexTime = 0;
					currentState = STATE_GEN;
				}
				break;
			}
			
 }

void TIM5_Init(void)
{
    // fonction de Evariste
    /* --- Horloges --- */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   /* port A */
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;    /* TIM5   */

    /* --- PA1 en fonction alternee AF2 (TIM5_CH2 = TI2) --- */
    GPIOA->MODER  &= ~(3u  << (1u*2u));
    GPIOA->MODER  |=  (2u  << (1u*2u));     /* mode AF                       */
    GPIOA->PUPDR  &= ~(3u  << (1u*2u));     /* pas de pull (sortie capteur)  */
    GPIOA->AFR[0] &= ~(0xFu << (1u*4u));
    GPIOA->AFR[0] |=  (0x2u << (1u*4u));    /* AF2 = TIM5                    */

    /* --- Base de temps : tick = 1 us, overflow = 100 ms ---
       En supposant TIMx_CLK = 180 MHz comme dans les TP precedents.
       180 MHz / (PSC+1) = 1 MHz  ->  PSC = 179  ->  1 tick = 1 us          */
    TIM5->PSC = 179u;
    TIM5->ARR = 100000u - 1u;              /* 100 000 us = 100 ms (TIM5 = 32 bits) */

    /* Seul l'overflow doit lever UIF : sinon le reset esclave a chaque
       front declencherait une fausse IT d'update.                          */
    TIM5->CR1 |= TIM_CR1_URS;

    /* --- Canal 2 en capture sur TI2 (CCMR1 AVANT CCER) --- */
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;
    TIM5->CCMR1 |=  TIM_CCMR1_CC2S_0;      /* CC2S = 01 : IC2 sur TI2       */
    /* (option : filtre IC2F dans CCMR1 pour l'anti-rebond)                 */

    /* --- Front descendant + activation de la capture --- */
    TIM5->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
    TIM5->CCER |=  TIM_CCER_CC2P;          /* CC2P = 1 : front descendant   */
    TIM5->CCER |=  TIM_CCER_CC2E;          /* autorise la capture           */

    /* --- Mode esclave RESET sur TI2FP2 : CNT remis a 0 a chaque front --- */
    TIM5->SMCR &= ~(TIM_SMCR_TS | TIM_SMCR_SMS);
    TIM5->SMCR |=  (TIM_SMCR_TS_2 | TIM_SMCR_TS_1);  /* TS  = 110 : TI2FP2  */
    TIM5->SMCR |=  TIM_SMCR_SMS_2;                   /* SMS = 100 : reset   */

    /* --- Interruptions : capture canal 2 + overflow --- */
    TIM5->DIER |= TIM_DIER_CC2IE | TIM_DIER_UIE;

    /* --- Chargement PSC/ARR puis nettoyage des flags --- */
    TIM5->EGR |= TIM_EGR_UG;
    TIM5->SR   = 0;

    /* --- NVIC --- */
    NVIC_SetPriority(TIM5_IRQn, 2);
    NVIC_ClearPendingIRQ(TIM5_IRQn);
    NVIC_EnableIRQ(TIM5_IRQn);

    /* --- Demarrage --- */
    TIM5->CR1 |= TIM_CR1_CEN;
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
	GPIOA->MODER |= GPIO_MODER_MODE5_0 | GPIO_MODER_MODE12_0; // PA5 output
	
	
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

