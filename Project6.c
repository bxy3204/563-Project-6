/*
 * Project.c
 *  This project will read an analog input signal with a max amplitude of +/- 5V, and send the STM32
 *  a value representing this value between 0 and 255
 *  Created on: May 3, 2017
 *      Author: bxy3204
 */


#include "port_setup.h"
#include <math.h>


double read_v;
int lsb_out=0;
int msb_out=0;


/* ______________________________________________________________________ */
int
main( )
{
	root_perm(); //Get root permission
	port_init(); //Initialize ports
	int flag = 255;
	//Wait for STM32 to be ready to receive
	while(flag == 255)
	{
		flag = ready_to_send();
	}
	while(1){
		read_v = analog_read(); //Read analog input voltage
		//Check the amplitude of incoming signal. Input signals should be limited to a max of +/- 5 volts.
		while(read_v > 5 || read_v < -5)
		{
			printf("VOLTAGE OUT OF RANGE(%.2f)! PLEASE REDUCE INPUT SIGNAL\n", read_v);
			sleep(1);
			read_v = analog_read();
		}
		/*Convert the voltage from a +/-5V scale to a 0-10 scale and divide by .0392 to get a value between 0-255.
		 * This value represents a position on the servo controlled by the STM32.
		 * The servo position value will be sent as 2 4 bit values.
		 */
		read_v =  ((read_v+5)/0.0392f);
		lsb_out = (int)(0.5f + read_v); //Round the position value to a whole number
		msb_out = (lsb_out & 240)>>4; // 4 bit MSB of servo position
		lsb_out = lsb_out & 15; //4 bit LSB of servo position

		//send lsb
		send_volts(lsb_out);
		//send msb
		send_volts(msb_out);

	}

	return 0;
}
