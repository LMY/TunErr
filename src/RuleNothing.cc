//
// File: RuleDebug.cc
// Created by: User <Email>
// Created on: Mon May 22 15:47:08 2006
//

#include "RuleNothing.h"
#include <stdio.h>


RuleNothing::RuleNothing(Matcher* mat) : RuleNetwork(mat)
{}


RuleNothing::~RuleNothing()
{}



int RuleNothing::do_apply(Packet* p)
{
	return Rule::ACCEPT;
}

void RuleNothing::do_display(FILE* f) const
{
	fprintf(f, "policy: nothing;\n");
}
