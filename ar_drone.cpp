/*
* ar_drone.cpp
* 
* Control ARDrone using c++
*
* Stephen Markham 01/04/15 
* 
* to use create a new thread and run control method from within (has to keep sending commands)
* then can use set values from main thread to control movements
*
* currently speeds have to be sent in as + or - 0.05,0.1,0.2 or 0.5
*/
#include "ar_drone.h"
#include <pthread.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iomanip>
#include <unistd.h>
#include <sstream>

using namespace std;

//Print Commands to std out?
#define printCommand

//Socket Things
struct sockaddr_in myAddress;
int socketNum;
int IP_PORT = 5556;
//IP Address of the ARDrone
const char * IP_ADDRESS = "192.168.1.1";

double roll = 0;
double altitude = 0;
double pitch = 0;
double yaw = 0;

/*
 * Constructor
 *
 * Creates ar_drone
 *
 */
ar_drone::ar_drone()
{
	cout << "Setting up ar_drone..." << endl;
	ar_drone::setUpSocket();
	ar_drone::count = 0;
}


/*
 * Constructor with IP Address Specified
 *
 * Creates ar_drone and takes char * ip address as argument
 */
ar_drone::ar_drone(char * ip)
{
	IP_ADDRESS = ip;
	cout << "Setting up ar_drone..." << endl;
	ar_drone::setUpSocket();
	ar_drone::count = 0;
}

/*
 * sendCommand
 *
 * Sends a command to the ARDrone
 *
 */
void sendCommand(string com)
{

	#ifdef printCommand 
		cout << com << endl;
	#endif
	if(sendto(socketNum, com.c_str(), com.size(), 0, (struct sockaddr *)&myAddress, sizeof(myAddress))!=com.size()){
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
void ar_drone::setValues(double r, double a, double p, double y)
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

void ar_drone::forward(double speed)
{
	pitch = -speed;
}

void ar_drone::backward(double speed)
{
	pitch = speed;
}

void ar_drone::right(double speed)
{
	roll = speed;
}

void ar_drone::left(double speed)
{
	roll = -speed;
}

void ar_drone::rotateRight(double speed)
{
	yaw = speed;
}

void ar_drone::rotateLeft(double speed)
{
	yaw = -speed;
}

void ar_drone::up(double speed)
{
	altitude = speed;
}

void ar_drone::down(double speed)
{
	altitude = -speed;
}

void ar_drone::hover()
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
int ar_drone::convertToInt(double f)
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
void ar_drone::setUpSocket()
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
 void ar_drone::prepareForTakeOff()
{
	count = 1;
	landing = false;
	string s;
	std::ostringstream commandBuilder;
 	
 	commandBuilder << "AT*FTRIM=" << count << "," << "\r";
  	s = commandBuilder.str();
  	sendCommand(s);
  	count ++;

	commandBuilder << "AT*REF=" << count << "," << "290718208\r";
	s = commandBuilder.str();
	sendCommand(s);

	cout << "Taking Off..." << endl;
}

/*
 * land
 *
 * Call method to land ARDrone.
 *
 */
void ar_drone::land()
{
	landing = true;
}


/*
 * control
 *
 * control thread that runs constantly
 * sending controls every 50ms (required by the ARDrone)
 *
 */
void ar_drone::control()
{  	
	cout << "Setup Complete" << endl;
	string s;
   	ostringstream commandBuilder;

   	ar_drone::prepareForTakeOff();

	while(1){
		string p;
		ostringstream command;
		ar_drone::count = ar_drone::count + 1;

		int xM = ar_drone::convertToInt(roll);
		int yM = ar_drone::convertToInt(altitude);
		int zM = ar_drone::convertToInt(pitch);
		int pM = ar_drone::convertToInt(yaw);

		//If Landing
		if (ar_drone::landing){
			command << "AT*REF=" << count << "," << "290717696\r";
			cout << "Landing" << endl;
			p = command.str();
			sendCommand(p);
			//Sleep to ensure command was sent
			sleep(1);
			exit(EXIT_FAILURE);

		}else{
			if (roll == 0 && pitch == 0 && altitude == 0 && yaw == 0){
				command << "AT*PCMD=" << ar_drone::count << ",0," << xM << "," << zM << "," << yM << "," << pM << "\r";
			}else{
				command << "AT*PCMD=" << ar_drone::count << ",1," << xM << "," << zM << "," << yM << "," << pM << "\r";
			}
		}
		p = command.str();
		sendCommand(p);
		usleep(50000);
	}
}