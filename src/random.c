#include "random.h"
#include <stdlib.h>

void rngInit()
{
	srand(RANDOM_SEED);
}

int rngRange(int min, int max)
{
	return min + rand() / (RAND_MAX / (max - min));
}

float rngRangeF(float min, float max)
{
	float scale = rand() / (float) RAND_MAX; // [0, 1.0] 
    return min + scale * (max - min); 		 // [min, max[ 
}
