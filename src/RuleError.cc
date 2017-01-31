//
// File: RuleError.cc
// Created by: LMY
// Created on: Mon Jun 12 17:35:24 2006
//

#include "RuleError.h"
#include "Randomizer.h"
#include <stdio.h>


RuleError::RuleError(Matcher* mat) : RuleNetwork(mat), probs(NULL), probs_length(1), type(ERROR_DATA)
{
	probs = new double[1];
	probs[0] = 1;
}


RuleError::~RuleError()
{
	if (probs)
		delete [] probs;
}



void RuleError::insert_interveal(int nerror, double prob)
{
	if (nerror >= probs_length) {											// not enough space
		double* errors = new double[nerror+1];				// allocate
		
		for (int i=0; i<nerror; i++)										// copy
			errors[i] = (i < probs_length  ?  probs[i]  :  0);
		
		errors[nerror] = prob;													// insert (last element)
		
		delete [] probs;															// swap pointers
		probs = errors;
		probs_length = nerror+1;
	}
	else
		probs[nerror] = prob;
}

	

int RuleError::do_apply(Packet* p)
{
	int n = 0;
	double min = 0;
	double max = probs[0];
	double value = Randomizer::random();
	
	
	while (1) {
		if (value >= min  && value < max) {		// if this is the right interveal
			if (n > 0) {
				insert_errors(p, n);								// insert the correct number of errors in the packet (if any)
				return Rule::MODIFIED;
			}
			else
				return Rule::ACCEPT;
		}
																			// otherwise
		n++;															// goto next interveal
		min = max;													// adjourn limits
		max += probs[n];
	}

	// since value < 1, max = 1 (at the last step), this code should be unreachable
	return Rule::ACCEPT;
}



void RuleError::insert_errors(Packet* p, int n)
{
	// calculate packet start and end
	int packet_start;
	int packet_length = p->getIPlength();
	
	switch (type) {
		case ERROR_TRANSPORT:
			packet_start = p->getIPHeaderLength();
			break;

		case ERROR_DATA:
			packet_start = p->getIPHeaderLength() + 8;
			break;		

//		case ERROR_ALL:
		default:
			packet_start = 0;
			break;
	}


	// calculate where to put the errors
	int byte, bit;

	for (int i=0; i<n; i++) {
		byte = Randomizer::random_range(packet_start, packet_length);
		bit = Randomizer::random_range(8);
		
		p->change(byte, bit, Packet::TOGGLE);
	}
}



void RuleError::do_display(FILE* f) const
{
	fprintf(f, "error affect ");
	
	if (type == ERROR_DATA)
		fprintf(f, "data");
	else if (type == ERROR_TRANSPORT)
		fprintf(f, "transport");
	else if (type == ERROR_ALL)
		fprintf(f, "all");
	
	for (int n=1; n<probs_length; n++)
		fprintf(f, " prob %i %lf", n, probs[n]);
	
	fprintf(f, ";\n");
}
