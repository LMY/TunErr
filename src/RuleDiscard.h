//
// File: RuleDiscard.h
// Created by: User <Email>
// Created on: Wed May 10 15:05:45 2006
//

#ifndef _RULEDISCARD_H_
#define _RULEDISCARD_H_

#include "RuleNetwork.h"


class RuleDiscard : public RuleNetwork
{
	public:
		RuleDiscard(Matcher* mat);
		RuleDiscard(Matcher* mat, double p);
		 ~RuleDiscard();
	
	
		void setProb(double p)			{ prob = p; }
		double getProb() const		{ return prob; }
	
		virtual int do_apply(Packet* p);
		virtual void do_display(FILE* f) const;


	protected:
		double prob;
};


#endif	//_RULEDISCARD_H_
