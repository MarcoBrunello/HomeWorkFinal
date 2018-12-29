#ifndef output_h
#define output_h

typedef struct Output{
	int time;
	double position;
}Output;

typedef struct Input{
	int leftWall;
	int rightWall;
	int cycleTimeController;
	int cycleTimeViewer;
}Input;

void printOutputViewer(Output* out);
void printOutputController(FILE *file, Output* out, Input* in);
void checkPosition(Input *in, Output *out);
void parseInput( int* numArg, char** arg, Input* inp);
#endif