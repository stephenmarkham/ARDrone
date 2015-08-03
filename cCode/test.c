#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ARDrone.h"
#include <pthread.h>

// ----- Required ---------------------------------
void *DroneControl()
{

   char ip[50] = "192.168.2.1";
   ar_drone(ip);
   control();

   return NULL;
}
// ------------------------------------------------

int main ()
{
   // ----- Required Code -------------------------
   pthread_t ControlThread;
   if(pthread_create(&ControlThread, NULL, DroneControl, NULL)) {
      fprintf(stderr, "Error creating thread\n");
      return 1;
   }
   // ---------------------------------------------

   sleep(5);

   takeOff();

   sleep(5);

   rotateRight(0.5);

   sleep(2);

   hover();

   sleep(3);

   land();

   //terminateThread();

   // ---- Stops Programme Terminating early by waiting for control thread to finish
   if(pthread_join(ControlThread, NULL)) {
      return 1;
   }

   return 0;
}