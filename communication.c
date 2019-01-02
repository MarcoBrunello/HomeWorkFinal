#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>



#include "linkingList.h"
#include "output.h"
#include "globalVariables.h"
#include "communication.h"


void viewerComm(Output *out){
	pthread_mutex_lock(&devPos);
	while(devicePosition==NULL){
		pthread_cond_wait(&notEmptyViewer,&devPos);
	}
	
	out->position = devicePosition->position;
	out->time = devicePosition->time;
	count1=0;
	
	if(count1 >= count2){
		devicePosition=destroy(devicePosition);
	}
	pthread_mutex_unlock(&devPos);
}

void controllerComm(Output *out){
	pthread_mutex_lock(&devPos);
	
	while(devicePosition==NULL && notDone){
		pthread_cond_wait(&notEmptyController,&devPos);
	}
	
	out->position = devicePosition->position;
	out->time = devicePosition->time;
	count2=0;
	
	if(count2 >= count1){
		devicePosition=destroy(devicePosition);
	}
	pthread_mutex_unlock(&devPos);
}


void modelComm(Positions *posit){
		pthread_mutex_lock(&devInp); //As consumer 
		while(deviceInput == NULL && remoteDeviceInput == NULL && notDone){
			pthread_cond_wait(&notEmpty, &devInp);
		}
		if(deviceInput != NULL && notDone){
		     posit->localChange=deviceInput->position;
		     posit->localTime=deviceInput->time;
		     deviceInput = deleteNode(deviceInput, posit->localTime);
		}
		 if(remoteDeviceInput != NULL && notDone){
			 posit->remoteChange=remoteDeviceInput->position;
			 posit->remoteTime=remoteDeviceInput->time;
			 remoteDeviceInput = deleteNode(remoteDeviceInput, posit->remoteTime);
		 }
		 pthread_mutex_unlock(&devInp);
		 
		 if(posit->lastLocalTime != posit->localTime){
		     posit->localPosition = posit->localPosition + posit->localChange;
			 posit->lastLocalTime = posit->localTime;
		 }
			 
		 if(posit->lastRemoteTime != posit->remoteTime){
	        posit->remotePosition = posit->remotePosition + posit->remoteChange;
			posit->lastRemoteTime =posit->remoteTime;
		 }
		
		 if(!remoteOff){
			 posit->timeToPrint = posit->lastRemoteTime;
			 posit->positionToPrint = posit->remotePosition;
		 }
		 else{
			 posit->timeToPrint = posit->lastLocalTime;
			 posit->positionToPrint = posit->localPosition;
		 }
		 
		 pthread_mutex_lock(&devPos);     //As producer
		 devicePosition = addToList(devicePosition, posit->positionToPrint, posit->timeToPrint);
		 count1=1;
		 count2=1;
		 pthread_mutex_unlock(&devPos);	
		 pthread_cond_signal(&notEmptyViewer);
		 pthread_cond_signal(&notEmptyController);
	 
}

