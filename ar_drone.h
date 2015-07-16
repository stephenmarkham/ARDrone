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
    	void setValues(double x, double y, double z, double p);

    private:
    	int convertToInt(double f);

    private:
    	void setUpSocket();

    private:
    	void prepareForTakeOff();

    private:
    	void sendCommand(std::string com);

    public:
    	void control();

    public:
        void land();
    	

};

#endif // ar_drone_H