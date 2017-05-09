//#include "analog_in.h"
#include "port_setup.h"
#include <math.h>


double read_v;
int stm_out=0;
int x;
struct timespec start, stop;
double elapsed_time;

void comm_connection()
{
	clock_gettime( CLOCK_REALTIME, &stop);
	if(stop.tv_nsec - start.tv_nsec > 20000000)
	{
		printf("COMMUNICATION LOST\n");
	}
}

/* ______________________________________________________________________ */
int
main( )
{
	root_perm();
	port_init();
	int flag = 255;
	while(flag == 255)
	{
		flag = ready_to_send();
	}
	while(1){
		read_v = analog_read();
		while(read_v > 5 || read_v < -5)
		{
			printf("VOLTAGE OUT OF RANGE(%.2f)! PLEASE REDUCE INPUT SIGNAL\n", read_v);
			sleep(1);
			read_v = analog_read();
		}
		read_v =  ((read_v+5)/0.667f);
		stm_out = (int)(0.5f + read_v);
		flag = ready_to_send();

		//Send flag + voltage read
		send_volts(16 + stm_out);
		clock_gettime( CLOCK_REALTIME, &start) ;
		//Wait for STM32 to indicate voltage received
		while(flag == 255)
		{
			comm_connection();
			send_volts(16 + stm_out);
			flag = ready_to_send();
		}
		clock_gettime( CLOCK_REALTIME, &start) ;
		//Clear flag but continue sending voltage
		send_volts(stm_out);
		//Wait for STM32 to finish reading voltage
		while(flag == 254)
		{
			comm_connection();
			flag = ready_to_send();
		}
	}

	return 0;
}
