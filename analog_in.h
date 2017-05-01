/*
 * analog_in.h
 *
 *  Created on: May 1, 2017
 *      Author: bxy3204
 */

#ifndef ANALOG_IN_H_
#define ANALOG_IN_H_
#include <stdio.h>
#include <unistd.h>       /* for sleep() */
#include <stdint.h>       /* for uintptr_t */
#include <hw/inout.h>     /* for in*() and out*() functions */
#include <sys/neutrino.h> /* for ThreadCtl() */
#include <sys/mman.h>     /* for mmap_device_io() */

/* The Neutrino IO port used here corresponds to a single register, which is
 * one byte long */
#define PORT_LENGTH 1

/* The base port for the DAQ register is 0x280. */
#define DATA_ADDRESS 0x280 //base,
#define CTRL_ADDRESS 0x283 //base + 3
#define CHANNEL_ADDRESS 0x282 // base + 2
#define PAGE_ADDRESS 0x281 //base +1

int privity_err;
//Handles for DAQ register
uintptr_t ctrl_handle;
uintptr_t data_handle;
uintptr_t channel_handle;
uintptr_t page_handle ;
uintptr_t polarity_handle ;

int lsb;
int count;
int msb;
short ad_total;
double voltage;

void analog_init(void);

double analog_read(void);
#endif /* ANALOG_IN_H_ */
