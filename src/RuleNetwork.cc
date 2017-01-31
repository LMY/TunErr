//
// File: RuleNetwork.cc
// Created by: User <Email>
// Created on: Tue May  9 23:19:04 2006
//

#include "RuleNetwork.h"
#include <stdio.h>

RuleNetwork::RuleNetwork(Matcher* mat) : Rule(), matcher(mat)
{}


RuleNetwork::~RuleNetwork()
{
	if (matcher)
		delete matcher;
}

int RuleNetwork::apply(Packet* p)
{
	if (matcher->matches(p))
		return do_apply(p);
	else
		return Rule::ACCEPT;
}


void RuleNetwork::display(FILE* f) const
{
	if (matcher)
		matcher->display(f);
	
	do_display(f);
}
