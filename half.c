//compile with -lwiringPi 

#include <wiringPiSPI.h>

int main(int[] argv){
	
	int channel = 0;
	int speed = 500000;
	int nLed = 32;	

	int ret = wiringPiSPISetup (channel, int speed); 
	if(ret!=0) fprintf("Error 1");
	
	unsigned char data[3*nLed];
	int len = 3*nLed;
	
	for(int i=0;i<3*nLed;i++)
		data[0] = 0;

	data[0] = 255;

	ret = wiringPiSPIDataRW (
		channel, /*unsigned char *data,*/ data, len
	);
	if(ret!=0) fprintf("Error 2");
	// or write();
}