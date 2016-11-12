#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <softPwm.h>

#define NLED 33
#define LEN 3*NLED
#define CHANNEL 0
#define SPEED 500000

int ret=0;
int spi=0;

unsigned char data[LEN];

int spi(){
	
	ret = wiringPiSPISetup(CHANNEL, SPEED);
	if(ret<0){
		printf("WiringPi Setup failed. ret=%d\n",ret);
		return -1;
	}
	spi = wiringPiSPIGetFd(CHANNEL);

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
void setColor(int n, unsigned char r, unsigned char g, unsigned char b){
	data[n*3] = r;
	data[n*3+1] = g;
	data[n*3+2] = b;
}
int toInt(char* s,int max){
	return toInt(s,0,max);
}
int toInt(char* s,int min,int max){
	int n=atoi(s);
	if(n<min) n=min;
	if(n>max) n=max;
	return n;
}
void scan(){
	int pin=0;
	char cmd[10];
	char arg[3][10];
	scanf("%s",cmd);
	if(strcmp(cmd, "exit")==0){
		exit(0);
	}
	else if(strcmp(cmd, "L")==0){
		wiringPiSetup();
		scanf("%s %s",arg[0],arg[1]);
		if(atoi(arg[0])==1)
			pin=25;
		softPwmWrite(pin,atoi(arg[1]));
		printf("Schalte Pin %d auf %s/200\n",pin,arg[1]);	
	//LED schalten
	}
	else if(strcmp(cmd, "l")==0){
		spi();
		scanf("%s %s %s %s",arg[0],arg[1],arg[2],arg[3]);
		if(strcmp(arg[0], "a")==0){
			oneColor(
				toInt(arg[1],255),
				toInt(arg[2],255),
				toInt(arg[3],255)
			);
		}else{
			setColor(
				toInt(arg[0],255),
				toInt(arg[1],255),
				toInt(arg[2],255),
				toInt(arg[3],255)
			);
		}
		write(spi,data,LEN);
	
		printf("Schalte Led %s auf (%s,%s,%s)\n",arg[0],arg[1],arg[2],arg[3]);
	}else{
		printf("Unbekanntes Kommando\n");
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
				oneColor(r,g,b);
				printf("base %d %d %d\n", r, g, b);
			}else if(strcmp(cmd, "run")==0){
				write(spi,data,LEN);
				wait(2);
				printf("run\n");
			}else{
				fscanf(fd, "%d %d %d", &r, &g, &b);
				setColor(atoi(cmd),r,g,b);
				printf("->%d %d %d %d\n", atoi(cmd), r, g, b);
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[]){
	
	//pinMode(25, OUTPUT);
	//digitalWrite(25, HIGH);
	
	wiringPiSetup();
	if(softPwmCreate(25, 200, 200)!=0){
		printf("Error for PWM\n");
	}

	while(1){
		scan();
	};
	return 0;
}
