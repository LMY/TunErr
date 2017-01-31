//
// File: PacketTCP.cc
// Created by: User <Email>
// Created on: Thu May 11 15:21:53 2006
//

#include "PacketTCP.h"


PacketTCP::PacketTCP(struct tcphdr* header) : PacketTransport(), tcp_header(header)
{}


PacketTCP::~PacketTCP()
{}
