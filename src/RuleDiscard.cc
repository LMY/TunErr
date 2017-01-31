//
// File: RuleDiscard.cc
// Created by: User <Email>
// Created on: Wed May 10 15:05:45 2006
//

#include "RuleDiscard.h"
#include "Randomizer.h"
#include <stdio.h>


RuleDiscard::RuleDiscard(Matcher* mat) : RuleNetwork(mat), prob(0)
{}

RuleDiscard::RuleDiscard(Matcher* mat, double p) : RuleNetwork(mat), prob(p)
{}

RuleDiscard::~RuleDiscard()
{}


int RuleDiscard::do_apply(Packet* p)
{
	if (Randomizer::random() < prob)
		return Rule::DISCARD;
	else
		return Rule::ACCEPT;
}

void RuleDiscard::do_display(FILE* f) const
{
	fprintf(f, "policy: discard %g;\n", prob);
}
