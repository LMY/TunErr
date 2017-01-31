
#include <stdio.h>
#include <time.h>
#include <sys/time.h>



int calcldelta(struct timeval* t0, struct timeval* t1)
{
	int deltausec = (t1->tv_usec - t0->tv_usec);
	int deltasec = (t1->tv_sec - t0->tv_sec);
	
	if (deltausec < 0)
		deltausec += 1000000;

	return deltausec + 1000000*deltasec;
}



void delay(int usec)
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



int main()
{
	int n;
	
	printf("GOGOGO!\n");
	
	for (n=10; n>0; n--) {
		printf("%d\n", n);
		
		delay(500000);
	}
	
	printf("blaaah.\n");
	
	return 0;
}
