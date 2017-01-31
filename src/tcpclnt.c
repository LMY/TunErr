#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>


// Dimensione dei buffer
#define ADDRESSBUFFER 128
#define BUFSIZE 64000

// Paramentri di default
#define DEFAULT_ADDRESS "localhost"
#define DEFAULT_PORT 17777
#define DEFAULT_PATTERN 0xFFAA00AA
#define DEFAULT_PACKETSIZE 1000
#define DEFAULT_SPEED 1000



// Porta e indirizzo a cui connettersi
int port = DEFAULT_PORT;
char address[ADDRESSBUFFER] = DEFAULT_ADDRESS;
int verbose = 0;

// Opzioni riguardanti i pacchetti
int pattern = DEFAULT_PATTERN;				// pattern da trasmettere
int packet_size = DEFAULT_PACKETSIZE;	// dimensione dei pacchetti (in byte)     (deve ovviamente essere minore di BUFSIZE)
int speed = DEFAULT_SPEED;					// velocità massima (in byte per secondo)

// Status
int sent_packets = 0;
int recv_packets = 0;
int error_sending = 0;
int error_receiving = 0;
int error_detected = 0;

// Thread
struct writer_arg {
	int sock;
	struct sockaddr_in* addr;
};

int sock;
pthread_t writer_thread;
pthread_t displayer_thread;
void writer_main(struct writer_arg* arg);
void displayer_main();


void construct_packet(char* buffer, int* buf_len);
int process_command_line(int argc, char** argv);
void print_help();
int analize(char* packet, int size);
void cleanup(int sigtype);


int main(int argc, char *argv[])
{
	int nread;
	struct sockaddr_in addr;
	char buffer[BUFSIZE];

	// process command line
	if (!process_command_line(argc, argv))
		return -2;
 
	printf("Connecting to: (%s:%i)\n", address, port);
	printf("Speed = %i, PacketSize = %i, Pattern = %u\n", speed, packet_size, pattern);
	if (verbose)
		printf("Being verbose.\n");

	signal(SIGINT, cleanup);
	signal(SIGTERM, cleanup);
	signal(SIGQUIT, cleanup);
	signal(SIGHUP, SIG_IGN);
	
    // create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket creation error");
		return -1;
	} 

   // initialize address
    memset((void *) &addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    // build address using inet_pton
	if ((inet_pton(AF_INET, address, &addr.sin_addr)) < 0) {
		perror("Address creation error");
		return -1;
	}

	struct writer_arg	wa;
	wa.sock = sock;
	wa.addr = &addr;

	if (connect(sock, &addr, sizeof(addr)) < 0) {
		perror("connect error");
		return -1;
	}
	
	
	if (pthread_create(&writer_thread, NULL, writer_main, &wa) != 0) {
		perror("Error creating writer_thread");
		return -1;
	}
		
	if (pthread_create(&displayer_thread, NULL, displayer_main, &wa) != 0) {
		perror("Error creating displayer_thread");
		return -1;
	}
		

	while (1) {
		nread = recv(sock, buffer, BUFSIZE, 0);
		
		if (nread < 0) {
			perror("Read error");
			error_receiving++;
		}
		else if (nread == 0)
			break;
		else {
			recv_packets++;
			error_detected += analize(buffer, nread);
		}
	}

	close(sock);
	sock = 0;
	return 0;
}


void construct_packet(char* buffer, int* buf_len)
{
	int i;
	int* intbuf = (int*) buffer;
	
	memset(buffer, 0, BUFSIZE);
	
	for (i=0; i<packet_size/sizeof(int); i++)
		intbuf[i] = pattern;
	
	*buf_len = packet_size;
}


 
int process_command_line(int argc, char** argv)
{
	int i;
	
	for (i=1; i<argc; i++) {
		if (strcmp(argv[i], "-p") == 0)  {				// change port
			if (++i >= argc || ((port=atoi(argv[i])) <= 0))
				goto command_line_error;
		}
		else if (strcmp(argv[i], "-a") == 0)  {	// change host
			if (++i >= argc || strlen(argv[i]) >= ADDRESSBUFFER || argv[i][0] == '-')
				goto command_line_error;

			strcpy(address, argv[i]);
		}
		else if (strcmp(argv[i], "-l") == 0)  {		// change limit
			if (++i >= argc || ((speed=atoi(argv[i])) <= 0))
				goto command_line_error;
		}
		else if (strcmp(argv[i], "-s") == 0)  {	// change packet_size
			if (++i >= argc || ((packet_size=atoi(argv[i])) <= 0))
				goto command_line_error;
		}
		else if (strcmp(argv[i], "-t") == 0)  {	// change pattern
			if (++i >= argc || ((pattern=atoi(argv[i])) <= 0))
				goto command_line_error;
		}
		else if (strcmp(argv[i], "-v") == 0)		// be verbose
			verbose = 1;

		else {														// unrecognized command
command_line_error:										// just jump here to display help and terminate program
			print_help(argv[0]);
			return 0;
		}
	}

	return 1;
}



void print_help(char* argv0)
{
	printf("Usage:\n $ %s  [-a ADDRESS] [-p PORT] [-l SPEEDLIMIT] [-s PACKET_SIZE] [-t PATTERN] [-v]\n", argv0);
	printf("Options:\n-h\t\tDisplay this help screen\n-v\t\tBe verbose (default off)\n-p PORT\t\tSpecify port (default %i)\n-a ADDRESS\tSpecify address (default %s)\n", DEFAULT_PORT, DEFAULT_ADDRESS);
	printf("-l LIMIT\tMax number of packets that will be sent per second (default %i)\n-s PACKET_SIZE\tSpecify packet size (default %i)\n", DEFAULT_SPEED, DEFAULT_PACKETSIZE);
	printf("-t PATTERN\tSpecify pattern to be sent (default %u)\n", DEFAULT_PATTERN);
}



void writer_main(struct writer_arg* arg)
{
	int nread;
	char buffer[BUFSIZE];

	while (1) {
		construct_packet(buffer, &nread);

		nread = send(arg->sock, buffer, nread, 0);
		
		sent_packets++;

		if (nread < 0) {
			perror("Sendto error");
			error_sending++;
		}
	}
}

int calcldelta(struct timeval* t0, struct timeval* t1)
{
	int deltausec = (t1->tv_usec - t0->tv_usec);
	int deltasec = (t1->tv_sec - t0->tv_sec);
	
	if (deltausec < 0)
		deltausec += 1000000;

	return deltausec + 1000000*deltasec;
}


void displayer_main()
{
	float pack_perc;
	float err_perpack;
	struct timespec rqtp;

	int usec, absusec;
	struct timeval timeabs;
	struct timeval time0;
	struct timeval time1;

	double inband, outband, meaninband, meanoutband;
	int saved_sent = 0;
	int saved_recv = 0;


	
	rqtp.tv_sec = 0;
	rqtp.tv_nsec = (long) 500000000;
	
	gettimeofday(&timeabs, NULL);
	gettimeofday(&time0, NULL);
	
	while (1) {
		gettimeofday(&time1, NULL);
		
		usec = calcldelta(&time0, &time1);
		absusec = calcldelta(&timeabs, &time1);
		inband = (double) (recv_packets - saved_recv)*packet_size / usec * 1000;
		outband = (double) (sent_packets - saved_sent)*packet_size / usec * 1000;
		meaninband = (double) recv_packets*packet_size / absusec * 1000;
		meanoutband = (double) sent_packets*packet_size / absusec * 1000;
		saved_recv = recv_packets;
		saved_sent = sent_packets;
		
		pack_perc = (recv_packets > 0  ?  1 - (float) recv_packets / sent_packets :  0);	// packet loss
		err_perpack = (recv_packets > 0  ?  (float) error_detected / recv_packets : 0);	// errori per pacchetto ricevuto
		
		printf("packets:\t(%f) %i = %i - %i\n", pack_perc, (sent_packets - recv_packets), sent_packets, recv_packets);
		printf("Band usage:\t\t(in: %g Kb/s)\t(out: %g Kb/s)\n", inband, outband);
		printf("Mean Band usage:\t(in: %g Kb/s)\t(out: %g Kb/s)\n", meaninband, meanoutband);
		printf("errors:\t(%f) %i\n\n", err_perpack, error_detected);
		
		if (error_sending > 0 || error_receiving > 0)
			printf("I/O errors: sending %i, receiving %i\n", error_sending, error_receiving);
		
		time1.tv_sec = time0.tv_sec;
		time1.tv_usec = time0.tv_usec;
		nanosleep(&rqtp, NULL);	
	}
}


int analize(char* packet, int packet_size)
{
	int i, count = 0;
	int* intbuf = (int*) packet;
	int size = packet_size/sizeof(int);
	
	for (i=0; i<size; i++)
		if (intbuf[i] != pattern)
			count++;
	
	return count;
}


void cleanup(int sigtype)
{
	if (sock)
		close(sock);
	sock = 0;
	exit(0);
}
