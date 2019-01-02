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


bool notDone = true;
bool remoteOff = true;
static sigset_t signal_mask;

static void handlerFunc(void *arg){
    int sigCaught; 
    int resp = sigwait (&signal_mask, &sigCaught);
	if(resp |= 0){
		perror("Handler error\n");
		exit(EXIT_FAILURE);
	}
	
	if(sigCaught == SIGINT){
	   notDone = false;
	   close(serverSocket);
	}
	pthread_exit(NULL);
}
	


int main(int argc, char **argv){
	pthread_t Interface, Model, Viewer, Controller, Handler, RemoteInterface;
	
	Input input;
	
	parseInput(&argc, argv, &input);
	
	sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGINT);
	
	if(pthread_sigmask (SIG_BLOCK, &signal_mask, NULL) != 0){
		perror("Error while masking Sigint");
		exit(EXIT_FAILURE);
	}
	
	if (pthread_create(&Handler, NULL, (void*) handlerFunc, 0) != 0){
		perror("Thread handler not created");
		exit(EXIT_FAILURE);
    }
		
	if (pthread_create(&RemoteInterface, NULL, (void*) remoteDeviceFunc, 0) != 0){
		perror("Thread remote interface not created");
		exit(EXIT_FAILURE);
    }
	
	if (pthread_create(&Interface, NULL, (void*) interfaceFunc, 0) != 0){
		perror("Thread Interface not created");
		exit(EXIT_FAILURE);
    }
	
	
	if (pthread_create(&Model, NULL, (void*) modelFunc, 0) != 0){
		perror("Thread Model not created");
		exit(EXIT_FAILURE);
    }
	
	if (pthread_create(&Viewer, NULL, (void*) viewerFunc, &input) != 0){
		perror("Thread Viewer not created");
		exit(EXIT_FAILURE);
    }
	
	if (pthread_create(&Controller, NULL, (void*) controllerFunc, &input) != 0){
		perror("Thread Controller not created");
		exit(EXIT_FAILURE);
    }
	
	if (pthread_join(Interface, NULL) != 0){
		perror("Error while joining interface");
		exit(EXIT_FAILURE);
	}
	
	if (pthread_join(RemoteInterface, NULL) != 0){
		perror("Error while joining remote remote interface");
		exit(EXIT_FAILURE);
	}
	
	if (pthread_join(Model, NULL) != 0){
		perror("Error while joining model");
		exit(EXIT_FAILURE);
	}
	
	if (pthread_join(Viewer, NULL) != 0){
		perror("Error while joining viewer");
		exit(EXIT_FAILURE);
	}
	
	if (pthread_join(Controller, NULL) != 0){
		perror("Error while joining controller");
		exit(EXIT_FAILURE);
	}
	
	pthread_mutex_destroy(&devInp);
	pthread_mutex_destroy(&devPos);
	remoteDeviceInput=destroy(remoteDeviceInput);
	deviceInput=destroy(deviceInput);
	devicePosition=destroy(devicePosition);
	exit(EXIT_SUCCESS);
}
	