//
// File: Matcher.h
// Created by: User <Email>
// Created on: Tue May  9 23:13:31 2006
//

#ifndef _MATCHER_H_
#define _MATCHER_H_

#include "Packet.h"
#include <stdio.h>

class Matcher
{
	public:
		Matcher();
		virtual ~Matcher();
	
		virtual bool matches(Packet* p) const = 0;
		virtual void display(FILE* F = stdout) const = 0;
	
		// costanti per i tipi di match
		const static int NOT_SPECIFIED = 0;	// not active
	
		const static int EQUAL = 1;
		const static int MORE = 2;
		const static int MORE_EQUAL = 3;
		const static int LESS = 4;
		const static int LESS_EQUAL = 5;
		const static int NOT_EQUAL = 6;
	
		// costanti per i match binari (dei flag)
		const static int ACTIVE = 1;
		const static int INACTIVE = 2;
	
		static bool calculate_match(int value, int rule, int match_type);
};


#endif	//_MATCHER_H_
