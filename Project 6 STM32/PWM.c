#include "PWM.h"
void PWM1_start(void){
		/* Set up GPIOA PA5 for alternate function connected to TIM2_CH1. High speed, Pull down */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //enable clock to GPIOA
  
	GPIOA -> MODER |= ~(0U) ; // Reset all GPIOA
	GPIOA -> MODER &= ~(1 << 2*5) ; // PA5 AF
	
	GPIOA -> OTYPER &= ~(1U<<5) ; //PA5 Push/Pull(0)
	GPIOA -> OSPEEDR |= (3U <<2*5) ; //PA5 High speed (11)
	
	GPIOA -> PUPDR &= ~(3U << 2*5) ; // PA5 no PU/PD
	GPIOA-> PUPDR |= (2U << 2*5) ; //PA5 PD
    
	GPIOA -> MODER &= ~3 ; // PA5 AF

	
	GPIOA -> PUPDR &= ~(3U) ; // PA5 no PU/PD
	GPIOA-> PUPDR |= (2U) ; //PA5 PD
	
	GPIOA -> AFR[0] |= (1U << 4 * 5) ; // PA5 attached to tim2_ch1
	/* Setup Timer 2 Channel for output. */
	RCC -> APB1ENR1 |= (1U<<0); // Clock for TIM2
    RCC -> APB1ENR1 |= RCC_APB1ENR1_TIM6EN; // Clock for TIM2
	//RCC_APB1ENR1_TIM2EN
	TIM2 -> SR &= ~(3U); // Clear update flag and input capture flag
	
	TIM2 -> PSC = 8000 ; //Reduce 80MHz system clock to .01MHz (100us period)
	
	
	TIM2 -> CCER &= ~(1U) ; //turn off output channel 1
	
	TIM2 -> CCMR1 &= ~(3U) ; // Channel 1 output
	TIM2 -> CCMR1 &= ~(6U << 4); //PWM channel 1 enable
	TIM2 -> CCMR1 |= (6U << 4) ;
	TIM2 -> CCMR1 |= (8U) ; //enable preload register
	TIM2 -> EGR |= 1U ; //Force PSC to update and reinit counter
	TIM2 -> CR1 |= (1U << 7); //Enable ARR
	TIM2 -> CCER |= (1U) ; //OC1 signal output
	TIM2 -> ARR = (200U) ; //Period of 20ms
	TIM2 -> CCR1 = (5U) ; //20ms pulse width
	TIM2 -> EGR |= 1U ; //Force PSC to update and reinit counter
	TIM2 -> CR1 |= TIM_CR1_CEN ; //enable counter
}

void PWM1_update(uint32_t width){
	TIM2 -> CCR1 = width ;
}
