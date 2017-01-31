//
// File: MatcherTCP.h
// Created by: User <Email>
// Created on: Thu May 11 16:07:52 2006
//

#ifndef _MATCHERTCP_H_
#define _MATCHERTCP_H_

#include "Matcher.h"


class MatcherTCP : public Matcher
{
	public:
		MatcherTCP();
		virtual ~MatcherTCP();

		int getSPort() const				{ return sport; }
		int getMSPort() const			{ return mSport; }
		int getDPort() const				{ return dport; }
		int getMDPort() const			{ return mDport; }

		void setSPort(int x)				{ sport = x; }
		void setMSPort(int x)				{ mSport = x; }
		void setDPort(int x)				{ dport = x; }
		void setMDPort(int x)			{ mDport = x; }

		virtual bool matches(Packet* p) const;
		virtual void display(FILE* f) const;

	
	protected:
		int sport;
		int mSport;
	
		int dport;
		int mDport;
};


#endif	//_MATCHERTCP_H_
