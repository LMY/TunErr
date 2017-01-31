//
// File: PacketUDP.cc
// Created by: User <Email>
// Created on: Thu May 11 15:35:04 2006
//

#include "PacketUDP.h"


PacketUDP::PacketUDP(struct udphdr* header) : PacketTransport(), udp_header(header)
{}


PacketUDP::~PacketUDP()
{}
