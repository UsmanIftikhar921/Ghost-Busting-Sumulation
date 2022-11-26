#include "defs.h"

float randInt(int lower, int upper) {
	srand(time(0));
	
	int val = (rand() % (upper - lower + 1)) + lower;
	
	return val;
}

float randFloat(float lower, float upper) {
	float r = sin(rand()*rand());
	
	float val = lower + (upper - lower) * fabs(r);
	
	return val;
}