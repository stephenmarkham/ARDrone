/*
 * ar_drone.h
 *
 *
 * Stephen Markham 01/04/15 
 */

#ifndef ar_drone_H
#define ar_drone_H

class ar_drone
{

	private:
		int count;

    private:
        bool landing;

    public:
        ar_drone();

    public:
        ar_drone(char * ip);

    public:
    	void setValues(double x, double y, double z, double p);

    private:
    	int convertToInt(double f);

    private:
    	void setUpSocket();

    private:
    	void prepareForTakeOff();

    public:
    	void control();

    public:
        void land();
    
    public:
        void forward(double speed);

    public:
        void backward(double speed);

    public:
        void right(double speed);

    public:
        void left(double speed);

    public:
        void up(double speed);

    public:
        void down(double speed);

    public:
        void rotateRight(double speed);

    public:
        void rotateLeft(double speed);

    public:
        void hover();

};

#endif // ar_drone_H