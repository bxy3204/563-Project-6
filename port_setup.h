/*
 * port_setup.h
 * Define constants for Project 6
 *  Created on: May 3, 2017
 *      Author: bxy3204
 */
#include <unistd.h>
#include <stdint.h>       /* for uintptr_t */
#include <hw/inout.h>     /* for in*() and out*() functions */
#include <sys/neutrino.h> /* for ThreadCtl() */
#include <sys/mman.h>     /* for mmap_device_io() */
#include <time.h>
#include <stdio.h>
#include <pthread.h>

#ifndef PORT_SETUP_H_
#define PORT_SETUP_H_

#define DIOB_ADDRESS 0x289  //Address  for DIOB

void portb_init(); //Port setup function
int ready_to_send(); //STM32 is ready to receive flag

//PORT A

#define DIO_ADDRESS 0x288
#define DIO_CTRL_ADDRESS 0x28B
#define CH4	0x44
#define FIFO_CLR 0x10
#define AOUTBIN 0x02
#define INIT_BIT 0x00

#define LOW 0L
#define HIGH ~0L


void send_volts() ;
void port_init();
int root_perm();

//analog

#define PORT_LENGTH 1

 //The base port for the DAQ register is 0x280.
#define DATA_ADDRESS 0x280 //base,
#define CTRL_ADDRESS 0x283 //base + 3
#define CHANNEL_ADDRESS 0x282 // base + 2
#define PAGE_ADDRESS 0x281 //base +1

int privity_err;
//Handles for DAQ register
uintptr_t ctrl_handle;
uintptr_t dio_ctrl_handle;
uintptr_t data_handle;
uintptr_t dio_data_handle;
uintptr_t channel_handle;
uintptr_t page_handle ;
uintptr_t polarity_handle ;
uintptr_t datab_handle;
//Digital variables
int out_r;
int lsb;
int count;
int msb;
short ad_total;
double voltage;

void analog_init(void);

double analog_read(void);

#endif /* PORT_SETUP_H_ */
