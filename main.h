/**
 * \file      main.h
 * \brief     Interfaces for main program
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>    // For printf (icon manage run time/compiler/IO/Stdout/ITM)
#include <stdbool.h>
#include <stm32f4xx.h>
#include "SysClock.h"
#include "PinNames.h"
#include "stm32f4xx_gpio_AF.h"
#include "gpio.h"
#include "systick.h"

typedef struct {
	unsigned short nbrIT;	
	unsigned short nbrUn;
	char finReception;
	char it;
} US;

void US_Init(US *self);

void US_InterruptHandler(US *self);

void US_Management(US *self);

unsigned short US_GetDistance(US *self);

char US_NeedFocus(US *self);

char US_DataReady(US *self);

#endif /* MAIN_H */

