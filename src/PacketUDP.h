//
// File: PacketUDP.h
// Created by: User <Email>
// Created on: Thu May 11 15:35:04 2006
//

#ifndef _PACKETUDP_H_
#define _PACKETUDP_H_

#include "PacketTransport.h"
#include <netinet/in.h>
#include <netinet/udp.h>



class PacketUDP : public PacketTransport
{
	public:
		PacketUDP(struct udphdr* header);
		 ~PacketUDP();
	
		unsigned int getSPort() const			{ return ntohs(udp_header->source); }
		unsigned int getDPort() const		{ return ntohs(udp_header->dest); }
	
		unsigned int getLength() const		{ return ntohs(udp_header->len); }
		unsigned int getChecksum() const	{ return udp_header->check; }
		
		unsigned int getProtocol() const	{ return IPPROTO_UDP; }
	
	protected:
		struct udphdr* udp_header;
};


#endif	//_PACKETUDP_H_
