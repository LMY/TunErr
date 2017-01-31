//
// File: TunErr.cc
// Created by: User <Email>
// Created on: Wed May 10 16:10:39 2006
//

#define BUFSIZE 2048 

#include <linux/netfilter.h>
#include "../include/libipq.h"


#include "TunErr.h"
#include "Config.h"
#include "Randomizer.h"


TunErr::TunErr()
: handler(NULL)
{}


TunErr::~TunErr()
{
	Config::close();
}


int TunErr::init()
{
	if (!Config::initialize()) {
		printf("Error loading conf file. exiting.\n");
		return 0;
	}

	Randomizer::initialize();

	return 1;
}


int TunErr::main()
{
        int status;
		int verdict;
        unsigned char buf[BUFSIZE];
		Packet* packet = NULL;


// Here we need a ipq_handle in order to use later when we want to do things, such as read packets and do any other libipq operations.
// Note we also have a buf[BUFSIZE] above this, which is a static sized buffer for packet data. We create the handle, and move or die trying.	
        handler = ipq_create_handle(0, PF_INET);
        if (!handler)
                die();
                

// Now we determine the mode that we want to use to get packets. The different modes are IPQ_COPY_META and IPQ_COPY_PACKET. Basically we need to know if we want
// payloads as well as meta-data. This also initiates the transfer of packets from the QUEUE to user-space. If we specify IPQ_COPY_PACKET, then the 3rd argument is the
// size of the payload we are going to want. If this fails we die.

        status = ipq_set_mode(handler, IPQ_COPY_PACKET, BUFSIZE);
        if (status < 0)
                die();
		
		
                
        while (1) {
			
// Next we read the contents of a packet, with ipq_read. This says to read into buf, for a maximum size of BUFSIZE (we don't want to overflow the buffer right...).
// The last arg, in this case 0, is the timeout for the read operation. This is the same as a select() function timeout.

                status = ipq_read(handler, buf, BUFSIZE, 0);
                if (status < 0) {
						//printf("ipqerrno: %d\n", errno);
					
                        //die();
						ipq_perror("passer");
				}
                        
                switch (ipq_message_type(buf)) {
                        case NLMSG_ERROR:
                                fprintf(stderr, "Received error message %d\n", ipq_get_msgerr(buf));
                                break;

// Based on the message type, we can determine an error (NLMSG_ERROR) or if we got a packet (IPQM_PACKET). Then we use the packet_msg_t to access the packet,
// and then set the verdict (ACCEPT or DROP). ACCEPT allows the packet through the netfilter hooks, and into the appropriate service. DROP makes the packet disappear.
// Also in the IPQM_PACKET block we have some additional code which is not in the man page. This is just an example of how to access parts of the packets, such as the
// IP layer header and the TCP header. To point the headers at the right places you basically just need the size of each previous header. Then by offsetting from the
// m->payload pointer we can access the different portions of the packet. Once we have this, you can get any of the data contained in the header, such as the port in this example.

                        case IPQM_PACKET:
                                packet = new Packet(ipq_get_packet(buf));

								if (getRuleset())
									verdict = getRuleset()->apply(packet);
								else
									verdict = Rule::ACCEPT;

//								packet->debugIPQ();
//								packet->debugIP();


								switch (verdict) {
									case Rule::ACCEPT:
										status = ipq_set_verdict(handler, packet->getIPQid(), NF_ACCEPT, 0, NULL);
										break;

									case Rule::MODIFIED:
										status = ipq_set_verdict(handler, packet->getIPQid(), NF_ACCEPT, packet->length(), packet->getBuffer());
										break;									

									case Rule::DISCARD:
									case Rule::ERROR:
									default:
										status = ipq_set_verdict(handler, packet->getIPQid(), NF_DROP, 0, NULL);
										break;
								}


								if (status < 0)
									die();
								
								delete packet;
								packet = NULL;
                                break;
                        
                        default:
                                fprintf(stderr, "Unknown message type!\n");
                                break;
                }
        }

  //  After we are done, we cleanup and exit. Need to remove the hook to cleanly end all of the ipq stuff.
        ipq_destroy_handle(handler);
        return 0;
}


RuleSet* TunErr::getRuleset() const
{
	return Config::getRuleSet();	
}

void TunErr::die()
{
        ipq_perror("passer");
        ipq_destroy_handle(handler);
        exit(1);
}
