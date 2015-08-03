#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include "ar_drone.h"
#include <unistd.h>

using namespace std;

ar_drone drone = ar_drone("192.168.2.1");

/*
 * Thread that ensures commands are constantly being sent to drone
 *
 */
void *ARDrone_Controller(void *threadarg)
{
	drone.control();
	pthread_exit(NULL);
}

void takeOff()
{
	
}

/*
 * Main Method
 * 
 */
int main ()
{	
 	pthread_t thread;

	pthread_create(&thread, NULL, ARDrone_Controller, NULL);
	//TIME TO ENSURE TAKEOFF IS COMPLETE
	sleep(7);
	
	//Rotate Right for 2 seconds
	drone.rotateRight(0.5);
	sleep(2);

	//Rotate Left for 2 seconds
	drone.rotateLeft(0.5);
	sleep(2);

	//Hover for 3 seconds
	drone.hover();
	sleep(3);

	//Land
	drone.land();
	

	if(pthread_join(thread, NULL)) {
      return 2;
   	}

	return 0;
}
