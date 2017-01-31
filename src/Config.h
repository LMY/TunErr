//
// File: Config.h
// Created by: User <Email>
// Created on: Mon May 22 16:07:56 2006
//

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "RuleSet.h"


class Config
{
	// Singleton
	protected:
		Config(bool verbose);
		 ~Config();
	private:
		static Config* instance;
	


	public:
		static bool initialize()				{ return initialize(true); }
		static bool initialize(bool verbose);
		static bool close();
		static bool display();
		static Config* getInstance()		{ return instance; }
	
		static bool reload();
	
		static RuleSet* getRuleSet();

	
	protected:
		RuleSet* rules;
		char rules_dir[128];
};

void reloadConfig(int sigtype, int verbose);


#endif	//_CONFIG_H_
