//
// File: MatcherIP.cc
// Created by: User <Email>
// Created on: Wed May 10 15:12:46 2006
//

#include "MatcherIP.h"
#include "RuleFactory.h"
#include <stdio.h>



MatcherIP::MatcherIP() : Matcher(),
length(0), mLength(NOT_SPECIFIED), ttl(0), mTtl(NOT_SPECIFIED), sip(0), mSip(NOT_SPECIFIED), dip(0), mDip(NOT_SPECIFIED)
{}


MatcherIP::~MatcherIP()
{}


bool MatcherIP::matches(Packet* p) const
{
	if (!p)
		return false;

	
	if (mLength != NOT_SPECIFIED  &&  !calculate_match(p->getIPlength(), length, mLength))
		return false;

	if (mTtl != NOT_SPECIFIED  &&  !calculate_match(p->getIPttl(), ttl, mTtl))
		return false;

	if (mSip != NOT_SPECIFIED  &&  !calculate_match(p->getIPsip(), sip, mSip))
		return false;

	if (mDip != NOT_SPECIFIED  &&  !calculate_match(p->getIPdip(), dip, mDip))
		return false;
	
	
	return true;
}



void MatcherIP::display(FILE* f) const
{
	fprintf(f, "ip: ");

	char sign[10];
	
	if (mLength != NOT_SPECIFIED) {
		RuleFactory::encodeSymbol(mLength, sign);
		fprintf(f, "length %s %i", sign, length);
		
		if (mTtl != NOT_SPECIFIED || mSip != NOT_SPECIFIED || mDip != NOT_SPECIFIED)
			fprintf(f, ", ");
	}
	
	if (mTtl != NOT_SPECIFIED) {
		RuleFactory::encodeSymbol(mTtl, sign);
		fprintf(f, "ttl %s %i", sign, ttl);

		if (mSip != NOT_SPECIFIED || mDip != NOT_SPECIFIED)
			fprintf(f, ", ");
	}

	if (mSip != NOT_SPECIFIED) {
		RuleFactory::encodeSymbol(mSip, sign);
		fprintf(f, "sip %s %i", sign, sip);

		if (mDip != NOT_SPECIFIED)
			fprintf(f, ", ");
	}

	if (mDip != NOT_SPECIFIED) {
		RuleFactory::encodeSymbol(mDip, sign);
		fprintf(f, "dip %s %i", sign, dip);
	}

	fprintf(f, ";\n");
}
