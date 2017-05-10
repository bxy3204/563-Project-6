/*
 * port_setup.c
 * This module sets up DIOA as output, DIOB as input and VIN4 as the analog input pin to be read on the ADC.
 *  Created on: May 3, 2017
 *      Author: bxy3204
 */
#include "port_setup.h"


/*					root_perm
 * input:none
 * output: permission failed(-1), permission obtained(0)
 * process:
 * Request root permission to access registers.
 */
int root_perm(void)
{
	privity_err = ThreadCtl( _NTO_TCTL_IO, NULL );
	if ( privity_err == -1 )
	{
		fprintf( stderr, "can't get root permissions\n" );
		return -1;
	}
	return 0;
}
/*				port_init
 * input:none
 * output:none
 * process:
 * See indivual line comments.
 * Some handles will be used in analog.c or digital.c
 */
void port_init(void)
{
	//ANALOG SETUP
	//Get a handle to the channel register. Step 1 of A/D conversion
	channel_handle = mmap_device_io( PORT_LENGTH, CHANNEL_ADDRESS);
	// Channel 4 only.
	out8(channel_handle, CH4);
	// Get a handle to the DAQ Control register
	ctrl_handle = mmap_device_io( PORT_LENGTH, CTRL_ADDRESS );
	// Setup input range of A/D to +/- 10V
	out8( ctrl_handle, INIT_BIT); //Step 2 input range
	// Get a handle to the MSB read register. Read only for MSB.
	page_handle = mmap_device_io( PORT_LENGTH, PAGE_ADDRESS );

	// Get a handle to the DAQ port's Data register
	data_handle = mmap_device_io( PORT_LENGTH, DATA_ADDRESS );
	out8(data_handle, FIFO_CLR);//reset FIFO

	//PORT A + B
	//DIO control, set up Port A as output and Port B as input
	dio_ctrl_handle = mmap_device_io( PORT_LENGTH, DIO_CTRL_ADDRESS );
	out8( dio_ctrl_handle, AOUTBIN );
	//PORT A
	 //DIO address
	dio_data_handle = mmap_device_io( PORT_LENGTH, DIO_ADDRESS );
	//PORT B

	datab_handle = mmap_device_io( PORT_LENGTH, DIOB_ADDRESS );

}


