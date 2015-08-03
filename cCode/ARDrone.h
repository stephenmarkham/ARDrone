/*
 * ar_drone.h
 *
 *
 * Stephen Markham 01/04/15 
 */

#ifndef ARDrone_H
#define ARDrone_H

        typedef struct ARDronerec *ARDrone;

        void ar_drone(char * ip );

    	void setValues(double x, double y, double z, double p);

    	int convertToInt(double f);

    	void setUpSocket();

    	void prepareForTakeOff();

    	void control();

        void land();

        void takeOff();

        void terminateThread();
    
        void forward(double speed);

        void backward(double speed);

        void right(double speed);

        void left(double speed);

        void up(double speed);

        void down(double speed);

        void rotateRight(double speed);

        void rotateLeft(double speed);

        void hover();

#endif // ar_drone_H