//
// File: RuleDebug.h
// Created by: User <Email>
// Created on: Mon May 22 15:47:08 2006
//

#ifndef _RULEDEBUG_H_
#define _RULEDEBUG_H_

#include "RuleNetwork.h"

class RuleDebug : public RuleNetwork
{
	public:
		RuleDebug(Matcher* mat);
		 ~RuleDebug();

		virtual int do_apply(Packet* p);
		virtual void do_display(FILE* f) const;
};


#endif	//_RULEDEBUG_H_
