//
// File: TimeTunnel.cc
// Created by: User <Email>
// Created on: Wed Nov 15 17:03:03 2006
//

#include "TimeTunnel.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>



TimeTunnel::TimeTunnel(Matcher* mat) : RuleNetwork(mat),
Blimit(0)
{
	gettimeofday(&time0, NULL);
}

	
TimeTunnel::TimeTunnel(Matcher* mat, double B) : RuleNetwork(mat),
Blimit(B)
{
	gettimeofday(&time0, NULL);
}
	

TimeTunnel::~TimeTunnel()
{}


int TimeTunnel::do_apply(Packet* p)
{
	if (Blimit == 0)
		return Rule::ACCEPT;
	
	struct timeval time1;
	gettimeofday(&time1, NULL);
	
	int deltausec = calcldelta(&time0, &time1);										// delta [usec]
	int waittime = (int) (1000000*p->length()/Blimit - deltausec);			// wait [usec]
	
//	printf("delta: %i\twait: %i\tsize:%i\n", deltausec, waittime, p->length());
	
	if (waittime > 0)
		delay(waittime);			// wait [msec]
//		return Rule::DISCARD;
//	#warning fai if (matcher è udp)
	
	gettimeofday(&time0, NULL);
	return Rule::ACCEPT;
}



void TimeTunnel::do_display(FILE* f) const
{
	fprintf(f, "policy: bandlimit %g;\n", Blimit);
}



int TimeTunnel::calcldelta(struct timeval* t0, struct timeval* t1)
{
	int deltausec = (t1->tv_usec - t0->tv_usec);
	int deltasec = (t1->tv_sec - t0->tv_sec);
	
	if (deltausec < 0)
		deltausec += 1000000;

	return deltausec + 1000000*deltasec;
}



void TimeTunnel::delay(int usec)
{
	struct timeval first;
	struct timeval last;
	double deltausec;


	gettimeofday(&first, NULL);
	
	do {
		gettimeofday(&last, NULL);
		
		deltausec = calcldelta(&first, &last);
	}
	while (deltausec < usec);
}
