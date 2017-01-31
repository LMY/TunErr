//
// File: RuleSet.h
// Created by: User <Email>
// Created on: Tue May  9 22:41:29 2006
//

#ifndef _RULESET_H_
#define _RULESET_H_

#include "Rule.h"
#include <stdio.h>

#include <list>
using namespace std;


class RuleSet : public Rule
{
	public:
		RuleSet();
		 ~RuleSet();
	
	int apply(Packet* p);
	void display(FILE* f = stdout) const;
	
	int size()								{ return rules.size(); }
	void addRule(Rule* r)			{ rules.push_back(r); }
	void delRule(Rule* r)			{ rules.remove(r); }
	

	protected:
		list<Rule*> rules;
};


#endif	//_RULESET_H_
