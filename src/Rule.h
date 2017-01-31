//
// File: Rule.h
// Created by: LMY
// Created on: Tue May  9 22:09:54 2006
//

#ifndef _RULE_H_
#define _RULE_H_

#include "Packet.h"
#include <stdio.h>


class Rule
{
	public:
		Rule();
		virtual ~Rule();
	
		virtual int apply(Packet* p) = 0;
		virtual void display(FILE* f = stdout) const = 0;
	
		// Costanti, tornate da apply per decidere se scartare il pacchetto, accettarlo o indicare uno stato d'errore
		const static int ERROR = -1;
		const static int DISCARD = 0;
		const static int ACCEPT = 1;
		const static int MODIFIED = 2;
};


#endif
