//
// File: RuleError.h
// Created by: LMY
// Created on: Mon Jun 12 17:35:24 2006
//

#ifndef _RULEERROR_H_
#define _RULEERROR_H_

#include "RuleNetwork.h"


class RuleError : public RuleNetwork
{
	public:
		RuleError(Matcher* mat);
		 ~RuleError();

		virtual int do_apply(Packet* p);
		virtual void do_display(FILE* f) const;
		
	
		void setType(int t)				{ type = t; }
		int getType() const			{ return type; }
		
		void insert_interveal(int nerror, double prob);
	
	
		const static int ERROR_DATA = 0;
		const static int ERROR_TRANSPORT = 1;
		const static int ERROR_ALL = 2;

	protected:
		double* probs;
		int probs_length;
	
		int type;					// one of ERROR_*
	
		void insert_errors(Packet* p, int n);
};


#endif	//_RULEERROR_H_
