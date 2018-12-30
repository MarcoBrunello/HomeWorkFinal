#ifndef globalVariables_h
#define globalVariables_h

extern int count1;
extern int count2;
extern int serverSocket;
extern struct node *deviceInput;
extern struct node *devicePosition;
extern struct node *remoteDeviceInput;
extern bool notDone;
extern bool notFinish;
extern bool remoteOff;
extern pthread_mutex_t devInp;
extern pthread_mutex_t devPos;
extern pthread_cond_t notEmpty;
extern pthread_cond_t notEmptyViewer;
extern pthread_cond_t notEmptyController;

#endif