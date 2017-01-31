//
// File: MatcherALL.cc
// Created by: User <Email>
// Created on: Wed Dec 13 16:28:24 2006
//

#include "MatcherALL.h"


MatcherALL::MatcherALL() : Matcher()
{}


MatcherALL::~MatcherALL()
{}

	
	
bool MatcherALL::matches(Packet* p) const
{
	return (p  ?  true  :  false);
}


	
void MatcherALL::display(FILE* f) const
{
	fprintf(f, "ALL;\n");
}
