//
// File: MatcherAND.cc
// Created by: User <Email>
// Created on: Thu May 11 16:16:45 2006
//

#include "MatcherAND.h"
#include <stdio.h>


MatcherAND::MatcherAND(Matcher* mat1, Matcher* mat2) : Matcher()
{
	m1 = mat1;
	m2 = mat2;
}


MatcherAND::~MatcherAND()
{
	if (m1)
		delete m1;
	
	if (m2)
		delete m2;
}


bool MatcherAND::matches(Packet* p) const
{
	if (m1  &&  !m1->matches(p))
		return false;
	
	if (m2  &&  !m2->matches(p))
		return false;
	
	return (m1 != NULL  ||  m2 != NULL);		// at least one !=NULL. ((ie, if m1=m2=NULL, return false))
}



void MatcherAND::display(FILE* f) const
{
	fprintf(f, "AND\n");
	
	if (m1)
		m1->display(f);
	
	if (m2)
		m2->display(f);
}
