//
// File: RuleDebug.h
// Created by: User <Email>
// Created on: Mon May 22 15:47:08 2006
//

#ifndef _RULENOTHING_H_
#define _RULENOTHING_H_

#include "RuleNetwork.h"

class RuleNothing : public RuleNetwork
{
	public:
		RuleNothing(Matcher* mat);
		 ~RuleNothing();

		virtual int do_apply(Packet* p);
		virtual void do_display(FILE* f) const;
};


#endif	//_RULEDEBUG_H_
