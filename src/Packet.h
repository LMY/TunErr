//
// File: Packet.h
// Created by: User <Email>
// Created on: Tue May  9 22:17:14 2006
//

#ifndef _PACKET_H_
#define _PACKET_H_

#include "../include/libipq.h"
#include <netinet/ip.h>

#include "PacketTransport.h"
 #include "PacketTCP.h"
#include "PacketUDP.h"


class Packet
{
	public:
		Packet(ipq_packet_msg_t* p);
		 ~Packet();
	
		// funzioni per accedere alle informazioni contenute nell'header ip
		unsigned int getIPttl() const;
		unsigned int getIPlength() const;
		unsigned long getIPsip() const;
		unsigned long getIPdip() const;
		unsigned int getIPprotocol() const;
		unsigned int getIPHeaderLength() const	{ return 4 * ip_header->ihl; }
	
		// funzioni per accedere agli header di trasporto (col cast.)
		PacketTCP* getTCPHeader() const		{ return (ip_header->protocol  == IPPROTO_TCP  ?  (PacketTCP*) proto_header  :  NULL); }
		PacketUDP* getUDPHeader() const	{ return (ip_header->protocol  == IPPROTO_UDP  ?  (PacketUDP*) proto_header  :  NULL); }
		void* getTransportHeader() const		{ return (void*) proto_header; }

		// funzione per accedere all'id ipq del pacchetto (per poter dare un verdetto)
		int getIPQid() const;

		// funzioni per accedere ai dati contenuti nel pacchetto (e modificarli)
		int length() const											{ return packet->data_len; }
		char get(int x) const										{ return getBuffer()[x]; }
		void set(int x, char value)								{ packet->payload[x] = value; }
		unsigned char* getBuffer() const				{ return packet->payload; }
		void change(int byte, int bit, int whattodo);					// per introdurre un errore di tipo arbitrario
		
		// costanti per la funzione change
		const static int OFF = 0;
		const static int ON = 1;
		const static int TOGGLE = 2;


		// funzioni di debug
		void debugIPQ() const;
		void debugIP() const;
		
	protected:
		ipq_packet_msg_t* packet;				// ipq header
		struct iphdr* ip_header;						// ip header 	(start of)
		PacketTransport* proto_header;	// transport protocol header (start of)
};


#endif	//_PACKET_H_
