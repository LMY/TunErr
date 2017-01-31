//
// File: MatcherALL.h
// Created by: User <Email>
// Created on: Wed Dec 13 16:28:24 2006
//

#ifndef _MATCHERALL_H_
#define _MATCHERALL_H_

#include "Matcher.h"
#include <stdio.h>


class MatcherALL : public Matcher
{
	public:
		MatcherALL();
		 ~MatcherALL();
	
		virtual bool matches(Packet* p) const;
		virtual void display(FILE* f) const;
	
	protected:
};


#endif	//_MATCHERALL_H_
