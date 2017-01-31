//
// File: Config.cc
// Created by: User <Email>
// Created on: Mon May 22 16:07:56 2006
//

#include "Config.h"
#include "Rule.h"
#include "RuleFactory.h"
#include <stdio.h>
#include <string.h>
#include <sys/signal.h>
#include <dirent.h>
#include <sys/types.h>


Config* Config::instance = NULL;


Config::Config(bool verbose) : rules(NULL)
{
	rules = new RuleSet();
	Rule* newrule;
	char filename[128];
	
	strcpy(rules_dir, ".");

	DIR* dir = opendir("./rules");
	struct dirent* dp;
		
	if (dir != NULL)
		while ((dp=readdir(dir)) != NULL) {
			if (strcmp(dp->d_name, ".") == 0   || strcmp(dp->d_name, "..") == 0)
				continue;
			
			sprintf(filename, "./rules/%s", dp->d_name);

			if ((newrule = RuleFactory::loadRule(filename, verbose)) != NULL)
				rules->addRule(newrule);
			
		}
	
	closedir(dir);
}



Config::~Config()
{}
	


bool Config::initialize(bool verbose)
{
	if (instance)
		delete instance;

	instance = new Config(verbose);

	return (getRuleSet() != NULL);
}



bool Config::close()
{
	if (instance) {
		delete instance;
		instance = NULL;
		
		return true;
	}
	
	return false;
}



bool Config::display()
{
	if (!instance)
		return false;

	instance->rules->display();
	
	return true;
}



bool Config::reload()
{
	close();
	return initialize();	
}



RuleSet* Config::getRuleSet()
{
		return (getInstance()  ?  getInstance()->rules  :  NULL);
}


void reloadConfig(int sigtype, int verbose)
{
	if (verbose)
		printf("Reloading config...\n");
	Config::reload();
}
