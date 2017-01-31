//
// File: PacketTCP.h
// Created by: User <Email>
// Created on: Thu May 11 15:21:53 2006
//

#ifndef _PACKETTCP_H_
#define _PACKETTCP_H_

#include "PacketTransport.h"
#include <netinet/tcp.h>
#include <netinet/in.h>



class PacketTCP : public PacketTransport
{
	public:
		PacketTCP(struct tcphdr* header);
		 ~PacketTCP();
	
		unsigned int getSPort() const			{ return ntohs(tcp_header->source); }
		unsigned int getDPort() const		{ return ntohs(tcp_header->dest); }
	
		unsigned int getSeq() const			{ return ntohl(tcp_header->seq); }
		unsigned int getAckSeq() const		{ return ntohl(tcp_header->ack_seq); }
		
		unsigned int getProtocol() const	{ return IPPROTO_TCP; }
		
	protected:
		struct tcphdr* tcp_header;
};


#endif	//_PACKETTCP_H_
