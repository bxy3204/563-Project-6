#include <string.h>
#include <stdio.h>

#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "PWM.h"
#include "input_pa0_test.h"

int main(void)
{
	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	UART2_Init();
  PWM1_start();
  TIM2 -> CR1 &= ~(1 << 1); // Enable UEV for TIM2
	LED_Init();
	init() ;
	run() ;		// loops forever!	
}

