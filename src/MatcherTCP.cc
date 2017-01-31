//
// File: MatcherTCP.cc
// Created by: User <Email>
// Created on: Thu May 11 16:07:52 2006
//

#include "MatcherTCP.h"
#include "RuleFactory.h"
#include <stdio.h>


MatcherTCP::MatcherTCP() : Matcher(),
sport(0), mSport(NOT_SPECIFIED), dport(0), mDport(NOT_SPECIFIED)
{}


MatcherTCP::~MatcherTCP()
{}


bool MatcherTCP::matches(Packet* p) const
{
	
	if (!p)
		return false;
	
	PacketTCP* tcp = p->getTCPHeader();
	if (!tcp)
		return false;

	
	if (mSport != NOT_SPECIFIED  &&  !calculate_match(tcp->getSPort(), sport, mSport))
		return false;

	if (mDport != NOT_SPECIFIED  &&  !calculate_match(tcp->getDPort(), dport, mDport))
		return false;


	return true;
}



void MatcherTCP::display(FILE* f) const
{
	fprintf(f, "tcp: ");
	
	char sign[10];
	
	if (mSport != NOT_SPECIFIED) {
		RuleFactory::encodeSymbol(mSport, sign);
		fprintf(f, "sport %s %i", sign, sport);
		
		if (mDport != NOT_SPECIFIED)
			fprintf(f, ", ");
	}
	
	if (mDport != NOT_SPECIFIED) {
		RuleFactory::encodeSymbol(mDport, sign);
		fprintf(f, "dport %s %i", sign, dport);
	}
	
	fprintf(f, ";\n");
}
