//
// File: MatcherUDP.cc
// Created by: User <Email>
// Created on: Thu May 11 15:50:46 2006
//

#include "MatcherUDP.h"
#include "RuleFactory.h"
#include <stdio.h>


MatcherUDP::MatcherUDP() : Matcher(),
sport(0), mSport(NOT_SPECIFIED), dport(0), mDport(NOT_SPECIFIED), length(0), mLength(NOT_SPECIFIED), checksum(0), mChecksum(NOT_SPECIFIED)
{}


MatcherUDP::~MatcherUDP()
{}


bool MatcherUDP::matches(Packet* p) const
{
	if (!p)
		return false;
	
	PacketUDP* udp = p->getUDPHeader();
	if (!udp)
		return false;
	

	if (!calculate_match(udp->getSPort(), sport, mSport))
		return false;

	if (!calculate_match(udp->getDPort(), dport, mDport))
		return false;

	if (!calculate_match(udp->getLength(), length, mLength))
		return false;

	if (!calculate_match(udp->getChecksum(), checksum, mChecksum))
		return false;

	return true;
}


void MatcherUDP::display(FILE* f) const
{
	fprintf(f, "udp: ");
	
	char sign[10];
	
	if (mSport != NOT_SPECIFIED) {
		RuleFactory::encodeSymbol(mSport, sign);
		fprintf(f, "sport %s %i", sign, sport);
		
		if (mDport != NOT_SPECIFIED || mLength != NOT_SPECIFIED || mChecksum != NOT_SPECIFIED)
			fprintf(f, ", ");
	}
	
	if (mDport != NOT_SPECIFIED) {
		RuleFactory::encodeSymbol(mDport, sign);
		fprintf(f, "dport %s %i", sign, dport);

		if (mLength != NOT_SPECIFIED || mChecksum != NOT_SPECIFIED)
			fprintf(f, ", ");
	}

	if (mLength != NOT_SPECIFIED) {
		RuleFactory::encodeSymbol(mLength, sign);
		fprintf(f, "length %s %i", sign, length);

		if (mChecksum != NOT_SPECIFIED)
			fprintf(f, ", ");
	}

	if (mChecksum != NOT_SPECIFIED) {
		RuleFactory::encodeSymbol(mChecksum, sign);
		fprintf(f, "checksum %s %i", sign, checksum);
	}

	
	fprintf(f, ";\n");
}
