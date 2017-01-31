//
// File: TunErr.h
// Created by: User <Email>
// Created on: Wed May 10 16:10:39 2006
//

#ifndef _TUNERR_H_
#define _TUNERR_H_

#include "RuleSet.h"


class TunErr
{
	public:
		TunErr();
		 ~TunErr();
	
		int init();
		int main();

	
	protected:
		RuleSet* getRuleset() const;
		struct ipq_handle* handler;
	
		void die();
};


#endif	//_TUNERR_H_
