//
// File: Matcher.cc
// Created by: User <Email>
// Created on: Tue May  9 23:13:31 2006
//

#include "Matcher.h"
#include <stdio.h>

Matcher::Matcher()
{}


Matcher::~Matcher()
{}

	
bool Matcher::calculate_match(int value, int rule, int match_type)
{
	switch (match_type) {
		case EQUAL:				return (value == rule);
		case MORE:				return (value > rule);
		case MORE_EQUAL:	return (value >= rule);
		case LESS:					return (value < rule);
		case LESS_EQUAL:	return (value <= rule);
		case NOT_EQUAL:		return (value != rule);
			
		case NOT_SPECIFIED:	return true;
		
		default:						return false;
	}
}
