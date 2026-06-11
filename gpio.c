/**
 * \file      gpio.c
 * \brief     Primitives functions for GPIO
 */

#include "gpio.h"


/**
 * \brief     Configure I/O pin
 * \details   
 * \param			PinId : Id enumeration see PinNames.h
 * \param			moder : GPIO Mode
 * \param			output_type : GPIO Output Type
 * \param			output_speed : GPIO Speed
 * \param			pull_up_down : GPIO Push-Pull
 * \return    \e boolean.
 */
 
//  Définition de la fonction  GPIOx_Pin_Configure
bool GPIOx_Pin_Configure(PinNameEnum      PinId,  // par exemple : PC_5
                       GpioPinFunctionEnum  moder,	// possible GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
																											
                       GpioOutputTypeEnum   output_type,		//possible GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
																											
                       GpioOutputSpeedEnum  output_speed,		// possibleGPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
                       GpioPullUpDownEnum  pull_up_down) 	//possible GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
{
	GPIO_TypeDef* GPIOx;
												 
	int PinNum;
	int PortNum;
	// Get Port Number and Pin number for this Port Number
											 
	PinNum=GET_PIN_INDEX(PinId);
	PortNum=GET_PORT_INDEX(PinId);

// Test Port Number and Pin Number

	if( PinNum >= 16) return false;
	if( PortNum >= 9) return false;
	// Enable GPIO clock
	RCC->AHB1ENR |= (0x1U << PortNum);
	

// init registers according to parameters
	GPIOx=(GPIO_TypeDef*)(GPIOA_BASE+(PortNum*0x0400U)); // uint_fast16_t = unsigned int
  GPIOx->MODER   = (GPIOx->MODER   & ~(0x3 << (2*PinNum))) | (moder         << (2*PinNum));
  GPIOx->OTYPER  = (GPIOx->OTYPER  & ~(0x1 <<    PinNum))  | (output_type  <<    PinNum);
  GPIOx->OSPEEDR = (GPIOx->OSPEEDR & ~(0x3 << (2*PinNum))) | (output_speed << (2*PinNum));
  GPIOx->PUPDR   = (GPIOx->PUPDR   & ~(0x3 << (2*PinNum))) | (pull_up_down << (2*PinNum));

  return true;

	
	
}

/**
 * \brief     Configure I/O as Alternate Function for peripheral
 * \details   
 * \param			PinId : Id enumeration see PinNames.h
 * \param  		AlternateFunction : See stm32f4xx_gpio_AF.h & Excel file for Pin peripheral functionnality 
 * \return    \e boolean.
 */
bool GPIOx_PinAF (PinNameEnum      PinId,
                 uint8_t    AlternateFunction) {

	GPIO_TypeDef* GPIOx;
												 
	int PinNum=GET_PIN_INDEX(PinId);
		 
	if( IS_GPIO_AF(AlternateFunction)!=true)
		while(1);
												 
	GPIOx=(GPIO_TypeDef*)(GPIOA_BASE+(GET_PORT_INDEX(PinId)*0x0400U));

  if ( PinNum >= 16) return false;
  if (PinNum >= 8) {
		PinNum%=8;
    GPIOx->AFR[1] = (GPIOx->AFR[1] & ~(0xF << (4*PinNum))) | (AlternateFunction << (4*PinNum));
  } else {
    GPIOx->AFR[0] = (GPIOx->AFR[0] & ~(0xF << (4*PinNum))) | (AlternateFunction << (4*PinNum));
  }

  return true;
}

/**
 * \brief     Force output pin to '1'
 * \details   
 * \param			PinId : Id enumeration see PinNames.h
 * \return    \e none.
 */
void GPIOx_Set_Output_Pin(PinNameEnum PinId)
{
	GPIO_TypeDef* GPIOx;
												 
	GPIOx=(GPIO_TypeDef*)(GPIOA_BASE+(GET_PORT_INDEX(PinId)*0x0400U));
	
	GPIOx->ODR|= (0x1 << GET_PIN_INDEX(PinId));
}

/**
 * \brief     Force output pin to '0'
 * \details   
 * \param			PinId : Id enumeration see PinNames.h
 * \return    \e none.
 */
void GPIOx_Reset_Output_Pin(PinNameEnum PinId)
{
	GPIO_TypeDef* GPIOx;
	
	GPIOx=(GPIO_TypeDef*)(GPIOA_BASE+(GET_PORT_INDEX(PinId)*0x0400U));

  GPIOx->ODR  = (GPIOx->ODR  & ~(0x1 << GET_PIN_INDEX(PinId)));
}

/**
 * \brief     Toggle output pin between VCC and GND
 * \details   
 * \param			PinId : Id enumeration see PinNames.h
 * \return    \e none.
 */
void GPIOx_Togle_Output_Pin(PinNameEnum PinId)
{
	GPIO_TypeDef* GPIOx;
											
	GPIOx=(GPIO_TypeDef*)(GPIOA_BASE+(GET_PORT_INDEX(PinId)*0x0400U));
	
	GPIOx->ODR^= (0x1 << GET_PIN_INDEX(PinId));
}

/**
 * \brief     Set output PORT I/O
 * \details   val  : value to set
 * \details   mask : modify only bit specified by mask
 * \return    \e none.
 */
void GPIOx_Set_Output_Port(GPIO_TypeDef* GPIOx, uint16_t val, uint16_t mask)
{	
	GPIOx->ODR  = (GPIOx->ODR  & ~mask)  | val;
}

/**
 * \brief     Read input status
 * \details   val  : value to set
 * \param			PinId : Id enumeration see PinNames.h
 * \return    \e boolean : 1 or 0.
 */
bool GPIOx_Read_Input_Pin(PinNameEnum PinId)
{
	GPIO_TypeDef* GPIOx;
	
	GPIOx=(GPIO_TypeDef*)(GPIOA_BASE+(GET_PORT_INDEX(PinId)*0x0400U));

  return (GPIOx->IDR & (0x01<<GET_PIN_INDEX(PinId)) );
}

/**
 * \brief     Toggle GPIO output
 * \param			PinId : between 0 to 15
 * \param			ODR_Register : register to modify to toggle output
 * \return    \e none.
 */
void GPIO_Togle_Output_Pin( uint8_t PinId, uint32_t* ODR_Register)
{	
	*ODR_Register^= (0x1 << PinId);
}

