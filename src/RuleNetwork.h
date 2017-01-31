//
// File: RuleNetwork.h
// Created by: User <Email>
// Created on: Tue May  9 23:19:04 2006
//

#ifndef _RULENETWORK_H_
#define _RULENETWORK_H_

#include "Rule.h"
#include "Matcher.h"

class RuleNetwork : public Rule
{
	public:
		RuleNetwork(Matcher* mat);
		~RuleNetwork();
	
	int apply(Packet* p);								// if packet matches, call do_apply (to be defined)
	void display(FILE* f = stdout) const;
	virtual int do_apply(Packet* p) = 0;
	virtual void do_display(FILE* f) const = 0;


	protected:
		Matcher* matcher;
};


#endif	//_RULENETWORK_H_
