#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __unix__
      #include <unistd.h>
#elif __MSDOS__ || __WIN32__ || _MSC_VER
      #include <io.h>
#endif
//#include <wiringPiSPI.h>

#define NLED 32
#define LEN 3*NLED
#define CHANNEL 0
#define SPEED 500000
//compile with -lwiringPi 

int ret=0;
int spi=0;

unsigned char data[LEN];

int init(){
	//ret = wiringPiSPISetup (CHANNEL, SPEED); 
	if(ret!=0){
		printf("WiringPi Setup failed\n");
		return -1;
	}
	//spi = wiringPiSPIGetFd(CHANNEL);
	if(spi==NULL){
		printf("Getting SPI FD failed\n");
		return -2;
	}
	return 0;
}
void oneColor(unsigned char r, unsigned char g, unsigned char b){
	for(int i=0;i<NLED;i++){
		data[3*i] 	= r;
		data[3*i+1] = g;
		data[3*i+2] = b;
	}
}

int readControlFile(char* url){
	FILE *fd;
	int line,r,g,b;	
	char cmd[10];

	fd = fopen(url, "r");

	if(fd == NULL) {
		printf("Datei konnte NICHT geoeffnet werden.\n");
	}
	for(int i=0;i<12;i++){
		
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

	ret=init();
	if(ret=init()!=0){
		printf("Init failed\n");
		return ret;
	}
	oneColor(0,0,0);

	data[0] = 255;

	if(write(spi, data, LEN)==LEN){
		printf("Writing failed\n");
		return 2;
	}
	//readControlFile("C:\\Users\\Admin\\Desktop\\Projects\\RaspLight\\controlFile.txt");
	//getchar();
	return 0;
}

