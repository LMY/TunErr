//
// File: Packet.cc
// Created by: User <Email>
// Created on: Tue May  9 22:17:14 2006
//

#include <stdio.h>		/* solo per debugIPQ e debugIP */

#include "Packet.h"



Packet::Packet(ipq_packet_msg_t* p)
: packet(p)
{
	ip_header = (struct iphdr*) packet->payload;

	switch (ip_header->protocol) {
		// ihl è "numero di dword occupate dell'header ip", quindi 4byte.
		case IPPROTO_TCP:
			proto_header = new PacketTCP((struct tcphdr*) (packet->payload + (4 * ip_header->ihl)));
			break;
		
		case IPPROTO_UDP:
			proto_header = new PacketUDP((struct udphdr*) (packet->payload + (4 * ip_header->ihl)));
			break;
		
		default:
			proto_header = NULL;
	}
}


Packet::~Packet()
{
//	delete [] data;
//	data = NULL;
	if (proto_header)
		delete proto_header;
}



void Packet::change(int byte, int bit, int whattodo)
{
	unsigned char* buffer = getBuffer();
	
	if (whattodo == OFF)
		buffer[byte] &= !(1 << bit);
	else if (whattodo == ON)
		buffer[byte] |= (1 << bit);
	else
		buffer[byte] ^= (1 << bit);
}


int Packet::getIPQid() const
{
	return (packet  ?  packet->packet_id  :  0);
}


unsigned int Packet::getIPttl() const
{
	return (ip_header  ?  ip_header->ttl  :  0);
}


unsigned int Packet::getIPlength() const
{
	return (ip_header  ?  ntohs(ip_header->tot_len)  :  0);
}


unsigned long Packet::getIPsip() const
{
	return (ip_header  ?  ntohl(ip_header->daddr)  :  0);
}


unsigned long Packet::getIPdip() const
{
	return (ip_header  ?  ntohl(ip_header->saddr)  :  0);
}


unsigned int Packet::getIPprotocol() const
{
	return (ip_header  ?  ip_header->protocol  :  0);
}



// Funzioni di debug, per scrivere le informazioni degli header su schermo

void Packet::debugIPQ() const
{
	if (!packet)
		printf("NULL packet.\n");
	else {
			printf("Dumping Metadata:\nID=0x%08lX MARK=0x%08lX TSEC=%ld "
	        "TUSEC=%ld HOOK=%u IN=%s OUT=%s DLEN=%d\n",
	        packet->packet_id,
	        packet->mark,
	        packet->timestamp_sec,
	        packet->timestamp_usec,
	        packet->hook,
	        packet->indev_name[0] ? packet->indev_name : "[none]",
	        packet->outdev_name[0] ? packet->outdev_name : "[none]",
	        packet->data_len);
	}
}

void Packet::debugIP() const
{
	printf("Dumping IP Header:\n");
	printf("VER=%u HLEN=%u TOS=0x%02hX TLEN=%u ID=%u ",
	        ip_header->version,
	        ip_header->ihl * 4,
	        ip_header->tos, 
	        ntohs(ip_header->tot_len),
	        ntohs(ip_header->id));
	if (ntohs(ip_header->frag_off) & IP_DF)
		printf("DF ");
	if (ntohs(ip_header->frag_off) & IP_MF)
		printf("MF ");
	if (ntohs(ip_header->frag_off) & 0x1FFF)
		printf("FRAG=%u ",
		        (ntohs(ip_header->frag_off) & 0x1FFF) * 8);
	printf("TTL=%u PROT=%u CSUM=0x%04X ",
	        ip_header->ttl,
	        ip_header->protocol,
	        ntohs(ip_header->check));
	printf("SRC=%u.%u.%u.%u DST=%u.%u.%u.%u\n\n",
	        (ntohl(ip_header->saddr) >> 24) & 0xFF,
	        (ntohl(ip_header->saddr) >> 16) & 0xFF,
	        (ntohl(ip_header->saddr) >> 8) & 0xFF,
	        (ntohl(ip_header->saddr)) & 0xFF,
	        (ntohl(ip_header->daddr) >> 24) & 0xFF,
	        (ntohl(ip_header->daddr) >> 16) & 0xFF,
	        (ntohl(ip_header->daddr) >> 8) & 0xFF,
	        (ntohl(ip_header->daddr)) & 0xFF);
}
