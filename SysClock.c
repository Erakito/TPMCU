/**
 * \file      SysClock.c
 * \author    L. BOURDEL
 * \version   1.0
 * \date      2017
 * \brief     Primitives functions for System Clock config
 *
 * \details   !!! Good Luck !!!
 */
 
 #include "SysClock.h"

uint32_t SystemCoreClock = 0;  /*!< Contains the core clock speed in Hz (HCLK) */
uint32_t Apb1BusClock = 0;  /*!< Contains the APB1 clock speed in Hz */
uint32_t Apb2BusClock = 0;  /*!< Contains the APB2 clock speed in Hz */

//******************************************************************************************
// Switch the PLL source from MSI to HSI, and select the PLL as SYSCLK source.
//******************************************************************************************
/**
 * \brief     Configure Clock for SYSCLK source
 * \details   Select and configure PLL
 * \return    \e none.
 */
 void System_Clock_Init(void){
	
// To correctly read data from FLASH memory, the number of wait states (LATENCY)
// must be correctly programmed according to the frequency of the CPU clock
// (HCLK) and the supply voltage of the device.		
	
// SEARCH LATENCY IN reference manual FOR DETAILS
	
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
#if defined(STM32F401xE)
	FLASH->ACR |=  FLASH_ACR_LATENCY_2WS;
#elif defined(STM32F407xx)
	FLASH->ACR |=  FLASH_ACR_LATENCY_5WS;
#elif defined(STM32F429xx)
	FLASH->ACR |=  FLASH_ACR_LATENCY_7WS;
#elif defined(STM32F446xx)
	FLASH->ACR |=  FLASH_ACR_LATENCY_5WS;
#else
 #error "Please select first the target STM32F4xx device used in your application (in stm32f4xx.h file)"
#endif
	
	// Enable the Internal High Speed oscillator 16Mhz (HSI)
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	// Adjusts the Internal High Speed oscillator (HSI) calibration value
	// RC oscillator frequencies are factory calibrated by ST for 1 % accuracy at 25oC
	// After reset, the factory calibration value is loaded in HSICAL[7:0] of RCC_ICSCR	
	RCC->CR &= ~RCC_CR_HSITRIM;
	RCC->CR |= RCC_CR_HSITRIM_4; // 16; // user-programmable trimming value that is added to  HSITRIM[7:0]not in HSICAL[7:0] !!!
	
	RCC->CR    &= ~RCC_CR_PLLON; 
	while((RCC->CR & RCC_CR_PLLRDY) == RCC_CR_PLLRDY);
	
	// Select clock source to PLL
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI; // 0 =  HSI, 1 = HSE

#if defined(STM32F401xE)
	// Make PLL as 84 MHz
	
//#define RCC_MAX_FREQUENCY            84000000U         /*!< Max frequency of family in Hz*/
//#define RCC_MAX_FREQUENCY_SCALE3     60000000U         /*!< Maximum frequency for system clock at power scale3, in Hz */
//#define RCC_MAX_FREQUENCY_SCALE2    RCC_MAX_FREQUENCY  /*!< Maximum frequency for system clock at power scale2, in Hz */
//#define RCC_PLLVCO_OUTPUT_MIN       192000000U       /*!< Frequency min for PLLVCO output, in Hz */
//#define RCC_PLLVCO_INPUT_MIN           950000U       /*!< Frequency min for PLLVCO input, in Hz  */
//#define RCC_PLLVCO_INPUT_MAX          2100000U       /*!< Frequency max for PLLVCO input, in Hz  */
//#define RCC_PLLVCO_OUTPUT_MAX       432000000U       /*!< Frequency max for PLLVCO output, in Hz */

//#define RCC_PLLN_MIN_VALUE                192U
//#define RCC_PLLN_MAX_VALUE                432U
	
	// (192Mhz<VCO clock<432Mhz) & (0.95Mhz<PLL clock input<2.1Mhz)
	// f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) = 16MHz * 336/16 = 336 MHz 
	// f(PLL_Out) = f(VCO clock) / PLLP = 336MHz/4 = 84MHz (max 84Mhz)  
	// f(Peripheral, USB, ..) = f(VCO clock) / PLLQ = 336MHz/7 = 48MHz
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLQ) | 8U <<  RCC_PLLCFGR_PLLQ_Pos;
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLP) | 1U <<  RCC_PLLCFGR_PLLP_Pos;  
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | 168U << RCC_PLLCFGR_PLLN_Pos;
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | 8U <<  RCC_PLLCFGR_PLLM_Pos;
	
	Apb1BusClock = Apb2BusClock = SystemCoreClock = 84000000;
#elif defined(STM32F407xx)
	// Make PLL as 168 MHz

//#define RCC_MAX_FREQUENCY           168000000U         /*!< Max frequency of family in Hz*/
//#define RCC_MAX_FREQUENCY_SCALE1    RCC_MAX_FREQUENCY  /*!< Maximum frequency for system clock at power scale1, in Hz */
//#define RCC_MAX_FREQUENCY_SCALE2    144000000U         /*!< Maximum frequency for system clock at power scale2, in Hz */
//#define RCC_PLLVCO_OUTPUT_MIN       100000000U       /*!< Frequency min for PLLVCO output, in Hz */
//#define RCC_PLLVCO_INPUT_MIN           950000U       /*!< Frequency min for PLLVCO input, in Hz  */
//#define RCC_PLLVCO_INPUT_MAX          2100000U       /*!< Frequency max for PLLVCO input, in Hz  */
//#define RCC_PLLVCO_OUTPUT_MAX       432000000U       /*!< Frequency max for PLLVCO output, in Hz */

//#define RCC_PLLN_MIN_VALUE                 50U
//#define RCC_PLLN_MAX_VALUE                432U

	// (100Mhz<VCO clock<432Mhz) & (0.95Mhz<PLL clock input<2.1Mhz)
	// f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) = 16MHz * 336/16 = 336 MHz 
	// f(PLL_Out) = f(VCO clock) / PLLP = 336MHz/2 = 168MHz (max 168Mhz)  
	// f(Peripheral, USB, ..) = f(VCO clock) / PLLQ = 336MHz/8 = 42MHz
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLQ) | 8U <<  RCC_PLLCFGR_PLLQ_Pos;
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLP) | 0U <<  RCC_PLLCFGR_PLLP_Pos;  
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | 336U << RCC_PLLCFGR_PLLN_Pos; 
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | 16U <<  RCC_PLLCFGR_PLLM_Pos;
	
	Apb1BusClock = Apb2BusClock = SystemCoreClock = 168000000;
#elif defined(STM32F429xx)
#elif defined(STM32F446xx)
// Make PLL as 180 MHz

//#define RCC_MAX_FREQUENCY           180000000U         /*!< Max frequency of family in Hz*/
//#define RCC_MAX_FREQUENCY_SCALE1    RCC_MAX_FREQUENCY  /*!< Maximum frequency for system clock at power scale1, in Hz */
//#define RCC_MAX_FREQUENCY_SCALE2    168000000U         /*!< Maximum frequency for system clock at power scale2, in Hz */
//#define RCC_MAX_FREQUENCY_SCALE3    120000000U         /*!< Maximum frequency for system clock at power scale3, in Hz */
//#define RCC_PLLVCO_OUTPUT_MIN       100000000U       /*!< Frequency min for PLLVCO output, in Hz */
//#define RCC_PLLVCO_INPUT_MIN           950000U       /*!< Frequency min for PLLVCO input, in Hz  */
//#define RCC_PLLVCO_INPUT_MAX          2100000U       /*!< Frequency max for PLLVCO input, in Hz  */
//#define RCC_PLLVCO_OUTPUT_MAX       432000000U       /*!< Frequency max for PLLVCO output, in Hz */

//#define RCC_PLLN_MIN_VALUE                 50U
//#define RCC_PLLN_MAX_VALUE                432U
	
	// (50Mhz<VCO clock<432Mhz) & (0.95Mhz<PLL clock input<2.1Mhz)
	// f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) = 16MHz * 180/8 = 360 MHz 
	// f(PLL_Out) = f(VCO clock) / PLLP = 360MHz/2 = 180MHz (max 180Mhz)  
	// f(Peripheral, USB, ..) = f(VCO clock) / PLLQ = 360MHz/2 = 180MHz

	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLQ) | 2U <<  RCC_PLLCFGR_PLLQ_Pos; // Q=2
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLP) | 0U <<  RCC_PLLCFGR_PLLP_Pos; // P=2
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | 180U << RCC_PLLCFGR_PLLN_Pos; // N=180
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | 8U <<  RCC_PLLCFGR_PLLM_Pos; // M=8
	
	Apb1BusClock = Apb2BusClock = SystemCoreClock = RCC_MAX_FREQUENCY;
#else
 #error "Please select first the target STM32F4xx device used in your application (in stm32f4xx.h file)"
#endif

// For debug MCO_1 & MCO_2 : MCO1 = PA8, MCO2 = PC9
//	/* Enable GPIOs clock */ 
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
//	
//	RCC->CFGR &= ~RCC_CFGR_MCO1; // 00: HSI clock selected
//	RCC->CFGR &= ~RCC_CFGR_MCO2; // 00: System clock (SYSCLK) selected
//	
//	RCC->CFGR |= RCC_CFGR_MCO1PRE; // 111: division by 5
//	RCC->CFGR |= RCC_CFGR_MCO2PRE; // 111: division by 5
//	///////////////////////////////////////////////////////////////////////////////////////////////
//	// MCO1 = PA8, MCO2 = PC9
//	///////////////////////////////////////////////////////////////////////////////////////////////
//	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
//	GPIOA->MODER &= ~GPIO_MODER_MODE8; // ~(3U<<(13*2));  // Clear bits
//	GPIOA->MODER |= GPIO_MODER_MODE8_1; // AlterFunc(10)
//	GPIOA->AFR[1] 	&= ~GPIO_AFRH_AFSEL8; // 0x0001 ;  HIGH:1 LOW:0 // 	AF 1 = TIM1_CH1N
//	GPIOA->OTYPER &= ~GPIO_OTYPER_OT8; //~(1U<<2);       // Push-pull
//	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED8; //~(3U<<(13*2));
//	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED8; //  3U<<(13*2);  // High speed
//	GPIOA->PUPDR   &= ~GPIO_PUPDR_PUPD8; //~(3U<<(13*2));  // No pull-up, no pull-down

//	GPIOC->MODER &= ~GPIO_MODER_MODE9; // ~(3U<<(13*2));  // Clear bits
//	GPIOC->MODER |= GPIO_MODER_MODE9_1; // AlterFunc(10)
//	GPIOC->AFR[1] &= ~GPIO_AFRH_AFSEL9; // 0x0001 ;  HIGH:1 LOW:0 // 	AF 1 = TIM1_CH1N
//	GPIOC->OTYPER &= ~GPIO_OTYPER_OT9; //~(1U<<2);       // Push-pull
//	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED9; //~(3U<<(13*2));
//	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED9; //  3U<<(13*2);  // High speed
//	GPIOC->PUPDR   &= ~GPIO_PUPDR_PUPD9; //~(3U<<(13*2));  // No pull-up, no pull-down

#if defined(STM32F401xE)
// Configure the main internal regulator output voltage 
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;
	
// The maximum frequency of the AHB, the APB1 and the APB2 domains is 80 MHz.
	RCC->CFGR &= ~RCC_CFGR_HPRE;  // AHB prescaler = 1; SYSCLK not divided
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB prescaler = 1; SYSCLK not divided
	RCC->CFGR &= ~RCC_CFGR_PPRE1; // APB high-speed prescaler (APB1) = 1, HCLK not divided
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // APB high-speed prescaler (APB1) = 1, HCLK not divided !!!42Mhz max!!!
	RCC->CFGR &= ~RCC_CFGR_PPRE2; // APB high-speed prescaler (APB2) = 1, HCLK not divided !!!84Mhz max!!!
#elif defined(STM32F407xx)
// Configure the main internal regulator output voltage 
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

// The maximum frequency of the AHB=168MHz, the APB1=42MHz and the APB2=84MHz.
	RCC->CFGR &= ~RCC_CFGR_HPRE;  // AHB prescaler = 1; SYSCLK not divided
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB prescaler = 1; SYSCLK not divided
	RCC->CFGR &= ~RCC_CFGR_PPRE1; // APB high-speed prescaler (APB1) = 1, HCLK reset
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; // APB high-speed prescaler (APB1) = 1, HCLK divided by 4 !!!42Mhz max!!!
	RCC->CFGR &= ~RCC_CFGR_PPRE2; // APB high-speed prescaler (APB2) = 1, reset 
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; // APB high-speed prescaler (APB2) = 1, HCLK fivided by 2 !!!84Mhz max!!!

#elif defined(STM32F429xx)
#elif defined(STM32F446xx)
// Configure the main internal regulator output voltage 
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;
//  enable the Over-drive mode
	PWR->CR |= PWR_CR_ODEN;
	while((PWR->CSR & PWR_CSR_ODRDY) == 0);
//  switch the voltage regulator
	PWR->CR |= PWR_CR_ODSWEN;
	while((PWR->CSR & PWR_CSR_ODSWRDY) == 0);
// To force APB1 Timer clock to 180Mhz	
	RCC->DCKCFGR |= RCC_DCKCFGR_TIMPRE;
	
	
// The maximum frequency of the AHB=180MHZ, the APB1=45MHZ and the APB2 domains is 90 MHz.
	RCC->CFGR &= ~RCC_CFGR_HPRE;  // AHB prescaler = 1; SYSCLK not divided
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB prescaler = 1; SYSCLK not divided
	RCC->CFGR &= ~RCC_CFGR_PPRE1; // APB high-speed prescaler (APB1) = 1, HCLK reset
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; // APB high-speed prescaler (APB1) = 1, HCLK divided by 4 !!!45Mhz max!!!
	RCC->CFGR &= ~RCC_CFGR_PPRE2; // APB high-speed prescaler (APB2) = 1, reset 
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; // APB high-speed prescaler (APB2) = 1, HCLK fivided by 2 !!!90Mhz max!!!
	
#elif defined(STM32F407xx)
// Configure the main internal regulator output voltage 
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

// The maximum frequency of the AHB=168MHz, the APB1=42MHz and the APB2=84MHz.
	RCC->CFGR &= ~RCC_CFGR_HPRE;  // AHB prescaler = 1; SYSCLK not divided
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB prescaler = 1; SYSCLK not divided
	RCC->CFGR &= ~RCC_CFGR_PPRE1; // APB high-speed prescaler (APB1) = 1, HCLK reset
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; // APB high-speed prescaler (APB1) = 1, HCLK divided by 4 !!!42Mhz max!!!
	RCC->CFGR &= ~RCC_CFGR_PPRE2; // APB high-speed prescaler (APB2) = 1, reset 
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; // APB high-speed prescaler (APB2) = 1, HCLK fivided by 2 !!!84Mhz max!!!
#else
 #error "Please select first the target STM32F4xx device used in your application (in stm32f4xx.h file)"
#endif	

	RCC->CR   |= RCC_CR_PLLON; 
	while((RCC->CR & RCC_CR_PLLRDY) == 0);
	
	// Select PLL selected as system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL; // 00: HSI, 01:HSE, 10: PLL, 11: Not allowed
	
	// Wait until System Clock has been selected
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);


}
