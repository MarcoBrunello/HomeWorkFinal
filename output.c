#include <stdio.h>
#include <stdlib.h>

#include "output.h"


void parseInput( int* numArg, char** arg, Input* inp){
	if(*numArg != 5 || arg[1] >= arg[2]  || arg[3] < 0 || arg[4] < 0 ){
		perror("Input not valid\n");
		exit(EXIT_FAILURE);
	}
	
	inp->leftWall = atoi(arg[1]);
	inp->rightWall= atoi(arg[2]);
	inp->cycleTimeController = atoi(arg[3]);
	inp->cycleTimeViewer = atoi(arg[4]);
}

void printOutputViewer(Output* out){
	    printf("This is the viewer. Position is %lf at time %d \n\n\n\n\n\n\n", out->position, out->time);
}

void printOutputController(FILE *file, Output* out, Input* in){
	if(out->position >= in->rightWall){
		printf("\a"); // :D
	    out->position = in->rightWall;
    }
	
	if(out->position <= in->leftWall){
		printf("\a");
		out->position = in->leftWall;
	}

    fprintf(file, "Time %d  Position %lf\r\n", out->time,out->position);
    fflush(file);
}

void checkPosition(Input* in, Output *out){
	printf("O");
	if(out->position >= in->rightWall){
		out->position = in->rightWall;
	    for(int i= in->leftWall; i < in->rightWall; i++)
			printf("-");
		printf("X");
	}
	else if(out->position <= in->leftWall){
		out->position = in->leftWall;
		printf("X");
	    for(int i= in->leftWall; i < in->rightWall; i++)
			printf("-");
	} 
	else{
	for(int i= in->leftWall; i < in->rightWall; i++){
	if( (int) out->position == i)
		printf("X");
	printf("-");
	}
	}
	printf("O\n\n");
}