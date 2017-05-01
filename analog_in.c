/*
 * analog_in.c
 *
 *  Created on: May 1, 2017
 *      Author: bxy3204
 */

#include "analog_in.h"
//TODO: Define constants in header.
/*
 * analog_int:
 * This function sets up the helios box to perform an A/D conversion
 * on an analog input signal with max amplitude of +/- 5V.
 * Pin 43 of the DAQ(J17, VIN4) connector is selected as the input pin.
 * return: none
 */
void analog_init(void)
{

	/* Give this thread root permissions to access the hardware */
	privity_err = ThreadCtl( _NTO_TCTL_IO, NULL );
	if ( privity_err == -1 )
	{
		fprintf( stderr, "can't get root permissions\n" );
	}
	//Get a handle to the channel register. Step 1 of A/D conversion
	channel_handle = mmap_device_io( PORT_LENGTH, CHANNEL_ADDRESS);
	// Channel 4 only.
	out8(channel_handle, 0x44);
	/* Get a handle to the DAQ Control register */
	ctrl_handle = mmap_device_io( PORT_LENGTH, CTRL_ADDRESS );
	/* Setup input range of A/D to +/- 5V */
	out8( ctrl_handle, 0x01); //Step 2 input range
	// Get a handle to the MSB read register. Read only for MSB.
	page_handle = mmap_device_io( PORT_LENGTH, PAGE_ADDRESS );

	/* Get a handle to the DAQ port's Data register */
	data_handle = mmap_device_io( PORT_LENGTH, DATA_ADDRESS );
	out8(data_handle, 0x10);//reset FIFO
}
/*
 * analog_read:
 * Obtain a reading from the A/D on the helios box. This function waits for the a/d to settle
 * then initiates a conversion. Before reading data, the function will wait until the a/d has completed
 * the conversion. At most, this will take 5us. After the conversion is complete, the LSB of the FIFO is read
 * (reading the LSB first is necessary, reading the MSB first will clear the LSB register).
 * The MSB and LSB are combined to create the 16 bit digital value representing the analog voltage.
 * To get the analog voltage, the digital value is divided by 32768, and multiplied by the max input voltage.
 * In this case, the max voltage is 5V.
 *
 * one LSB = 153uV
 */
double analog_read(void)
{

		while (in8(channel_handle) & 0x20); //let a/d settle
		out8(data_handle, 0x80); // Trigger a/d conversion.

		while (in8(ctrl_handle) & 0x80); // wait for a/d conversion to finish. ~5us

		/* Output a byte of highs to the data lines */
		lsb = in8(data_handle);  //LSB of a/d FIFO
		msb = in8(page_handle); // MSB of a/d FIFO
		ad_total = (msb * 256 + lsb) ; //16 Bit value of input voltage
		voltage = 5 + (ad_total/32768.0f * 5); //Convert to volts
		int i = 0;
		char stars[11] = {0};
		for (;i<10;i++) {
			if (i>voltage) {
				stars[i] = ' ';
			} else {
				stars[i] = '*';
			}
		}
		stars[10] = '\0';
		printf("%s\n",stars);
		return voltage;
//		printf( "MSB:%d          LSB:%d		total:%.2fV\n",msb, lsb, voltage );
}
