#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>


// se verbose = 1, viene scritto (sul terminale) quando il server riceve un pacchetto (DEFAULT 0)
int verbose = 0;
// la porta su cui il server udp accetta connessioni (DEFAULT 17777)
#define DEFAULT_PORT 17777
int port = DEFAULT_PORT;

// se policy = 0, il server non invierà nulla al mittente
// se policy = 1, il server risponderà con un semplice ack
// se policy = 2, il server rimanderà i pacchetti al mittente (così come li ha ricevuti)
// se policy = 3, il server rimanderà i pacchetti al mittente rovesciati
// DEFAULT 2 (resend)
#define DEFAULT_POLICY 2
int policy = DEFAULT_POLICY;


#define BUFSIZE 64000


void construct_packet(char* buffer, int* buf_len);
int process_command_line(int argc, char** argv);
void print_help();



int main(int argc, char *argv[])
 {
    int  n;
    int sock;
	unsigned int	 len;
    struct sockaddr_in addr;
    char buffer[BUFSIZE];

	if (!process_command_line(argc, argv))
		return -2;
 
	printf("Accepting connections on port: %i\nPolicy is: %i\n", port, policy);
	if (verbose)
		printf("Being verbose.\n");

	// create socket
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Socket creation error");
		return -1;
	}

	// initialize address
	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	len = sizeof(struct sockaddr);

	// Bind
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind error");
		
		if (port < 1024)
			printf("Be sure to be root if you want to lauch the server on a reserved (< 1024) port.\n");
		
		return -1;
	}

     // Main loop
	while (1) {
		n = recvfrom(sock, buffer, BUFSIZE, 0, (struct sockaddr *)&addr, &len);
		if (n < 0) {
			perror("Recvfrom error");
			return -1;
		}
	
		if (verbose) {
			inet_ntop(AF_INET, &addr.sin_addr, buffer, sizeof(buffer));
			printf("Request from host %s, port %d\n", buffer, ntohs(addr.sin_port));
		}
	
		// construct answer
		construct_packet(buffer, &n);

		// if answer_length > 0, send answer
		if (n > 0)
			n = sendto(sock, buffer, n, 0,  (struct sockaddr *)&addr, sizeof(addr));

		if (n < 0) {
			perror("sendto error");
			return -1;
		}
	} 

	return 0;
 }

 
 
 void construct_packet(char* buffer, int* buf_len)
 {
	int i;
	char c;

 	switch (policy) {
		case 0:											// no answer
			*buf_len = 0;
			break;
		
		case 1:											// simple ack
			strcpy(buffer, "ACK");
			*buf_len = 4;
			break;

		case 2:											// resend packet
			break;

		case 3:											// invert packet
			for (i=0; i<*buf_len/2; i++) {
				c = buffer[i];
				buffer[i] = buffer[*buf_len - i];
				buffer[*buf_len - i] = c;				
			}
			break;		
	}	 
 }

 
 
int process_command_line(int argc, char** argv)
{
	int i;
	
	for (i=1; i<argc; i++) {
		if (strcmp(argv[i], "-p") == 0)  {				// change port
			if (++i >= argc || ((port=atoi(argv[i])) <= 0))
				goto command_line_error;
		}
		else if (strcmp(argv[i], "-y") == 0)   {	// change policy
			if (++i >= argc || ((policy=atoi(argv[i])) < 0))
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
	printf("Usage:\n $ %s  [-v] [-p PORT] [-y POLICY ]\n", argv0);
	printf("Options:\n-h\t\tDisplay this help screen\n-v\t\tBe verbose (default off)\n-p PORT\t\tSpecify port (default %i)\n-y POLICY\tSpecify policy (default %i)\n", DEFAULT_PORT, DEFAULT_POLICY);
	
	printf("Policies:\n0\t\tServer won't answer when receiving a packet\n1\t\tServer will answer with a simple ACK\n");
	printf("2\t\tServer will send back every received packet\n3\t\tServer will send back every received packet reversed\n");
}
