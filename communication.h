#ifndef communication_h
#define communication_h

typedef struct Positions{
	int localTime;
    int remoteTime;
	int timeToPrint;
	int lastLocalTime;
	int lastRemoteTime;
	double localChange;
	double remoteChange;
    double localPosition;
    double remotePosition;
	double positionToPrint;
}Positions;

void viewerComm(Output *out);
void controllerComm(Output *out);
void modelComm(Positions *p);

#endif