//
// File: PacketTransport.h
// Created by: User <Email>
// Created on: Thu May 11 15:07:12 2006
//

#ifndef _PACKETTRANSPORT_H_
#define _PACKETTRANSPORT_H_


class PacketTransport
{
	public:
		PacketTransport();
		virtual ~PacketTransport();

		virtual unsigned int getSPort() const = 0;
		virtual unsigned int getDPort() const = 0;
		virtual unsigned int getProtocol() const = 0;
	
		// le costanti per i diversi tipi di protocollo sono IPPROTO_TCP, IPPROTO_UDP, IPPROTO_ICMP
};


#endif	//_PACKETTRANSPORT_H_
