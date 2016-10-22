#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPiSPI.h>

#define NLED 33
#define LEN 3*NLED
#define CHANNEL 0
#define SPEED 500000
//compile with -lwiringPi 

int ret=0;
int spi=0;

unsigned char data[LEN];

int init(){
	ret = wiringPiSPISetup (CHANNEL, SPEED); 
	if(ret<0){
		printf("WiringPi Setup failed. ret=%d\n",ret);
		return -1;
	}
	spi = wiringPiSPIGetFd(CHANNEL);
		
	/*
	if(spi==NULL){
		printf("Getting SPI FD failed\n");
		return -2;
	}
	*/
	return 0;
}
void oneColor(unsigned char r, unsigned char g, unsigned char b){
	int i=0;
	for(i=0;i<NLED;i++){
		data[3*i]   = r;
		data[3*i+1] = g;
		data[3*i+2] = b;
	}
}

int readControlFile(char* url){
	FILE *fd;
	int line,r,g,b;	
	char cmd[10];
	int i=0;

	fd = fopen(url, "r");

	if(fd == NULL) {
		printf("Datei konnte NICHT geoeffnet werden.\n");
	}
	
	for(i=0;i<12;i++){
		
		if(fscanf(fd, "%s", cmd)!=EOF){
			if(strcmp(cmd, "all")==0){
				fscanf(fd, "%d %d %d", &r, &g, &b);
				//printf("base %d %d %d\n", r, g, b);
			}else if(strcmp(cmd, "run")==0){
				printf("run\n");
			}else{
				fscanf(fd, "%d %d %d", &r, &g, &b);
				//printf("->%s %d %d %d\n", &cmd, r, g, b);
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[]){
	int i=0;
	ret=init();
	if(ret=init()!=0){
		printf("Init failed\n");
		return ret;
	}
	oneColor(180,0,180);
	/*
	data[0] = (unsigned char)100;
	data[1] = (unsigned char)0;
	data[2] = (unsigned char)0;
	*/
	/*
	for(i=0;i<NLED;i++){
		data[3*i]=55;
		data[3*i+1]=55;
		data[3*i+2]=55;
		ret=write(spi, data, LEN);
		sleep(1);
	}
	*/
	ret=write(spi,data,LEN);
	printf("Debug: ret=%d, LEN=%d, spi=%d\n",ret,LEN,spi);
	if(ret!=LEN){
		printf("Writing failed. ret=%d, LEN=%d\n",ret,LEN);
		return 2;
	}
	//readControlFile("C:\\Users\\Admin\\Desktop\\Projects\\RaspLight\\controlFile.txt");
	//getchar();
	return 0;
}

