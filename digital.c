/*
 * digital.c
 *	This module controls the digital in and out of Project 6.
 *	DIOA0-DIOA3(output) are the data lines, DIOA4(output) is the control too the STM32 and DIOB(input) is the control line from the STM32.
 *  Created on: May 3, 2017
 *      Author: bxy3204
 */
#include "port_setup.h"
struct timespec start, stop;
int flag = 255;
//Check time elapsed between control signal level change
void comm_connection()
{
	clock_gettime( CLOCK_REALTIME, &stop);
	if(stop.tv_nsec - start.tv_nsec > 20000000)
	{
		printf("COMMUNICATION LOST\n");
	}
}
/*						send_volts
 * 	This function sends a 4 bit value to the STM32.
 * input: a 4 bit voltage or value
 * output:none
 * process:
 * 1. Send the STM32 the input value(v) plus a control line indicating a new value has been sent.
 * 2. Monitor the input flag from the STM32, and indicate a timeout after 20ms. Do not proceed untill STM32 has indicated it is ready to recieve a value.
 * 3. Clear the control line to the STM32, but continue sending the input value(v).
 * 4. Monitor the input flag from the STM32, and indicate a timeout after 20ms. Do not proceed untill STM32 has indicated it is has recieved a value.
 */

void send_volts(int v)
{
		flag = ready_to_send();
		/* Output a byte of lows to the data lines */
		out_r =16 + v ;
		out8( dio_data_handle, out_r );
		clock_gettime( CLOCK_REALTIME, &start) ;
		//Wait for STM32 to indicate voltage received
		while(flag == 255)
		{
			comm_connection();
			flag = ready_to_send();
		}
		clock_gettime( CLOCK_REALTIME, &start) ;
		//Clear flag but continue sending voltage
		out_r = v ;
		out8( dio_data_handle, out_r );
		//Wait for STM32 to finish reading voltage
		while(flag == 254)
		{
			comm_connection();
			flag = ready_to_send();
		}
}
/*				ready_to_send
 * input:none
 * output: Value of DIOB block of the DAQ.
 * process: Return the value from DIOB
 */
int ready_to_send(void)
{
	return in8(datab_handle);
}
