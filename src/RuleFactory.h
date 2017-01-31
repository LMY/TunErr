//
// File: RuleFactory.h
// Created by: User <Email>
// Created on: Mon May 22 16:56:53 2006
//

#ifndef _RULEFACTORY_H_
#define _RULEFACTORY_H_

#include "Matcher.h"
#include "Rule.h"
#include <stdio.h>


class RuleFactory
{
	public:
		static Rule* loadRule(char* filename, bool verbose);

		static int decodeSymbol(char* string);
		static void encodeSymbol(int sign, char* string);
	
	private:
		static inline Matcher* createMatcher(FILE* fp);
		static inline Matcher* createMatcherTCP(FILE* fp);
		static inline Matcher* createMatcherUDP(FILE* fp);
		static inline Matcher* createMatcherIP(FILE* fp);
	
		static inline Rule* createRule(FILE* fp, Matcher* mat);
};


#endif	//_RULEFACTORY_H_
