//
// File: MatcherUDP.h
// Created by: User <Email>
// Created on: Thu May 11 15:50:46 2006
//

#ifndef _MATCHERUDP_H_
#define _MATCHERUDP_H_

#include "Matcher.h"


class MatcherUDP : public Matcher
{
	public:
		MatcherUDP();
		virtual ~MatcherUDP();
	
		
		int getSPort() const				{ return sport; }
		int getMSPort() const			{ return mSport; }
		int getDPort() const				{ return dport; }
		int getMDPort() const			{ return mDport; }
		int getLength() const			{ return length; }
		int getMLength() const			{ return mLength; }
		int getChecksum() const		{ return checksum; }
		int getMChecksum() const	{ return mChecksum; }

		void setSPort(int x)				{ sport = x; }
		void setMSPort(int x)				{ mSport = x; }
		void setDPort(int x)				{ dport = x; }
		void setMDPort(int x)			{ mDport = x; }
		void setLength(int x)				{ length = x; }
		void setMLength(int x)			{ mLength = x; }
		void setChecksum(int x)		{ checksum = x; }
		void setMChecksum(int x)		{ mChecksum = x; }
	
		virtual bool matches(Packet* p) const;
		virtual void display(FILE* f) const;

	
	protected:
		int sport;
		int mSport;
	
		int dport;
		int mDport;
	
		int length;
		int mLength;
	
		int checksum;
		int mChecksum;
};


#endif	//_MATCHERUDP_H_
