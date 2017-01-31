//
// File: RuleSet.cc
// Created by: User <Email>
// Created on: Tue May  9 22:41:29 2006
//

#include "RuleSet.h"


RuleSet::RuleSet() : Rule(), rules()
{}


RuleSet::~RuleSet()
{}


int RuleSet::apply(Packet* p)
{
	int ret = Rule::ACCEPT;
	int newret;
	
	for (list<Rule*>::const_iterator iter = rules.begin(); iter != rules.end(); iter++) {
		newret = (*iter)->apply(p);
		
		if (newret == Rule::ERROR  || newret == Rule::DISCARD)		// on error, error. on discard, discard. :P
				return newret;
		else {
			if (ret == Rule::MODIFIED  || newret == Rule::MODIFIED)	// if one of the rules modifies the packet, m.odify
				ret = Rule::MODIFIED;
			else																					// otherwise, just accept.
				ret = Rule::ACCEPT;
		}			
	}
		
	return ret;
}



void RuleSet::display(FILE* f) const
{
	int i=0;
	char filename[128];
	
		
	for (list<Rule*>::const_iterator iter = rules.begin(); iter != rules.end(); iter++) {
		sprintf(filename, "./rules/rule%d", i);
		printf("%s\n", filename);
		
		(*iter)->display(f);
		printf("\n");
		i++;
	}	
}
