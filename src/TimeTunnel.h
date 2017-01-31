//
// File: TimeTunnel.h
// Created by: User <Email>
// Created on: Wed Nov 15 17:03:03 2006
//

#ifndef _TIMETUNNEL_H_
#define _TIMETUNNEL_H_

#include "RuleNetwork.h"
#include <sys/time.h>


class TimeTunnel : public RuleNetwork
{
	public:
		TimeTunnel(Matcher* mat);
		TimeTunnel(Matcher* mat, double B);
		 ~TimeTunnel();
	
		void setBlimit(double v)			{ Blimit = v; }
		double getBlimit() const			{ return Blimit; }

		virtual int do_apply(Packet* p);
		virtual void do_display(FILE* f) const;
	
		static int calcldelta(struct timeval* t0, struct timeval* t1);	// usec t0->t1


	protected:
		static void delay(int usec);
		
		double Blimit;
		
		struct timeval time0;
};


#endif	//_TIMETUNNEL_H_
