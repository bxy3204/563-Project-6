/*
 * digital.c
 *
 *  Created on: May 3, 2017
 *      Author: bxy3204
 */
#include "port_setup.h"


/*Run the pwm signal. Initializes servo to pos 0*/

int send_volts(int v)
{
		/* Output a byte of lows to the data lines */
		out_r = v ;
		out8( dio_data_handle, out_r );
		printf("STM32 value = %d\n", v);
		return 0;
}

int ready_to_send(void)
{

	return in8(datab_handle);
}

