#include "RandomNumberGenerator.hpp"
#include <stdlib.h>
#include <math.h>

int RandomNumberGenerator::RollRandomIntLessThan(int maxNotInclusive)
{
    int randomNumber = rand();
    randomNumber = randomNumber % maxNotInclusive;
    return randomNumber;
}

int RandomNumberGenerator::RollRandomIntRange(int minInclusive, int maxInclusive)
{
	int randomNumber = rand();
	randomNumber = randomNumber % (maxInclusive -minInclusive+1);
    randomNumber += minInclusive;
	return randomNumber;
}

float RandomNumberGenerator::RollRandomFloatZeroToOne()
{
    float randomNumber =static_cast<float>(rand());
    randomNumber = (float)rand() / (float)RAND_MAX;

    return randomNumber;
}

float RandomNumberGenerator::RollRandomFloatInRange(float minInclusive, float maxInclusive)
{
    
    float range = maxInclusive - minInclusive;

	float randomNumber = minInclusive + (range*RollRandomFloatZeroToOne());

    return randomNumber;
}
