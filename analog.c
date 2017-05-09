/*
 * analog.c
 *
 *  Created on: May 3, 2017
 *      Author: bxy3204
 */

#include "port_setup.h"

 /* analog_read:
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

		 //Output a byte of highs to the data lines
		lsb = in8(data_handle);  //LSB of a/d FIFO
		msb = in8(page_handle); // MSB of a/d FIFO
		ad_total = (msb * 256 + lsb) ; //16 Bit value of input voltage
		voltage = (ad_total/32768.0f * 10); //Convert to volts
		//printf( "MSB:%d          LSB:%d		total:%.2fV\n",msb, lsb, voltage );
		return voltage;

}
