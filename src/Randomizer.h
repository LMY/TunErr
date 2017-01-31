//
// File: Randomizer.h
// Created by: LMY
// Created on: Tue Jun  6 17:41:28 2006
//

#ifndef _RANDOMIZER_H_
#define _RANDOMIZER_H_


class Randomizer
{
	public:
		static bool initialize();

		static double random();
		static double randomn(double mean, double var);
	
		static int random_range(int highest_number);
		static int random_range(int lowest_number, int highest_number);
};


#endif	//_RANDOMIZER_H_
