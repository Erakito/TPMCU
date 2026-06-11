/**
 * \file      SysTick.c
 * \author    L. BOURDEL
 * \version   1.0
 * \date      2017
 * \brief     Primitives functions for System Tick
 *
 * \details   !!! Good Luck !!!
 */
 
 #include "SysTick.h"

/**
 * \brief     Configure SysTick Interrupt
 * \details   Select and configure PLL
 * \return    \e none.
 */
 void SysTick_Initialize (uint32_t ticks) {
//  __IOM uint32_t CTRL;                   /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
//  __IOM uint32_t LOAD;                   /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */
//  __IOM uint32_t VAL;                    /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
//  __IM  uint32_t CALIB;                  /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */

    SysTick->CTRL = 0;            // Disable SysTick

    SysTick->LOAD = SystemCoreClock/ticks - 1;    // Set reload register

    // Set interrupt priority of SysTick to least urgency (i.e., largest priority value)
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

    SysTick->VAL = 0;             // Reset the SysTick counter value

    // Select processor clock: 1 = processor clock; 0 = external clock
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    // Enables SysTick interrupt, 1 = Enable, 0 = Disable
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

    // Enable SysTick
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

volatile uint32_t SysTickTimeDelay;	/*!< Delay var for SysTick service */

/**
 * \brief     SysTick delay interface
 * \details   Value in milliseconds
 * \return    \e none.
 */
void Delay_ms(uint32_t nTime) {
  // nTime: specifies the delay time length
  SysTickTimeDelay = nTime;      // SysTickTimeDelay must be declared as volatile
  while(SysTickTimeDelay != 0);  // Busy wait
}

/**
 * \brief     Handler for SysTick Interrupt
 * \details   Decrement SysTickTimeDelay
 * \return    \e none.
 */
void SysTick_Handler (void) { // SysTick interrupt service routine 
  if (SysTickTimeDelay > 0)    // Prevent it from being negative 
    SysTickTimeDelay--;        // SysTickTimeDelay is a global volatile variable 
} 	
