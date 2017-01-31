//
// File: RuleFactory.cc
// Created by: User <Email>
// Created on: Mon May 22 16:56:53 2006
//

#include "RuleFactory.h"

#include "MatcherALL.h"
#include "MatcherIP.h"
#include "MatcherTCP.h"
#include "MatcherUDP.h"
#include "MatcherAND.h"
#include "MatcherOR.h"
#include "MatcherNOT.h"


#include "RuleDebug.h"
#include "RuleDiscard.h"
#include "RuleError.h"
#include "RuleNothing.h"
#include "TimeTunnel.h"
 
 #include <string.h>
 #include <stdlib.h>


Rule* RuleFactory::loadRule(char* filename, bool verbose)
{
	if (verbose)
		printf("opening: %s...", filename);
	
	FILE* fp = fopen(filename, "r");
	
	if (fp == NULL)
		return NULL;
	
	Matcher* mat = createMatcher(fp);
	if (!mat) {
		printf("Cannot create matcher for rule %s.\n", filename);
		fclose(fp);
		return NULL;
	}
	
	Rule* rule = createRule(fp, mat);

	if (!rule) {
		delete mat;
		if (verbose)
			printf(" cannot load rule.\n");
	}
	else if (verbose)
		printf(" ok.\n");

	
	fclose(fp);

	return rule;
}


Matcher* RuleFactory::createMatcher(FILE* fp)
{
	char buffer[128];
	Matcher* mat = NULL;
	
	
	while (1) {
		if (fscanf(fp, "%s", buffer) == EOF)
			break;

		// Simple matchers
		if (strcasecmp(buffer, "matching:") == 0)
			continue;
		else if (strcasecmp(buffer, "udp:") == 0)
			mat = createMatcherUDP(fp);
		else if (strcasecmp(buffer, "tcp:") == 0)
			mat = createMatcherTCP(fp);
		else if (strcasecmp(buffer, "ip:") == 0)
			mat = createMatcherIP(fp);
		else if (strcasecmp(buffer, "all;") == 0)
			mat = new MatcherALL;
		
		// Matcher composite
		else if (strcasecmp(buffer, "and") == 0) {
			Matcher* mat2 = createMatcher(fp);
			
			mat = (mat2  ?  new MatcherAND(mat, mat2)  :  mat);
		}
		else if (strcasecmp(buffer, "or") == 0) {
			Matcher* mat2 = createMatcher(fp);
			
			mat = (mat2  ?  new MatcherOR(mat, mat2)  :  mat);
		}
		else if (strcasecmp(buffer, "not") == 0) {
			mat = createMatcher(fp);
			
			mat = (mat  ?  new MatcherNOT(mat)  :  mat);		
		}

		// "policy:" ? =)
		else
			break;
	}

	
	return mat;
}


Matcher* RuleFactory::createMatcherTCP(FILE* fp)
{
	char param[128];
	char sign[128];
	char value[128];
	int iValue;				// integer value
	int sValue;				// sign integer value (see Matcher.h)

	int pos;
	bool quit = 1;

	MatcherTCP* mat = new MatcherTCP();

	while (quit) {
		
		if (fscanf(fp, "%s %s %s", param, sign, value) == EOF)
			break;
		
		pos = strlen(value) - 1;
		if (value[pos] == ';') {
			quit = 0;
			value[pos] = '\0';
		}
		else if (value[pos] == ',')
			value[pos] = '\0';
		else
			printf("Warning. missing separator in rule file.\n");


		sValue = decodeSymbol(sign);
		iValue = atoi(value);
		
		if (strcasecmp(param, "sport") == 0) {
			mat->setSPort(iValue);
			mat->setMSPort(sValue);
		}
		else if (strcasecmp(param, "dport") == 0) {
			mat->setDPort(iValue);
			mat->setMDPort(sValue);
		}
		else
			break;
	}
	
	return mat;
}


Matcher* RuleFactory::createMatcherUDP(FILE* fp)
{
	char param[128];
	char sign[128];
	char value[128];
	int iValue;				// integer value
	int sValue;				// sign integer value (see Matcher.h)
	
	int pos;
	bool quit = 1;

	MatcherUDP* mat = new MatcherUDP();
	
	while (quit) {
		
		if (fscanf(fp, "%s %s %s", param, sign, value) == EOF)
			break;
		
		pos = strlen(value) - 1;
		if (value[pos] == ';') {
			quit = 0;
			value[pos] = '\0';
		}
		else if (value[pos] == ',')
			value[pos] = '\0';
		else
			printf("Warning. missing separator in rule file.\n");
		
		
		sValue = decodeSymbol(sign);
		iValue = atoi(value);
			
		
		if (strcasecmp(param, "sport") == 0) {
			mat->setSPort(iValue);
			mat->setMSPort(sValue);
		}
		else if (strcasecmp(param, "dport") == 0) {
			mat->setDPort(iValue);
			mat->setMDPort(sValue);
		}
		else if (strcasecmp(param, "chk") == 0  ||  strcasecmp(param, "checksum") == 0) {
			mat->setChecksum(iValue);
			mat->setMChecksum(sValue);
		}
		else if (strcasecmp(param, "length") == 0) {
			mat->setLength(iValue);
			mat->setMLength(sValue);
		}
		else
			break;
	}
	
	return mat;
}


	
Matcher* RuleFactory::createMatcherIP(FILE* fp)
{
	char param[128];
	char sign[128];
	char value[128];
	int iValue;				// integer value
	int sValue;				// sign integer value (see Matcher.h)

	int pos;
	bool quit = 1;

	MatcherIP* mat = new MatcherIP();

	while (quit) {
		
		if (fscanf(fp, "%s %s %s", param, sign, value) == EOF)
			break;
		
		pos = strlen(value) - 1;
		if (value[pos] == ';') {
			quit = 0;
			value[pos] = '\0';
		}
		else if (value[pos] == ',')
			value[pos] = '\0';
		else
			printf("Warning. missing separator in rule file.\n");

		
		sValue = decodeSymbol(sign);
		iValue = atoi(value);
		
		if (strcasecmp(param, "length") == 0) {
			mat->setLength(iValue);
			mat->setMLength(sValue);
		}
		else if (strcasecmp(param, "ttl") == 0) {
			mat->setTtl(iValue);
			mat->setMTtll(sValue);
		}
#warning saddr e daddr sono ancora da fare, si deve fare (string) IP -> (long) IP...
		else if (strcasecmp(param, "saddr") == 0  ||  strcasecmp(param, "sip") == 0) {
			mat->setSip(iValue);
			mat->setMSip(sValue);
		}
		else if (strcasecmp(param, "daddr") == 0  ||  strcasecmp(param, "dip") == 0) {
			mat->setDip(iValue);
			mat->setMDip(sValue);
		}
		
		else
			break;
	}
	
	return mat;

}

	
	


Rule* RuleFactory::createRule(FILE* fp, Matcher* mat)
{
	char buffer[128];
	double p;
	int n;
	
	Rule* rule = NULL;
	
	fscanf(fp, "%s", buffer);
	
	if (strncasecmp(buffer, "debug", 5) == 0)			// just check the "debug" string, assuming a ';' will follow.
		rule = new RuleDebug(mat);
	else if (strncasecmp(buffer, "nop", 3) == 0  ||  strncasecmp(buffer, "nothing", 7) == 0)
		rule = new RuleNothing(mat);		
	else if (strncasecmp(buffer, "discard", 7) == 0) {
		fscanf(fp, "%lf", &p);
		rule = new RuleDiscard(mat, p);
	}
	else if (strncasecmp(buffer, "bandlimit", 9) == 0) {
		fscanf(fp, "%lf", &p);
		rule = new TimeTunnel(mat, p);
	}	
	else if (strncasecmp(buffer, "error", 5) == 0) {
		rule = new RuleError(mat);

		while (fscanf(fp, "%s", buffer) != EOF) {	// read term by term, till the end.
			if (strcasecmp(buffer, "affect") == 0) {
				fscanf(fp, "%s", buffer);
				
				if (strncasecmp(buffer, "data", 4) == 0)
					static_cast<RuleError*>(rule)->setType(RuleError::ERROR_DATA);
				else if (strncasecmp(buffer, "transport", 9) == 0)
					static_cast<RuleError*>(rule)->setType(RuleError::ERROR_TRANSPORT);
				else if (strncasecmp(buffer, "all", 3) == 0)
					static_cast<RuleError*>(rule)->setType(RuleError::ERROR_ALL);
				else {
					printf("Unknown RuleError type: \"%s\"\n", buffer);
					delete rule;
					rule = NULL;
					break;
				}			
			}
			else if (strcasecmp(buffer, "calc") == 0) {
				
				do {
					if (fscanf(fp, "%s", buffer) == EOF)
						break;
				}
				while (buffer[strlen(buffer) - 1] != ','  &&  buffer[strlen(buffer) - 1] != ';');
				
				printf("RuleError::calc is not implemented, yet... skipping.\n");				
			}
			else if (strcasecmp(buffer, "prob") == 0) {
				fscanf(fp, "%d %s", &n, buffer);
				p = atof(buffer);
				
				static_cast<RuleError*>(rule)->insert_interveal(n, p);
			}
			else {
				printf("Unknown param in rule error: \"%s\"\n", buffer);
				delete rule;
				rule = NULL;
				break;
			}

			
			if (buffer[strlen(buffer) - 1] == ';')
				break;
		}
	}
	
	
	return rule;	
}

	
int RuleFactory::decodeSymbol(char* string)
{
	if (strcmp(string, "==") == 0)
		return Matcher::EQUAL;
	else if (strcmp(string, ">") == 0)
		return Matcher::MORE;
	else if (strcmp(string, ">=") == 0) 
		return Matcher::MORE_EQUAL;
	else if (strcmp(string, "<") == 0)
		return Matcher::LESS;
	else if (strcmp(string, "<=") == 0)
		return Matcher::LESS_EQUAL;
	else if (strcmp(string, "!=") == 0)
		return Matcher::NOT_EQUAL;
	else
		return Matcher::NOT_SPECIFIED;
}



void RuleFactory::encodeSymbol(int sign, char* string)
{
	if (sign == Matcher::EQUAL)
		strcpy(string, "==");
	else if (sign == Matcher::MORE)
		strcpy(string, ">");
	else if (sign == Matcher::MORE_EQUAL)
		strcpy(string, ">=");
	else if (sign == Matcher::LESS)
		strcpy(string, "<");
	else if (sign == Matcher::LESS_EQUAL)
		strcpy(string, "<=");
	else if (sign == Matcher::NOT_EQUAL)
		strcpy(string, "!=");
	else
		strcpy(string, "?=");
}
