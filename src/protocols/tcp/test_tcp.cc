/*
 * AIEngine a deep packet inspector reverse engineering engine.
 *
 * Copyright (C) 2013-2016  Luis Campo Giralte
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 * Written by Luis Campo Giralte <luis.camp0.2009@gmail.com> 
 *
 */
#include "test_tcp.h"

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE tcptest
#endif
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(tcp_suite1,StackTCPTest)
// check a TCP header values
//
BOOST_AUTO_TEST_CASE (test1_tcp)
{
	unsigned char *pkt = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_http_get);
        int length = raw_packet_ethernet_ip_tcp_http_get_length;
	Packet packet(pkt,length);
	
        // executing the packet
        // forward the packet through the multiplexers
        mux_eth->setPacket(&packet);
        eth->setHeader(packet.getPayload());
	mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet);

        // Check the TCP integrity
        BOOST_CHECK(tcp->getSourcePort() == 53637);
        BOOST_CHECK(tcp->getDestinationPort() == 80);
	BOOST_CHECK(tcp->getTotalBytes() == 809);
}

BOOST_AUTO_TEST_CASE (test2_tcp)
{
        unsigned char *pkt = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_client_hello);
        int length = raw_packet_ethernet_ip_tcp_ssl_client_hello_length;
        Packet packet(pkt,length);
                
        // executing the packet
        // forward the packet through the multiplexers
        mux_eth->setPacket(&packet);
        eth->setHeader(packet.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet);
                
        // Check the TCP integrity
        BOOST_CHECK(tcp->getSourcePort() == 44265);
        BOOST_CHECK(tcp->getDestinationPort() == 443);
        BOOST_CHECK(tcp->getTotalBytes() == 225);
}

// Test case for verify tcp flags
BOOST_AUTO_TEST_CASE (test3_tcp)
{
        unsigned char *pkt = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_client_hello);
        int length = raw_packet_ethernet_ip_tcp_ssl_client_hello_length;
        Packet packet(pkt,length);

        mux_eth->setPacket(&packet);
        eth->setHeader(packet.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet);

	Flow *flow = tcp->getCurrentFlow();

	BOOST_CHECK(flow != nullptr);
        BOOST_CHECK(flow->tcp_info != nullptr);
	BOOST_CHECK(flow->regex_mng.lock() == nullptr);
	SharedPointer<TCPInfo> info = flow->tcp_info;

	BOOST_CHECK(info->syn == 0);
	BOOST_CHECK(info->fin == 0);
	BOOST_CHECK(info->syn_ack == 0);
	BOOST_CHECK(info->ack == 1);
	BOOST_CHECK(info->push == 1);
}

BOOST_AUTO_TEST_CASE (test4_tcp)
{
        unsigned char *pkt1 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_syn);
        int length1 = raw_packet_ethernet_ip_tcp_syn_length;
        Packet packet(pkt1,length1);

        mux_eth->setPacket(&packet);
        eth->setHeader(packet.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet);

        Flow *flow = tcp->getCurrentFlow();

        BOOST_CHECK(flow != nullptr);
        BOOST_CHECK(flow->tcp_info != nullptr);
        SharedPointer<TCPInfo> info = flow->tcp_info;

        BOOST_CHECK(info->syn == 1);
        BOOST_CHECK(info->fin == 0);
        BOOST_CHECK(info->syn_ack == 0);
        BOOST_CHECK(info->ack == 0);
        BOOST_CHECK(info->push == 0);
}

BOOST_AUTO_TEST_CASE (test5_tcp)
{
        unsigned char *pkt1 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_syn_ack);
        int length1 = raw_packet_ethernet_ip_tcp_syn_ack_length;
        Packet packet(pkt1,length1);

        mux_eth->setPacket(&packet);
        eth->setHeader(packet.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet);

        Flow *flow = tcp->getCurrentFlow();

        BOOST_CHECK(flow != nullptr);
        BOOST_CHECK(flow->tcp_info != nullptr);
        SharedPointer<TCPInfo> info = flow->tcp_info;

        BOOST_CHECK(info->syn == 0);
        BOOST_CHECK(info->fin == 0);
        BOOST_CHECK(info->syn_ack == 1);
        BOOST_CHECK(info->ack == 0);
        BOOST_CHECK(info->push == 0);
}

BOOST_AUTO_TEST_CASE (test6_tcp)
{
        unsigned char *pkt1 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_syn);
        int length1 = raw_packet_ethernet_ip_tcp_syn_length;
        Packet packet1(pkt1,length1);
        unsigned char *pkt2 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_syn_ack);
        int length2 = raw_packet_ethernet_ip_tcp_syn_ack_length;
        Packet packet2(pkt2,length2);

        mux_eth->setPacket(&packet1);
        eth->setHeader(packet1.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet1);

        mux_eth->setPacket(&packet2);
        eth->setHeader(packet2.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet2);

        Flow *flow = tcp->getCurrentFlow();

        BOOST_CHECK(flow != nullptr);
        BOOST_CHECK(flow->tcp_info != nullptr);
        SharedPointer<TCPInfo> info = flow->tcp_info;

        BOOST_CHECK(info->syn == 1);
        BOOST_CHECK(info->fin == 0);
        BOOST_CHECK(info->syn_ack == 1);
        BOOST_CHECK(info->ack == 0);
        BOOST_CHECK(info->push == 0);
}

// Test case for verify tcp bad flags
BOOST_AUTO_TEST_CASE (test7_tcp)
{
        unsigned char *pkt1 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_syn_ack_fin);
        int length1 = raw_packet_ethernet_ip_tcp_syn_ack_fin_length;
        Packet packet(pkt1,length1);

        mux_eth->setPacket(&packet);
        eth->setHeader(packet.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet);

        Flow *flow = tcp->getCurrentFlow();

        BOOST_CHECK(flow != nullptr);
        BOOST_CHECK(flow->tcp_info != nullptr);
        SharedPointer<TCPInfo> info = flow->tcp_info;
	PacketAnomalyType pa = flow->getPacketAnomaly();

        BOOST_CHECK(info->syn == 0);
        BOOST_CHECK(info->fin == 1);
        BOOST_CHECK(info->syn_ack == 1);
        BOOST_CHECK(info->ack == 0);
        BOOST_CHECK(info->push == 0);

	// Verfiy that the packet is bad :D
	BOOST_CHECK(pa == PacketAnomalyType::TCP_BAD_FLAGS);
}

// The TCP header is corrupted on terms of length
BOOST_AUTO_TEST_CASE (test8_tcp)
{
        unsigned char *pkt = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_syn_bogus);
        int length = raw_packet_ethernet_ip_tcp_syn_bogus_length;
        Packet packet(pkt,length);

        mux_eth->setPacket(&packet);
        eth->setHeader(packet.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet);

        Flow *flow = tcp->getCurrentFlow();

        BOOST_CHECK(flow != nullptr);
        BOOST_CHECK(flow->tcp_info != nullptr);
	PacketAnomalyType pa = flow->getPacketAnomaly();
	BOOST_CHECK(pa == PacketAnomalyType::TCP_BOGUS_HEADER);
}

#if defined(HAVE_TCP_QOS_METRICS)

// Verify the Connection setup time, time between syn and first ack
// Verify also the application response time
BOOST_AUTO_TEST_CASE (test9_tcp)
{
	std::vector<Packet> pktlist;
        unsigned char *pkt1 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_1);
        int length1 = raw_packet_ethernet_ip_tcp_ssl_1_length;
        Packet packet1(pkt1,length1); // Syn packet
	packet1.setPacketTime(1);
	pktlist.push_back(packet1);
	
        unsigned char *pkt2 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_2);
        int length2 = raw_packet_ethernet_ip_tcp_ssl_2_length;
        Packet packet2(pkt2,length2); // Syn ACK packet
	packet2.setPacketTime(5);
	pktlist.push_back(packet2);

        unsigned char *pkt3 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_3);
        int length3 = raw_packet_ethernet_ip_tcp_ssl_3_length;
        Packet packet3(pkt3,length3); // Ack packet
	packet3.setPacketTime(10);
	pktlist.push_back(packet3);

        unsigned char *pkt4 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_4);
        int length4 = raw_packet_ethernet_ip_tcp_ssl_4_length;
        Packet packet4(pkt4,length4); // Ack with data packet
	packet4.setPacketTime(11);
	pktlist.push_back(packet4);
        
	unsigned char *pkt5 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_5);
        int length5 = raw_packet_ethernet_ip_tcp_ssl_5_length;
        Packet packet5(pkt5,length5); // Ack with no data packet
	packet5.setPacketTime(11);
	pktlist.push_back(packet5);
	
	unsigned char *pkt6 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_6);
        int length6 = raw_packet_ethernet_ip_tcp_ssl_6_length;
        Packet packet6(pkt6,length6); // Ack with data packet
	packet6.setPacketTime(17);
	pktlist.push_back(packet6);

	// Inject the 6 packets 
	for (auto &pkt: pktlist) { 
		mux_eth->setPacket(&pkt);
        	eth->setHeader(pkt.getPayload());
        	mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        	mux_eth->forwardPacket(pkt);
	}
	
        Flow *flow = tcp->getCurrentFlow();

        BOOST_CHECK(flow != nullptr);
        BOOST_CHECK(flow->tcp_info != nullptr);
        SharedPointer<TCPInfo> info = flow->tcp_info;

	BOOST_CHECK(info->state_curr == static_cast<int>(TcpState::ESTABLISHED));
	BOOST_CHECK(info->state_prev == static_cast<int>(TcpState::ESTABLISHED));
	BOOST_CHECK(info->connection_setup_time == 9);
	BOOST_CHECK(info->application_response_time == 6);
}

// Similar test case but with different value results
BOOST_AUTO_TEST_CASE (test10_tcp)
{
        std::vector<Packet> pktlist;
        unsigned char *pkt1 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_1);
        int length1 = raw_packet_ethernet_ip_tcp_ssl_1_length;
        Packet packet1(pkt1,length1); // Syn packet
        packet1.setPacketTime(1);
        pktlist.push_back(packet1);

        unsigned char *pkt2 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_2);
        int length2 = raw_packet_ethernet_ip_tcp_ssl_2_length;
        Packet packet2(pkt2,length2); // Syn ACK packet
        packet2.setPacketTime(1);
        pktlist.push_back(packet2);

        unsigned char *pkt3 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_3);
        int length3 = raw_packet_ethernet_ip_tcp_ssl_3_length;
        Packet packet3(pkt3,length3); // Ack packet
        packet3.setPacketTime(1);
        pktlist.push_back(packet3);

        unsigned char *pkt4 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_4);
        int length4 = raw_packet_ethernet_ip_tcp_ssl_4_length;
        Packet packet4(pkt4,length4); // Ack with data packet
        packet4.setPacketTime(2);
        pktlist.push_back(packet4);

        unsigned char *pkt5 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_5);
        int length5 = raw_packet_ethernet_ip_tcp_ssl_5_length;
        Packet packet5(pkt5,length5); // Ack with no data packet
        packet5.setPacketTime(2);
        pktlist.push_back(packet5);

        unsigned char *pkt6 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_ssl_6);
        int length6 = raw_packet_ethernet_ip_tcp_ssl_6_length;
        Packet packet6(pkt6,length6); // Ack with data packet
        packet6.setPacketTime(2);
        pktlist.push_back(packet6);

        // Inject the 6 packets
        for (auto &pkt: pktlist) {
                mux_eth->setPacket(&pkt);
                eth->setHeader(pkt.getPayload());
                mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
                mux_eth->forwardPacket(pkt);
        }

        Flow *flow = tcp->getCurrentFlow();

        BOOST_CHECK(flow != nullptr);
        BOOST_CHECK(flow->tcp_info != nullptr);
        SharedPointer<TCPInfo> info = flow->tcp_info;

        BOOST_CHECK(info->state_curr == static_cast<int>(TcpState::ESTABLISHED));
        BOOST_CHECK(info->state_prev == static_cast<int>(TcpState::ESTABLISHED));
        BOOST_CHECK(info->connection_setup_time == 0);
        BOOST_CHECK(info->application_response_time == 0);
}

#endif

BOOST_AUTO_TEST_SUITE_END( )

BOOST_FIXTURE_TEST_SUITE(tcp_suite2,StackIPv6TCPTest)

BOOST_AUTO_TEST_CASE (test1_tcp)
{
        unsigned char *pkt = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ipv6_tcp_http_get);
        int length = raw_packet_ethernet_ipv6_tcp_http_get_length;
        Packet packet(pkt,length);

        // executing the packet
        // forward the packet through the multiplexers
        mux_eth->setPacket(&packet);
        eth->setHeader(packet.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet);

        // Check the TCP integrity
        BOOST_CHECK(tcp->getSourcePort() == 1287);
        BOOST_CHECK(tcp->getDestinationPort() == 80);
        BOOST_CHECK(tcp->getTotalBytes() == 797+20);
}

BOOST_AUTO_TEST_CASE (test2_tcp)
{
        unsigned char *pkt = reinterpret_cast <unsigned char*> (raw_ethernet_ipv6_dstopthdr_tcp_http_get);
        int length = raw_ethernet_ipv6_dstopthdr_tcp_http_get_length;
        Packet packet(pkt,length);

        // executing the packet
        // forward the packet through the multiplexers
        mux_eth->setPacket(&packet);
        eth->setHeader(packet.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet);

        // Check the TCP integrity
        BOOST_CHECK(tcp->getSourcePort() == 36951);
        BOOST_CHECK(tcp->getDestinationPort() == 80);
        BOOST_CHECK(tcp->getTotalBytes() == 15+20);
}

BOOST_AUTO_TEST_CASE (test3_tcp)
{
        unsigned char *pkt1 = reinterpret_cast <unsigned char*> (raw_ethernet_ipv6_ahhdr_tcp_syn_flow1);
        int length1 = raw_ethernet_ipv6_ahhdr_tcp_syn_flow1_length;
        Packet packet1(pkt1,length1);

        // executing the packet
        // forward the packet through the multiplexers
        mux_eth->setPacket(&packet1);
        eth->setHeader(packet1.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet1);

        // Check the TCP integrity
        BOOST_CHECK(tcp->getSourcePort() == 17257);
        BOOST_CHECK(tcp->getDestinationPort() == 80);
	BOOST_CHECK(tcp->isSyn() == true);
	BOOST_CHECK(tcp->isFin() == false);
	BOOST_CHECK(tcp->isRst() == false);
	BOOST_CHECK(tcp->isAck() == false);
	BOOST_CHECK(tcp->isPushSet() == false);
        BOOST_CHECK(tcp->getTotalBytes() == 20);
}

BOOST_AUTO_TEST_CASE (test4_tcp)
{
        unsigned char *pkt = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ipv6_hophop_dstopt_tcp_syn);
        int length = raw_packet_ethernet_ipv6_hophop_dstopt_tcp_syn_length;
        Packet packet(pkt,length);

        // executing the packet
        // forward the packet through the multiplexers
        mux_eth->setPacket(&packet);
        eth->setHeader(packet.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet);

        // Check the TCP integrity
        BOOST_CHECK(tcp->getSourcePort() == 55617);
        BOOST_CHECK(tcp->getDestinationPort() == 80);
        BOOST_CHECK(tcp->isSyn() == true);
        BOOST_CHECK(tcp->isFin() == false);
        BOOST_CHECK(tcp->isRst() == false);
        BOOST_CHECK(tcp->isAck() == false);
        BOOST_CHECK(tcp->isPushSet() == false);
        BOOST_CHECK(tcp->getTotalBytes() == 40);
}


BOOST_AUTO_TEST_SUITE_END( )

BOOST_AUTO_TEST_SUITE(tcp_suite3)
// Unit tests for the tcp state machine

BOOST_AUTO_TEST_CASE (test1_tcp)
{
	/***
	for (int i = 0;i < static_cast<int>(TcpState::MAX_STATES);++i ) {
		const struct ST_TCPStateMachine *state = &tcp_states[i];	

		std::cout << "State(" << state << "):" << state->state->name << std::endl;
		std::cout << "Forward" << std::endl;
		for (int j = 0; j < static_cast<int>(TcpFlags::MAX_FLAGS); ++j) {
			std::cout << "(" << j << ")=" << state->state->dir[0].flags[j] << " ";
		}
		std::cout << std::endl;
		std::cout << "Backward" << std::endl;
		for (int j = 0; j < static_cast<int>(TcpFlags::MAX_FLAGS); ++j) {
			std::cout << "(" << j << ")=" << state->state->dir[1].flags[j] << " ";
		}
		std::cout << std::endl;
	}
	*/
	int flags = static_cast<int>(TcpFlags::INVALID);
	FlowDirection dir = FlowDirection::FORWARD;
	int state = static_cast<int>(TcpState::CLOSED);

	int newstate = ((tcp_states[state]).state)->dir[static_cast<int>(dir)].flags[flags];	

	BOOST_CHECK(newstate == 0);

	// receive a syn packet for the three way handshake
	flags = static_cast<int>(TcpFlags::SYN);
	dir = FlowDirection::FORWARD;

	state = newstate;	
	newstate = ((tcp_states[static_cast<int>(state)]).state)->dir[static_cast<int>(dir)].flags[flags];	

	BOOST_CHECK ( newstate == static_cast<int>(TcpState::SYN_SENT));

	flags = static_cast<int>(TcpFlags::SYNACK);
	dir = FlowDirection::BACKWARD;
	state = newstate;	
	newstate = ((tcp_states[newstate]).state)->dir[static_cast<int>(dir)].flags[flags];	

	BOOST_CHECK ( newstate == static_cast<int>(TcpState::SYN_RECEIVED));

	flags = static_cast<int>(TcpFlags::ACK);
	dir = FlowDirection::FORWARD;
	state = newstate;	
	newstate = ((tcp_states[newstate]).state)->dir[static_cast<int>(dir)].flags[flags];	
	BOOST_CHECK ( newstate == static_cast<int>(TcpState::ESTABLISHED));
}

BOOST_AUTO_TEST_CASE (test2_tcp)
{
	// The flow have been established previously
     
        int flags = static_cast<int>(TcpFlags::ACK);
        FlowDirection dir = FlowDirection::BACKWARD;
        int state = static_cast<int>(TcpState::ESTABLISHED);
	int newstate = state;
        newstate = ((tcp_states[newstate]).state)->dir[static_cast<int>(dir)].flags[flags];
        if (newstate == -1) { // Keep on the same state
                newstate = state;
        }
        BOOST_CHECK ( newstate == static_cast<int>(TcpState::ESTABLISHED));

        dir = FlowDirection::FORWARD;
	state = newstate;
        newstate = ((tcp_states[newstate]).state)->dir[static_cast<int>(dir)].flags[flags];
        if (newstate == -1) { // Keep on the same state
                newstate = state;
        }
        BOOST_CHECK ( newstate == static_cast<int>(TcpState::ESTABLISHED));

	flags = static_cast<int>(TcpFlags::ACK);
        dir = FlowDirection::BACKWARD;
        state = newstate;
        newstate = ((tcp_states[newstate]).state)->dir[static_cast<int>(dir)].flags[flags];
        if (newstate == -1) { // Keep on the same state
                newstate = state;
        }
        BOOST_CHECK ( newstate == static_cast<int>(TcpState::ESTABLISHED));
}

BOOST_AUTO_TEST_CASE (test3_tcp)
{
        // The flow have been established previously and a wrong flag appears

        int flags = static_cast<int>(TcpFlags::ACK);
        FlowDirection dir = FlowDirection::BACKWARD;
        int state = static_cast<int>(TcpState::ESTABLISHED);
        int newstate = state;
        newstate = ((tcp_states[newstate]).state)->dir[static_cast<int>(dir)].flags[flags];
        if (newstate == -1) { // Keep on the same state
                newstate = state;
        }
        BOOST_CHECK ( newstate == static_cast<int>(TcpState::ESTABLISHED));

	
        flags = static_cast<int>(TcpFlags::SYNACK);
        dir = FlowDirection::FORWARD;
        state = newstate;
        newstate = ((tcp_states[newstate]).state)->dir[static_cast<int>(dir)].flags[flags];

        if (newstate == -1) { // Keep on the same state
                newstate = state;
        }
        BOOST_CHECK ( newstate == static_cast<int>(TcpState::CLOSED));

}

BOOST_AUTO_TEST_SUITE_END( )

BOOST_FIXTURE_TEST_SUITE(tcp_suite_timeouts,StackTCPTest)
// Unit tests for test the timeouts on the tcp part

BOOST_AUTO_TEST_CASE (test1_tcp) // Two flows, the first expires
{
        unsigned char *pkt1 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_syn);
        int length1 = raw_packet_ethernet_ip_tcp_syn_length;
        Packet packet1(pkt1,length1,0,PacketAnomalyType::NONE,0);
        unsigned char *pkt2 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_syn_2);
        int length2 = raw_packet_ethernet_ip_tcp_syn_2_length;
        Packet packet2(pkt2,length2,0,PacketAnomalyType::NONE,100);

	flow_mng->setTimeout(80);

        mux_eth->setPacket(&packet1);
        eth->setHeader(packet1.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet1);

        BOOST_CHECK(flow_mng->getTotalProcessFlows() == 1);
        BOOST_CHECK(flow_mng->getTotalFlows() == 1);
        BOOST_CHECK(flow_mng->getTotalTimeoutFlows() == 0);

        BOOST_CHECK(flow_cache->getTotalFlows() == 1);
        BOOST_CHECK(flow_cache->getTotalAcquires() == 1);
        BOOST_CHECK(flow_cache->getTotalReleases() == 0);
        BOOST_CHECK(flow_cache->getTotalFails() == 0);

        mux_eth->setPacket(&packet2);
        eth->setHeader(packet2.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet2);

	BOOST_CHECK(flow_mng->getTotalProcessFlows() == 2);
	BOOST_CHECK(flow_mng->getTotalFlows() == 1);
        BOOST_CHECK(flow_mng->getTotalTimeoutFlows() == 1);

        BOOST_CHECK(flow_cache->getTotalFlows() == 1);
        BOOST_CHECK(flow_cache->getTotalAcquires() == 2);
        BOOST_CHECK(flow_cache->getTotalReleases() == 1);
        BOOST_CHECK(flow_cache->getTotalFails() == 0);
}

BOOST_AUTO_TEST_CASE (test2_tcp) // Two flows, none of them expires due to the timeout value
{
        unsigned char *pkt1 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_syn);
        int length1 = raw_packet_ethernet_ip_tcp_syn_length;
        Packet packet1(pkt1,length1,0,PacketAnomalyType::NONE,0);
        unsigned char *pkt2 = reinterpret_cast <unsigned char*> (raw_packet_ethernet_ip_tcp_syn_2);
        int length2 = raw_packet_ethernet_ip_tcp_syn_2_length;
        Packet packet2(pkt2,length2,0,PacketAnomalyType::NONE,100);

        flow_mng->setTimeout(120);

        mux_eth->setPacket(&packet1);
        eth->setHeader(packet1.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet1);

        BOOST_CHECK(flow_mng->getTotalProcessFlows() == 1);
        BOOST_CHECK(flow_mng->getTotalFlows() == 1);
        BOOST_CHECK(flow_mng->getTotalTimeoutFlows() == 0);

        BOOST_CHECK(flow_cache->getTotalFlows() == 1);
        BOOST_CHECK(flow_cache->getTotalAcquires() == 1);
        BOOST_CHECK(flow_cache->getTotalReleases() == 0);
        BOOST_CHECK(flow_cache->getTotalFails() == 0);

        mux_eth->setPacket(&packet2);
        eth->setHeader(packet2.getPayload());
        mux_eth->setNextProtocolIdentifier(eth->getEthernetType());
        mux_eth->forwardPacket(packet2);

        BOOST_CHECK(flow_mng->getTotalProcessFlows() == 2);
        BOOST_CHECK(flow_mng->getTotalFlows() == 2);
        BOOST_CHECK(flow_mng->getTotalTimeoutFlows() == 0);

        BOOST_CHECK(flow_cache->getTotalFlows() == 0);
        BOOST_CHECK(flow_cache->getTotalAcquires() == 2);
        BOOST_CHECK(flow_cache->getTotalReleases() == 0);
        BOOST_CHECK(flow_cache->getTotalFails() == 0);
}


BOOST_AUTO_TEST_SUITE_END( )
