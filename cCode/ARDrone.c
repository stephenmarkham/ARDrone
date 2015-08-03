/*
* ARDrone.c
* 
* Control ARDrone using c
*
* Stephen Markham 01/04/15 
* 
* to use create a new thread and run control method from within (has to keep sending commands)
* then can use set values from main thread to control movements
*
* currently speeds have to be sent in as + or - 0.05,0.1,0.2 or 0.5
*/

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#include "ARDrone.h"

//Print Commands to std out?
#define printCommand

//Socket Things
//int socketDescriptor;
//struct sockaddr_in serverAddress;
//unsigned short int serverPort;

//IP Address of the ARDrone
//const char * IP_ADDRESS = "192.168.1.1";
//unsigned short int IP_PORT = 5556;

struct sockaddr_in myAddress;
int socketNum;
int IP_PORT = 5556;
//IP Address of the ARDrone
const char * IP_ADDRESS = "192.168.1.1";

double roll = 0;
double altitude = 0;
double pitch = 0;
double yaw = 0;

int count = 0;

int landing = 0;
int takingOff = 0;
int terminate = 0;
/*
 * Constructor
 *
 * Creates ar_drone
 *
 */
 /*
ar_drone()
{
	printf("Setting Up Drone\n");
	setUpSocket();
	count = 0;
}
*/

/*
 * Constructor with IP Address Specified
 *
 * Creates ar_drone and takes char * ip address as argument
 */
void ar_drone(char * ip)
{
	IP_ADDRESS = ip;
	printf("Setting Up Drone\n");
	setUpSocket();
	count = 0;
}

/*
 * sendCommand
 *
 * Sends a command to the ARDrone
 *
 */
void sendCommand(char *com)
{
	printf("%s\n", com);
	/*
	if (sendto(socketDescriptor, com, strlen(com), 0, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		printf("Failed To Send\n\n\n");
		exit(EXIT_FAILURE);
	}
	*/

	if(sendto(socketNum, com, strlen(com), 0, (struct sockaddr *)&myAddress, sizeof(myAddress))!=strlen(com)){
      	perror("Mismatch in number of bytes sent");
      	exit(EXIT_FAILURE);
  	}
}

/*
 * setValues
 *
 * Set the control values for the drone manually
 *
 */
void setValues(double r, double a, double p, double y)
{
	roll = r;
	altitude = a;
	pitch = p;
	yaw = y;
}


/*
 * A variety of control methods, each is a direction
 * and takes a speed (+ or - 0.05,0.1,0.2 or 0.5) as an argument
 */

void forward(double speed)
{
	pitch = -speed;
}

void backward(double speed)
{
	pitch = speed;
}

void right(double speed)
{
	roll = speed;
}

void left(double speed)
{
	roll = -speed;
}

void rotateRight(double speed)
{
	yaw = speed;
}

void rotateLeft(double speed)
{
	yaw = -speed;
}

void up(double speed)
{
	altitude = speed;
}

void down(double speed)
{
	altitude = -speed;
}

void hover()
{
	roll = 0;
	altitude = 0;
	pitch = 0;
	yaw = 0;
}


/*
 * convertToInt
 *
 * Method to take in movement parameter and convert to 32 bit int
 *
 * Currently a look up table, need to write correct method
 *
 * @param double, number to be converted to int
 * @return integer value
 *
 */
int convertToInt(double f)
{	
	if (f == 0.05) return (int)1028443341;
	else if (f == 0.1) return (int)1036831949;
	else if (f == 0.2) return (int)1045220557;
	else if (f == 0.5) return (int)1056964608;
	else if (f == -0.05) return (int)-1119040307;
	else if (f == -0.1) return (int)-1110651699;
	else if (f == -0.2) return (int)-1102263091;
	else if (f == -0.5) return (int)-1090519040;
	else return 0;	
}

/*
 * setUpSocket
 *
 * sets up a socket for connection to ARDrone
 *
 */
void setUpSocket()
{	
	memset(&myAddress, 0, sizeof(myAddress));
   	myAddress.sin_family=AF_INET;

	myAddress.sin_addr.s_addr=htonl(INADDR_ANY);

   	myAddress.sin_port=htons(IP_PORT);

   	if((socketNum=socket(AF_INET, SOCK_DGRAM, 0))<0) {
      perror("Socket Failed");
      exit(EXIT_FAILURE);
   	}

   	if(bind(socketNum,( struct sockaddr *) &myAddress, sizeof(myAddress))<0) {
      perror("Failed to Bind");
      exit(EXIT_FAILURE);
   	}

   	inet_pton(AF_INET,IP_ADDRESS,&myAddress.sin_addr.s_addr);

   	myAddress.sin_port=htons(IP_PORT);

   	sleep(1);
 }

/*
 * prepareForTakeOff
 *
 * Prepares the ARDrone for takeoff 
 * and sends the take off command
 *
 */
 void prepareForTakeOff()
{
	count = 1;
	landing = 0;
 	
 	char s[15] = "AT*FTRIM=1,\r";
  	sendCommand(s);
  	count ++;

  	char p[25] = "AT*REF=2,290718208\r";
	sendCommand(p);

	printf("Taking Off\n");
}

/*
 * land
 *
 * Call method to land ARDrone.
 *
 */
void land()
{
	landing = 1;
}

/*
 * takeOff
 *
 * Call method to takeOff.
 *
 */
void takeOff()
{
	takingOff = 1;
}

/*
 * control
 *
 * control thread that runs constantly
 * sending controls every 50ms (required by the ARDrone)
 *
 */
void control()
{  	
	printf("Set Up Complete\nWaiting for takeoff command\n");
	while(takingOff == 0){

	}
   	prepareForTakeOff();

	while(1){
		count = count + 1;
		char s[50] = "";

		int xM = convertToInt(roll);
		int yM = convertToInt(altitude);
		int zM = convertToInt(pitch);
		int pM = convertToInt(yaw);

		//If Landing
		if (landing){
			char str1[50] = "AT*REF=";
			char str2[50] = ",290717696\r";
			sprintf(s, "%s%d%s", str1, count, str2);
			printf("Landing\n");
			sendCommand(s);
			//Sleep to ensure command was sent
			sleep(1);
			exit(EXIT_FAILURE);

		}else{
			char str1[50] = "AT*PCMD=";
			if (roll == 0 && pitch == 0 && altitude == 0 && yaw == 0){
				sprintf(s, "%s%d,0,%d,%d,%d,%d\r", str1, count, xM, zM, yM, pM);
			}else{
				sprintf(s, "%s%d,1,%d,%d,%d,%d\r", str1, count, xM, zM, yM, pM);
			}
		}
		sendCommand(s);
		usleep(50000);
	}
}