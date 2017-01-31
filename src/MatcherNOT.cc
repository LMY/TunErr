//
// File: MatcherNOT.cc
// Created by: User <Email>
// Created on: Thu May 11 16:27:32 2006
//

#include "MatcherNOT.h"
#include <stdio.h>


MatcherNOT::MatcherNOT(Matcher* mat) : Matcher(), m(mat)
{}


MatcherNOT::~MatcherNOT()
{
	if (m)
		delete m;
}



bool MatcherNOT::matches(Packet* p) const
{
	return (m  ?  !m->matches(p)  :  false);
}


void MatcherNOT::display(FILE* f) const
{
	fprintf(f, "NOT ");
	
	if (m)
		m->display(f);
}
