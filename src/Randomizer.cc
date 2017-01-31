//
// File: Randomizer.cc
// Created by: LMY
// Created on: Tue Jun  6 17:41:28 2006
//

#include "Randomizer.h"
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <limits.h>
#include <math.h>

	
bool Randomizer::initialize()
{
	srand((unsigned) time(0));
	return true;
}


// Static functions
double Randomizer::random()
{
	return (double) rand() / RAND_MAX;
}


double Randomizer::randomn(double mean, double var)
{
	double x1, x2, w;
	
	do {
		x1 = 2.0 * random() - 1.0;
		x2 = 2.0 * random() - 1.0;
		
		w = x1 * x1 + x2 * x2;
	} while (w >= 1);
	
	w = sqrt((-2.0 * log(w)) / w);
	
	return (x1*w);
}


int Randomizer::random_range(int highest_number)
{
	return random_range(0, highest_number);
}


int Randomizer::random_range(int lowest_number, int highest_number)
{
    int range = highest_number - lowest_number;	// +1
	
	return lowest_number + (int) (range * ((double) rand()/(RAND_MAX + 1.0)));
}
