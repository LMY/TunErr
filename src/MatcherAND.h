//
// File: MatcherAND.h
// Created by: User <Email>
// Created on: Thu May 11 16:16:45 2006
//

#ifndef _MATCHERAND_H_
#define _MATCHERAND_H_

#include "Matcher.h"


class MatcherAND : public Matcher
{
	public:
		MatcherAND(Matcher* mat1, Matcher* mat2);
		virtual ~MatcherAND();
	
		Matcher* getMatcher1() const	{ return m1; }
		Matcher* getMatcher2() const	{ return m2; }

		void setMatcher1(Matcher* m)	{ m1 = m; }
		void setMatcher2(Matcher* m)	{ m2 = m; }


		virtual bool matches(Packet* p) const;
		virtual void display(FILE* f) const;


	protected:
		Matcher* m1;
		Matcher* m2;
};


#endif	//_MATCHERAND_H_
