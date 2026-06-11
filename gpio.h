/**
 * \file      gpio.h
 * \brief     Interfaces for I/O
 */


#ifndef GPIO_H
#define GPIO_H

#include "main.h"

// ************************************************************************************
// *********************************** ENUM *******************************************
// ************************************************************************************

// Pin Function
/**
 * \brief     I/O direction and function
 * \details   Used for config I/O.
 */
 typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT = 1,
    PIN_ALTERNATE = 2,
    PIN_ANALOG = 3,
}GpioPinFunctionEnum;

// Output Type
/**
 * \brief     Output Type
 * \details   Used for config I/O.
 */
typedef enum {
    OUTPUT_PUSHPULL          = 0,
		OUTPULL_OUTPUT_DEFAULT		 = OUTPUT_PUSHPULL,
    OUTPUT_OPENDRAIN         = 1
}GpioOutputTypeEnum;

// Output Speed
/**
 * \brief     Output Speed
 * \details   Used for config I/O.
 */
typedef enum {
    OUTPUT_SPEED_LOW          	= 0,
		OUTPUT_SPEED_DEFAULT    		= OUTPUT_SPEED_LOW,
    OUTPUT_SPEED_MEDIUM         = 1,
    OUTPUT_SPEED_FAST         	= 2,
    OUTPUT_SPEED_HS             = 3
}GpioOutputSpeedEnum;

// Input Type
/**
 * \brief     Pull-Up Pull-Out config
 * \details   Used for config I/O.
 */
typedef enum {
    INPUT_NO_PULLUP_PULLDOWN          = 0,
    INPUT_PUPDR_DEFAULT        				= INPUT_NO_PULLUP_PULLDOWN,
    INPUT_PULL_UP        					    = 1,
    INPUT_PULL_DOWN     					    = 2
}GpioPullUpDownEnum; 

// ************************************************************************************
// ********************************** STRUCT*******************************************
// ************************************************************************************

// ************************************************************************************
// ********************************** DEFINE ******************************************
// ************************************************************************************


// ************************************************************************************
// ********************************* FUNCTION *****************************************
// ************************************************************************************

// Config
/* ##################################### Debug In/Output function ########################################### */
/**
  \defgroup GpioGroup 01 - GPIO Interface
  \brief    Functions to manipulate I/O. See PinNameEnum definition in PinNames.h.
  @{
 */
bool GPIOx_Pin_Configure(PinNameEnum      ,
												 GpioPinFunctionEnum  , 
												 GpioOutputTypeEnum   , 
												 GpioOutputSpeedEnum  ,
												 GpioPullUpDownEnum  );

bool GPIOx_PinAF (PinNameEnum ,
                 uint8_t    );

void GPIOx_Set_Output_Pin(PinNameEnum );

void GPIOx_Reset_Output_Pin(PinNameEnum );
	
void GPIOx_Togle_Output_Pin(PinNameEnum );
	
void GPIOx_Set_Output_Port(GPIO_TypeDef* , uint16_t , uint16_t );

bool GPIOx_Read_Input_Pin(PinNameEnum );
/**@} end of GPIO interface */
bool GPIO_PinAF(uint8_t PinId,
                 uint8_t AlternateFunction,
								 uint32_t AFR_Register[2]	);

void GPIO_Togle_Output_Pin( uint8_t PinId, uint32_t *ODR_Register);

#endif
