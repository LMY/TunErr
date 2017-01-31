//
// File: MatcherAND.cc
// Created by: User <Email>
// Created on: Thu May 11 16:16:45 2006
//

#include "MatcherOR.h"
#include <stdio.h>


MatcherOR::MatcherOR(Matcher* mat1, Matcher* mat2) : Matcher()
{
	m1 = mat1;
	m2 = mat2;
}


MatcherOR::~MatcherOR()
{
	if (m1)
		delete m1;
	
	if (m2)
		delete m2;
}


bool MatcherOR::matches(Packet* p) const
{
	if (m1  &&  m1->matches(p))
		return true;
	
	if (m2  &&  m2->matches(p))
		return true;
	
	return false;
}


void MatcherOR::display(FILE* f) const
{
	fprintf(f, "OR\n");
	
	if (m1)
		m1->display(f);
	
	if (m2)
		m2->display(f);
}
