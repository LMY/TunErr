//
// File: MatcherNOT.h
// Created by: User <Email>
// Created on: Thu May 11 16:27:32 2006
//

#ifndef _MATCHERNOT_H_
#define _MATCHERNOT_H_

#include "Matcher.h"


class MatcherNOT : public Matcher
{
	public:
		MatcherNOT(Matcher* mat);
		virtual ~MatcherNOT();
	
	
		Matcher* getMatcher() const		{ return m; }

		void setMatcher(Matcher* mat)	{ m = mat; }

		
		virtual bool matches(Packet* p) const;
		virtual void display(FILE* f) const;

		
	protected:
		Matcher* m;
};


#endif	//_MATCHERNOT_H_
