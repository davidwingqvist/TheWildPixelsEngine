#pragma once
#include <stdio.h>
#include <stdlib.h>

// Randomizes a value between the lower and higher parameters.
float randomize(float lower, float higher)
{
	return (float)(lower + static_cast<float>(rand()) / (RAND_MAX / (higher - lower)));
}

// Multiplies parameter with a factor and returns parameter in precentage form.
float precentage(float val)
{
	return val * 0.01f;
}

