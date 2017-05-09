// L. Kiser Feb. 8, 2017

#include "stm32l476xx.h"
#include "LED.h"
#include "UART.h"
#include "PWM.h"
#include "input_pa0_test.h"
#include <string.h>
#include <stdio.h>

#define READY_PIN 15

int pins[] = {10,12,13,14,15};
void init( void )
{
	int i;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOHEN;		// enable clock for A group of GPIO

	for (i = 0; i < 5; i++) {
		int pin = pins[i];
		GPIOE->MODER &= ~(3 << 2*pin);				//set mode for each pin to 00 (input mode)
		
		GPIOE->PUPDR &= ~(3 << 2*pin);				//set PU/PD to 00
		GPIOE->PUPDR |= (2 << 2*pin);               //set PU/PD to 10 (PD)
	}
  
    GPIOH->MODER &= ~3;
    GPIOH->MODER |= 1;
    GPIOH->OSPEEDR |= 3;
	GPIOH->OTYPER &= ~1;	   // Push-pull
	
    GPIOH->PUPDR   &= ~3;  // No pull-up, no pull-down
    GPIOH->PUPDR  |= 2;
    GPIOH->ODR    |= 1;
}

int read_pin( int pin )
{
    // Reads the pin at PEx, where x is the pin passed as argument
    // returns 1 if high, 0 if low.
	if (GPIOE->IDR & (1 << pin)) {
		return 1;
	}
	return  0;				
																
}
void run( void )
{
	int i;
	int total;
    int timed_out = 0;
	while (!(GPIOA->IDR & 1)){} //wait until joystick is pressed

    while(1) {
        TIM2 -> SR &= ~1;
        Red_LED_Off();
        timed_out=0;
        while(!read_pin( READY_PIN ) ) {
            if ((TIM2 -> SR & 1)) {
                timed_out ++;
                TIM2 -> SR &= ~1;
                if (timed_out > 2) {
                    Red_LED_On();
                }
            }  
        }
        Red_LED_Off();

		total = 0;

		 //wait for the ready bit to be set
		
        for (i = 0; i < 4; i++) {
            total += (read_pin(pins[i]) << i);			
		}
        
        
		GPIOH->ODR &= ~1;   //Set "done" bit to 0

        //ensure input is between [0, 15]. It should be, but this is to ensure we don't go outside the servo's range
		if (total < 0) {
		  total = 0;
		}
		if (total >= 15) {
		  total = 15;
		}
        
		PWM1_update( 5 + total); //the servo accepts values from 5 - 20. conveniently, input values are between 0-15, so we can simply add 5
		
        TIM2 -> SR &= ~1;
        Red_LED_Off();
        timed_out=0;
        while(read_pin( READY_PIN ) ) {
            if ((TIM2 -> SR & 1)) {
                timed_out ++;
                TIM2 -> SR &= ~1;
                if (timed_out > 2) {
                    Red_LED_On();
                }
            }  
        }
        Red_LED_Off();
        //while(read_pin( READY_PIN )){}; //wait for the ready bit to be reset
			
		GPIOH->ODR |= 1;    //Set "done" bit to 1
	} 

	
}
uint8_t buffer[10];
char rxByte;
void printNum(int num) {
	
		int n = sprintf((char *)buffer, "%d\r\n", num);
		USART_Write(USART2, buffer, n);	
}

