#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "linkingList.h"
#include "output.h"
#include "globalVariables.h"
#include "communication.h"
#include "threads.h"

#define MAX_LINE_LENGTH 80
#define MAXBUF 32
#define SERVERPORT 2019
#define SERVERADDRESS "127.0.0.1"

int count1=0;
int count2=0;
int serverSocket;
pthread_mutex_t devInp = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t devPos = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notEmpty=PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmptyViewer=PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmptyController=PTHREAD_COND_INITIALIZER;
bool notFinish=true;
struct node *deviceInput=NULL;
struct node *devicePosition=NULL;
struct node *remoteDeviceInput=NULL;


void remoteDeviceFunc( void *arg){       //THREAD REMOTE INTERFACE
    int connectSocket;                                
 	int returnCode;
	socklen_t clientAddrLen;
	struct sockaddr_in serverAddr, clientAddr;
	char buffer[MAXBUF];
	int t;
	double change;
	
	if (((serverSocket = socket(AF_INET,SOCK_STREAM,0))) == -1){
		perror("Error in server socket()");
		exit(EXIT_FAILURE);
	}
		
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVERADDRESS);

	
	if ((returnCode = bind (serverSocket, (struct sockaddr*)  &serverAddr, sizeof(serverAddr))) == -1){
		perror("Error in server socket bind()");
		pthread_exit(NULL);
	}

	if ((returnCode = listen(serverSocket, 1)) == -1){
		perror("Error in server socket listen()");
		pthread_exit(NULL);
	}	
	
	printf("Server ready\n");
	clientAddrLen = sizeof(clientAddr);
		if ((connectSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) == -1){
			printf("Connection closed, remote interface closed\n");
		    pthread_cond_signal(&notEmpty);
		    pthread_exit(NULL);
		}
		
		remoteOff =false;
		
		while ((returnCode = read(connectSocket, buffer, MAXBUF-1)) > 0  && notDone){
		buffer[returnCode] = '\0';
		sscanf(buffer, "%d %lf", &t, &change);
		pthread_mutex_lock(&devInp);
		remoteDeviceInput = addToList(remoteDeviceInput, change, t);
		pthread_mutex_unlock(&devInp);
		pthread_cond_signal(&notEmpty);
		}
		close(connectSocket);
		
		remoteOff=true;
		if(!notFinish)
	    notDone = false;
	    notFinish=false;
		printf("Connection closed, remote interface closed\n");
		pthread_cond_signal(&notEmpty);
		pthread_exit(NULL);
}


void viewerFunc(void *inp){                           //THREAD VIEWER
	Input* input= inp;
	Output out;
	
	while(notDone){
	viewerComm(&out);
	checkPosition(inp, &out);
	printOutputViewer(&out);
	sleep(input->cycleTimeViewer);
	}
	
	printf("Viewer Thread closed\n");
	pthread_exit(NULL);
	
}

void controllerFunc( void *inp){                   //THREAD CONTROLLER
	Input* input= inp;
	FILE *output_file = fopen("output.txt", "w"); 
	Output out;
	
	while(notDone){
    controllerComm(&out);
	printOutputController(output_file, &out, inp);
	sleep(input->cycleTimeController);
	}
	fclose(output_file);
	
	printf("Controller Thread closed\n");
	pthread_exit(NULL);
}

void interfaceFunc(void *arg){                         //THREAD INTERFACE
	FILE *device_file;
    char line[MAX_LINE_LENGTH]; 
    
    device_file = fopen("device.txt", "r"); 
    
    if (device_file == NULL) {
        printf("Can't open device.txt\n");
        exit(EXIT_FAILURE);
    }
	
    int t;
	int prevTime=0;
    double change;
	
    while ( fgets(line, sizeof(line), device_file)  && notDone) {
        sscanf(line, "%d %lf", &t, &change);
		sleep(t - prevTime);
		pthread_mutex_lock(&devInp);
		deviceInput = addToList(deviceInput, change, t);
		pthread_mutex_unlock(&devInp);
		pthread_cond_signal(&notEmpty);
		prevTime = t;
		
		}
    fclose(device_file);
	if(!notFinish)
	notDone = false;
    notFinish=false;
	pthread_cond_signal(&notEmpty);	
	printf("Interface Thread closed\n");
	pthread_exit(NULL);
}	


void modelFunc( void *arg){	                                  //THREAD MODEL
Positions pos;
pos.lastLocalTime = 0;
pos.lastRemoteTime = 0;
pos.localPosition = 0;
pos.remotePosition = 0;
	while(notDone){
		modelComm(&pos);
	}
	printf("Model Thread closed\n");
	pthread_exit(NULL);
		
}  