//
// File: RuleDebug.cc
// Created by: User <Email>
// Created on: Mon May 22 15:47:08 2006
//

#include "RuleDebug.h"
#include <stdio.h>


RuleDebug::RuleDebug(Matcher* mat) : RuleNetwork(mat)
{}


RuleDebug::~RuleDebug()
{}



int RuleDebug::do_apply(Packet* p)
{
	printf("Rule matches!\n");
	return Rule::ACCEPT;
}


void RuleDebug::do_display(FILE* f) const
{
	fprintf(f, "policy: debug;\n");
}
