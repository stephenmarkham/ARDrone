#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include "ar_drone.h"
#include <unistd.h>

using namespace std;

ar_drone drone = ar_drone(0, "192.168.1.2");


void *ARDrone_Controller(void *threadarg)
{
	drone.control();
	pthread_exit(NULL);
}

/*
 * Main Method
 * 
 */
int main ()
{	
 	pthread_t thread;

	pthread_create(&thread, NULL, ARDrone_Controller, NULL);

	sleep(5);

	drone.forward(0.1);

	sleep(2);

	drone.hover();

	sleep(2);

	drone.land();

	sleep(1);

	return 0;
}
