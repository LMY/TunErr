//
// File: MatcherIP.h
// Created by: User <Email>
// Created on: Wed May 10 15:12:46 2006
//

#ifndef _MATCHERIP_H_
#define _MATCHERIP_H_

#include "Matcher.h"


class MatcherIP : public Matcher
{
	public:
		MatcherIP();
		virtual ~MatcherIP();

		int getLength() const		{ return length; }
		int getMLength() const		{ return mLength; }
		int getTtl() const				{ return ttl; }
		int getMTtl() const				{ return mTtl; }
		long getSip() const			{ return sip; }
		int getMSip() const			{ return mSip; }
		long getDip() const			{ return dip; }
		int getMDip() const			{ return mDip; }
		
		void setLength(int x)			{ length = x; }
		void setMLength(int x)		{ mLength = x; }
		void setTtl(int x)				{ ttl = x; }
		void setMTtll(int x)				{ mTtl = x; }
		void setSip(long x)			{ sip = x; }
		void setMSip(int x)			{ mSip = x; }
		void setDip(long x)			{ dip = x; }
		void setMDip(int x)			{ mDip = x; }

		virtual bool matches(Packet* p) const;
		virtual void display(FILE* f) const;

		
	protected:
		int length;			// packet size
		int mLength;		// ...e suo match type (Matcher::EQUAL, LESS, MORE, ...)
	
		int ttl;					// ttl
		int mTtl;
	
		long sip;				// source ip
		int mSip;
	
		long dip;				// destination ip
		int mDip;
};


#endif	//_MATCHERIP_H_
