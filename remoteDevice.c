#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define SERVERPORT 2019
#define SERVERADDRESS "127.0.0.1"
#define MAX_LINE_LENGTH 80

int main(int argc, char *argv[])
{
	int clientSocket;
	struct sockaddr_in serverAddr;
	size_t msglen;
	int retCode,t;
	double change;
	int prevTime=0;
	FILE *device_file;
    char line[MAX_LINE_LENGTH]; 
	char address[15]; 
	
	if (argc == 2){
			strcpy(address,argv[1]);
	}
	else 
		strcpy(address, SERVERADDRESS);
	
	
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Error in socket()");
		exit(EXIT_FAILURE);
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	serverAddr.sin_addr.s_addr = inet_addr(address);
	
	if ((connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) == -1)
	{
		perror("Error in connect()");
		exit(EXIT_FAILURE);
	}

	device_file = fopen("remoteDevice.txt", "r"); 
	
	if (device_file == NULL) {
        printf("Can't open device.txt\n");
        exit(EXIT_FAILURE);
    }

		while ( fgets(line, sizeof(line), device_file)) {
        sscanf(line, "%d %lf", &t, &change);
	    sleep(t - prevTime);
		prevTime = t;
	    printf("Client connected to %s\n", address);
	    msglen = strlen(line)+1;
	    retCode = write(clientSocket, line, msglen);
		if(retCode == 0)
			printf("Connection closed\n");
		}
	 close(clientSocket);
}