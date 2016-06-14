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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <string>
#include "Multiplexer.h"
#include "FlowForwarder.h"
#include "PacketDispatcher.h"
#include "protocols/ethernet/EthernetProtocol.h"
#include "protocols/ip/IPProtocol.h"
#include "protocols/udp/UDPProtocol.h"
#include "protocols/tcp/TCPProtocol.h"
#include "protocols/ssl/SSLProtocol.h"
#include "protocols/http/HTTPProtocol.h"
#include "protocols/frequency/FrequencyGroup.h"
#include "learner/LearnerEngine.h"
#include "StackLanTest.h"
#include "StackVirtual.h"
#include "StackOpenFlow.h"
#include "ipset/IPSet.h"
#include "ipset/IPBloomSet.h"
#include "EvidenceManager.h"

#ifdef HAVE_REDIS
#include "RedisAdaptor.h"
#endif

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main 
#include <boost/test/unit_test.hpp>

using namespace aiengine;

BOOST_AUTO_TEST_SUITE (test_suite_1) 

BOOST_AUTO_TEST_CASE ( test_case_1 )
{
	MultiplexerPtr m1 = MultiplexerPtr(new Multiplexer());
	MultiplexerPtr m2 = MultiplexerPtr(new Multiplexer());
	MultiplexerPtr m3 = MultiplexerPtr(new Multiplexer());
	MultiplexerPtr m4 = MultiplexerPtr(new Multiplexer());

	BOOST_CHECK(m1->getNumberUpMultiplexers()== 0);
	BOOST_CHECK(m2->getNumberUpMultiplexers()== 0);
	BOOST_CHECK(m3->getNumberUpMultiplexers()== 0);
	BOOST_CHECK(m4->getNumberUpMultiplexers()== 0);

	BOOST_CHECK(m1->getDownMultiplexer().use_count() == 0);
	BOOST_CHECK(m2->getDownMultiplexer().use_count() == 0);
	BOOST_CHECK(m3->getDownMultiplexer().use_count() == 0);
	BOOST_CHECK(m4->getDownMultiplexer().use_count() == 0);

	m1->addDownMultiplexer(m2);
	m1->addUpMultiplexer(m3,1);	
	m1->addUpMultiplexer(m4,2);	
	BOOST_CHECK(m1->getNumberUpMultiplexers()== 2);

	MultiplexerPtrWeak m5 = m1->getUpMultiplexer(1);
	BOOST_CHECK(m5.lock() == m3);

	m5 = m1->getUpMultiplexer(2);
	BOOST_CHECK(m5.lock() == m4);

	m5 = m1->getDownMultiplexer();
	BOOST_CHECK(m5.lock() == m2);

}

BOOST_AUTO_TEST_CASE (test_case_2)
{
        MultiplexerPtr m1 = MultiplexerPtr(new Multiplexer());
        MultiplexerPtr m2 = MultiplexerPtr(new Multiplexer());
        MultiplexerPtr m3 = MultiplexerPtr(new Multiplexer());
        MultiplexerPtr m4 = MultiplexerPtr(new Multiplexer());

        m1->addUpMultiplexer(m2,2);
        m2->addDownMultiplexer(m1);

        m2->addUpMultiplexer(m3,3);
        m3->addDownMultiplexer(m2);

        m3->addUpMultiplexer(m4,4);
        m4->addDownMultiplexer(m3);

        BOOST_CHECK(m1->getNumberUpMultiplexers()== 1);
        BOOST_CHECK(m2->getNumberUpMultiplexers()== 1);
        BOOST_CHECK(m3->getNumberUpMultiplexers()== 1);
        BOOST_CHECK(m4->getNumberUpMultiplexers()== 0);

        // Now check the position of the mux
        MultiplexerPtrWeak w_mux;

        // check positions from m1
        w_mux = m1->getUpMultiplexer(2);
        BOOST_CHECK(w_mux.lock() == m2);

        w_mux = m1->getUpMultiplexer(3);
        BOOST_CHECK(w_mux.lock() == nullptr);

        w_mux = m1->getUpMultiplexer(4);
        BOOST_CHECK(w_mux.lock() == nullptr);

        // check positions from m2
        w_mux = m2->getUpMultiplexer(1);
        BOOST_CHECK(w_mux.lock() == nullptr);

        w_mux = m2->getUpMultiplexer(3);
        BOOST_CHECK(w_mux.lock() == m3);

        w_mux = m2->getUpMultiplexer(4);
        BOOST_CHECK(w_mux.lock() == nullptr);

        // check positions from m3
        w_mux = m3->getUpMultiplexer(2);
        BOOST_CHECK(w_mux.lock() == nullptr);

        w_mux = m3->getUpMultiplexer(3);
        BOOST_CHECK(w_mux.lock() == nullptr);

        w_mux = m3->getUpMultiplexer(4);
        BOOST_CHECK(w_mux.lock() == m4);
}

BOOST_AUTO_TEST_CASE (test_case_3)
{
	PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

	pd->open("../pcapfiles/4udppackets.pcap");
	pd->run();
	pd->close();
	BOOST_CHECK(pd->getTotalPackets() == 4);
}

BOOST_AUTO_TEST_CASE(test_case_4)
{
	EthernetProtocol *eth = new EthernetProtocol();
	MultiplexerPtr mux = MultiplexerPtr(new Multiplexer());

	eth->setMultiplexer(mux);	

	delete eth;
}

BOOST_FIXTURE_TEST_CASE(test_case_5,StackLanTest)
{

	PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

	// connect with the stack
	pd->setDefaultMultiplexer(mux_eth);

	pd->open("../pcapfiles/4udppackets.pcap");
	pd->run();
	pd->close();
	BOOST_CHECK(pd->getTotalPackets() == 4);
	BOOST_CHECK(ip->getTotalValidatedPackets() == 4);
	BOOST_CHECK(ip->getTotalMalformedPackets() == 0);
	BOOST_CHECK(ip->getTotalPackets() == 4);
	BOOST_CHECK(udp->getTotalPackets() == 4);
	BOOST_CHECK(udp->getTotalValidatedPackets() == 4);
	BOOST_CHECK(udp->getTotalMalformedPackets() == 0);
	BOOST_CHECK(tcp->getTotalPackets() == 0);
	BOOST_CHECK(tcp->getTotalValidatedPackets() == 0);
	BOOST_CHECK(tcp->getTotalMalformedPackets() == 0);

	BOOST_CHECK(eth->getTotalBytes() == 655);
	BOOST_CHECK(ip->getTotalBytes() == 599); 
	BOOST_CHECK(udp->getTotalBytes() == 487); 
	BOOST_CHECK(tcp->getTotalBytes() == 0); 
}

BOOST_FIXTURE_TEST_CASE(test_case_6,StackLanTest)
{

        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        pd->open("../pcapfiles/sslflow.pcap");
        pd->run();
        pd->close();
        BOOST_CHECK(pd->getTotalPackets() == 95);
        BOOST_CHECK(ip->getTotalValidatedPackets() == 95);
        BOOST_CHECK(ip->getTotalMalformedPackets() == 0);
        BOOST_CHECK(ip->getTotalPackets() == 95);
        BOOST_CHECK(udp->getTotalPackets() == 0);
        BOOST_CHECK(udp->getTotalValidatedPackets() == 0);
        BOOST_CHECK(udp->getTotalMalformedPackets() == 0);
        BOOST_CHECK(tcp->getTotalPackets() == 95);
        BOOST_CHECK(tcp->getTotalValidatedPackets() == 95);
        BOOST_CHECK(tcp->getTotalMalformedPackets() == 0);

}

BOOST_FIXTURE_TEST_CASE(test_case_7,StackLanTest)
{

        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
	FlowManagerPtr flowmgr = FlowManagerPtr(new FlowManager());
	FlowCachePtr flowcache1 = FlowCachePtr(new FlowCache());
	FlowCachePtr flowcache2 = FlowCachePtr(new FlowCache());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

	// Connect the flow manager and flow cache to their corresponding analyzer
	udp->setFlowManager(flowmgr);
	udp->setFlowCache(flowcache1);

	// No flows on the cache 	
        pd->open("../pcapfiles/4udppackets.pcap");
        pd->run();
        pd->close();
	
	//Checkers
        BOOST_CHECK(flowcache1->getTotalFlows() == 0);
        BOOST_CHECK(flowcache1->getTotalAcquires() == 0);
        BOOST_CHECK(flowcache1->getTotalReleases() == 0);
        BOOST_CHECK(flowcache1->getTotalFails() == 4);
	BOOST_CHECK(flowmgr->getTotalFlows() == 0);

	// One flow on the cache
	flowcache2->createFlows(1);
	udp->setFlowCache(flowcache2);

        pd->open("../pcapfiles/4udppackets.pcap");
        pd->run();
        pd->close();

	//Checkers
        BOOST_CHECK(flowcache2->getTotalFlows() == 0);
        BOOST_CHECK(flowcache2->getTotalAcquires() == 1);
        BOOST_CHECK(flowcache2->getTotalReleases() == 0);
        BOOST_CHECK(flowcache2->getTotalFails() == 0);
	BOOST_CHECK(flowmgr->getTotalFlows() == 1);
	//this->statistics();
	
	// Add one flow on the cache
	flowcache2->createFlows(1);
	tcp->setFlowCache(flowcache2);
	tcp->setFlowManager(flowmgr);

        pd->open("../pcapfiles/sslflow.pcap");
        pd->run();
        pd->close();

        //Checkers
        BOOST_CHECK(flowcache2->getTotalFlows() == 0);
        BOOST_CHECK(flowcache2->getTotalAcquires() == 2);
        BOOST_CHECK(flowcache2->getTotalReleases() == 0);
        BOOST_CHECK(flowcache2->getTotalFails() == 0);
        BOOST_CHECK(flowmgr->getTotalFlows() == 1);

}

BOOST_FIXTURE_TEST_CASE(test_case_8,StackLanTest)
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        FlowManagerPtr flowmgr = FlowManagerPtr(new FlowManager());
        FlowCachePtr flowcache = FlowCachePtr(new FlowCache());
	SharedPointer<FlowForwarder> ff_tcp_aux = SharedPointer<FlowForwarder>(new FlowForwarder());	
	SharedPointer<FlowForwarder> ff_ssl_aux = SharedPointer<FlowForwarder>(new FlowForwarder());	
	SSLProtocolPtr ssl_aux = SSLProtocolPtr(new SSLProtocol());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        flowcache->createFlows(1);
        tcp->setFlowCache(flowcache);
        tcp->setFlowManager(flowmgr);

	// configure the flow forwarder
	tcp->setFlowForwarder(ff_tcp_aux);
	ff_tcp_aux->setProtocol(static_cast<ProtocolPtr>(tcp));
	ff_tcp_aux->addUpFlowForwarder(ff_ssl_aux);

	ssl_aux->setFlowForwarder(ff_ssl_aux);
	ff_ssl_aux->setProtocol(static_cast<ProtocolPtr>(ssl_aux));
	
	//connect the ssl protocol on top of tcp
	ff_tcp_aux->addUpFlowForwarder(ff_ssl_aux);

	ff_ssl_aux->addChecker(std::bind(&SSLProtocol::sslChecker,ssl_aux,std::placeholders::_1));
        ff_ssl_aux->addFlowFunction(std::bind(&SSLProtocol::processFlow,ssl_aux,std::placeholders::_1));

	ssl_aux->increaseAllocatedMemory(4);

        pd->open("../pcapfiles/sslflow.pcap");
        pd->run();
        pd->close();

        //Checkers
        BOOST_CHECK(flowcache->getTotalFlows() == 0);
        BOOST_CHECK(flowcache->getTotalAcquires() == 1);
        BOOST_CHECK(flowcache->getTotalReleases() == 0);
        BOOST_CHECK(flowcache->getTotalFails() == 0);
        BOOST_CHECK(flowmgr->getTotalFlows() == 1);

	//Checkers of the forwarders
	BOOST_CHECK(ff_tcp_aux->getTotalForwardFlows() == 1);
	BOOST_CHECK(ff_tcp_aux->getTotalReceivedFlows() == 56); // just 56 packets with payload;
	BOOST_CHECK(ff_tcp_aux->getTotalFailFlows() == 0);

	// Verify the SSLProtocol values
	BOOST_CHECK(ssl_aux->getTotalBytes() == 41821);
	BOOST_CHECK(ssl_aux->getTotalRecords() == 3);
	BOOST_CHECK(ssl_aux->getTotalClientHellos() == 1);
	BOOST_CHECK(ssl_aux->getTotalServerHellos() == 1);
	BOOST_CHECK(ssl_aux->getTotalCertificates() == 1);
}

BOOST_FIXTURE_TEST_CASE(test_case_9,StackLanTest)
{

        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        SharedPointer<FlowForwarder> ff_tcp_aux = SharedPointer<FlowForwarder>(new FlowForwarder());
        SharedPointer<FlowForwarder> ff_ssl_aux = SharedPointer<FlowForwarder>(new FlowForwarder());
        SharedPointer<FlowForwarder> ff_http_aux = SharedPointer<FlowForwarder>(new FlowForwarder());
        HTTPProtocolPtr http_aux = HTTPProtocolPtr(new HTTPProtocol());
        SSLProtocolPtr ssl_aux = SSLProtocolPtr(new SSLProtocol());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        // configure the flow forwarder
        tcp->setFlowForwarder(ff_tcp_aux);
        ff_tcp_aux->setProtocol(static_cast<ProtocolPtr>(tcp));

        ssl_aux->setFlowForwarder(ff_ssl_aux);
        ff_ssl_aux->setProtocol(static_cast<ProtocolPtr>(ssl_aux));

        //connect the ssl protocol on top of tcp
        ff_tcp_aux->addUpFlowForwarder(ff_ssl_aux);
        ff_ssl_aux->addChecker(std::bind(&SSLProtocol::sslChecker,ssl_aux,std::placeholders::_1));
        ff_ssl_aux->addFlowFunction(std::bind(&SSLProtocol::processFlow,ssl_aux,std::placeholders::_1));

        http_aux->setFlowForwarder(ff_http_aux);
        ff_http_aux->setProtocol(static_cast<ProtocolPtr>(http_aux));

        //connect the http protocol on top of tcp
        ff_tcp_aux->addUpFlowForwarder(ff_http_aux);
        ff_http_aux->addChecker(std::bind(&HTTPProtocol::httpChecker,http_aux,std::placeholders::_1));
        ff_http_aux->addFlowFunction(std::bind(&HTTPProtocol::processFlow,http_aux,std::placeholders::_1));

        pd->open("../pcapfiles/accessgoogle.pcap");
        pd->run();
        pd->close();

        //Checkers of the forwarders
        BOOST_CHECK(ff_tcp_aux->getTotalForwardFlows() == 1);
        BOOST_CHECK(ff_tcp_aux->getTotalReceivedFlows() == 4); // just 56 packets with payload;
        BOOST_CHECK(ff_tcp_aux->getTotalFailFlows() == 0);

	// Verify the UDP part
	BOOST_CHECK(udp->getTotalPackets() == 4);
	BOOST_CHECK(udp->getTotalValidatedPackets() == 4);
	BOOST_CHECK(udp->getTotalBytes() == 252);

	BOOST_CHECK(mux_udp->getTotalReceivedPackets() == 4);
	BOOST_CHECK(mux_udp->getTotalForwardPackets() == 0);// nothing on top of UDP
	BOOST_CHECK(mux_udp->getTotalFailPackets() == 4);// nothing to forward

	// Verify the ICMP part
	BOOST_CHECK(icmp->getTotalPackets() == 0);
	BOOST_CHECK(icmp->getTotalValidatedPackets() == 0);

	BOOST_CHECK(mux_icmp->getTotalReceivedPackets() == 0);
	BOOST_CHECK(mux_icmp->getTotalForwardPackets() == 0);
	BOOST_CHECK(mux_icmp->getTotalFailPackets() == 0);

	// Verify the TCP part

	// verify the SSL Part
        BOOST_CHECK(ssl_aux->getTotalBytes() == 0);

	// verify the HTTP part
	BOOST_CHECK(http_aux->getTotalBytes() == 1826);
}

BOOST_FIXTURE_TEST_CASE(test_case_10,StackLanTest)
{

        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

	this->enableLinkLayerTagging("vlan");

	// Enable VLan Tagging but packets dont have the VLAN tag

        pd->open("../pcapfiles/4udppackets.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(pd->getTotalPackets() == 4);
	BOOST_CHECK(mux_eth->getTotalForwardPackets() == 4);
	BOOST_CHECK(mux_eth->getTotalReceivedPackets() == 4);
	BOOST_CHECK(mux_eth->getTotalFailPackets() == 0);
	
	BOOST_CHECK(mux_vlan->getTotalForwardPackets() == 0);
	BOOST_CHECK(mux_vlan->getTotalReceivedPackets() == 0);
	BOOST_CHECK(mux_vlan->getTotalFailPackets() == 0);
        BOOST_CHECK(vlan->getTotalValidatedPackets() == 0);
        BOOST_CHECK(vlan->getTotalMalformedPackets() == 0);
        BOOST_CHECK(vlan->getTotalPackets() == 0);

        BOOST_CHECK(ip->getTotalValidatedPackets() == 4);
        BOOST_CHECK(ip->getTotalMalformedPackets() == 0);
        BOOST_CHECK(ip->getTotalPackets() == 4);

	// Now inject pcap with VLan Tagging and netbios
	// The trace contains 3 packets.
        
	pd->open("../pcapfiles/flow_vlan_netbios.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(pd->getTotalPackets() == 7);
        BOOST_CHECK(mux_eth->getTotalForwardPackets() == 7);
        BOOST_CHECK(mux_eth->getTotalReceivedPackets() == 7);
        BOOST_CHECK(mux_eth->getTotalFailPackets() == 0);

        BOOST_CHECK(mux_vlan->getTotalForwardPackets() == 3);
        BOOST_CHECK(mux_vlan->getTotalReceivedPackets() == 3);
        BOOST_CHECK(mux_vlan->getTotalFailPackets() == 0);
        BOOST_CHECK(vlan->getTotalValidatedPackets() == 3);
        BOOST_CHECK(vlan->getTotalMalformedPackets() == 0);
        BOOST_CHECK(vlan->getTotalPackets() == 3);

        BOOST_CHECK(ip->getTotalValidatedPackets() == 7);
        BOOST_CHECK(ip->getTotalMalformedPackets() == 0);
        BOOST_CHECK(ip->getTotalPackets() == 7);

}

BOOST_FIXTURE_TEST_CASE(test_case_11,StackLanTest)
{

        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        this->enableLinkLayerTagging("mpls");

        pd->open("../pcapfiles/mpls_icmp.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(pd->getTotalPackets() == 10);
        BOOST_CHECK(mux_eth->getTotalForwardPackets() == 10);
        BOOST_CHECK(mux_eth->getTotalReceivedPackets() == 10);
        BOOST_CHECK(mux_eth->getTotalFailPackets() == 0);

        BOOST_CHECK(mux_mpls->getTotalForwardPackets() == 5);
        BOOST_CHECK(mux_mpls->getTotalReceivedPackets() == 5);
        BOOST_CHECK(mux_mpls->getTotalFailPackets() == 0);
        BOOST_CHECK(mpls->getTotalValidatedPackets() == 5);
        BOOST_CHECK(mpls->getTotalMalformedPackets() == 0);
        BOOST_CHECK(mpls->getTotalPackets() == 5);

        BOOST_CHECK(mux_ip->getTotalForwardPackets() == 10);
        BOOST_CHECK(mux_ip->getTotalReceivedPackets() == 10);
        BOOST_CHECK(mux_ip->getTotalFailPackets() == 0);
        BOOST_CHECK(ip->getTotalValidatedPackets() == 10);
        BOOST_CHECK(ip->getTotalMalformedPackets() == 0);
        BOOST_CHECK(ip->getTotalPackets() == 10);

        BOOST_CHECK(mux_icmp->getTotalForwardPackets() == 0);
        BOOST_CHECK(mux_icmp->getTotalReceivedPackets() == 10);
        BOOST_CHECK(mux_icmp->getTotalFailPackets() == 10);
        BOOST_CHECK(icmp->getTotalValidatedPackets() == 10);
        BOOST_CHECK(icmp->getTotalMalformedPackets() == 0);
        BOOST_CHECK(icmp->getTotalPackets() == 0);
}

BOOST_FIXTURE_TEST_CASE(test_case_12,StackLanTest)
{

        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        pd->open("../pcapfiles/ipv6_tcp_stream.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(pd->getTotalPackets() == 13);
        BOOST_CHECK(mux_eth->getTotalForwardPackets() == 13);
        BOOST_CHECK(mux_eth->getTotalReceivedPackets() == 13);
        BOOST_CHECK(mux_eth->getTotalFailPackets() == 0);

        BOOST_CHECK(mux_ip6->getTotalForwardPackets() == 13);
        BOOST_CHECK(mux_ip6->getTotalReceivedPackets() == 13);
        BOOST_CHECK(mux_ip6->getTotalFailPackets() == 0);

        BOOST_CHECK(ip6->getTotalValidatedPackets() == 13);
        BOOST_CHECK(ip6->getTotalMalformedPackets() == 0);
        BOOST_CHECK(ip6->getTotalPackets() == 13);

        BOOST_CHECK(tcp6->getTotalValidatedPackets() == 13);
        BOOST_CHECK(tcp6->getTotalMalformedPackets() == 0);
        BOOST_CHECK(tcp6->getTotalPackets() == 13);

        BOOST_CHECK(tcp_generic6->getTotalValidatedPackets() == 1);
        BOOST_CHECK(tcp_generic6->getTotalMalformedPackets() == 0);
        BOOST_CHECK(tcp_generic6->getTotalPackets() == 4);
        BOOST_CHECK(tcp_generic6->getTotalBytes() == 213);

}

// test a chaining regex with one flow that matchs on the first and
// on the last packet
BOOST_FIXTURE_TEST_CASE(test_case_13,StackLanTest)
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        RegexManagerPtr rmng = RegexManagerPtr(new RegexManager());

        SharedPointer<Regex> r_head = SharedPointer<Regex>(new Regex("r1","^(its peanut).*$"));
        SharedPointer<Regex> r_tail = SharedPointer<Regex>(new Regex("r2","^(invalid command).*$"));

        r_head->setNextRegex(r_tail);

        rmng->addRegex(r_head);

        tcp_generic6->setRegexManager(rmng);
        tcp6->setRegexManager(rmng);

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        pd->open("../pcapfiles/ipv6_tcp_stream.pcap");
        pd->run();
        pd->close();

	// Check pcap file for see the results
	BOOST_CHECK(r_head->getMatchs() == 1);
	BOOST_CHECK(r_head->getTotalEvaluates() == 1);

	BOOST_CHECK(r_tail->getMatchs() == 1);
	BOOST_CHECK(r_tail->getTotalEvaluates() == 3);
}

// Test with a generic ipv6 exploit
BOOST_FIXTURE_TEST_CASE(test_case_14,StackLanTest)
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        RegexManagerPtr rmng = RegexManagerPtr(new RegexManager());
        SharedPointer<Regex> r_generic = SharedPointer<Regex>(new Regex("generic exploit","^.*\\x90\\x90\\x90\\x90\\x90\\x90\\x90\\x90.*$"));

        rmng->addRegex(r_generic);

        tcp_generic6->setRegexManager(rmng);
        tcp6->setRegexManager(rmng);

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        pd->open("../pcapfiles/generic_exploit_ipv6_defcon20.pcap");
        pd->run();
        pd->close();

        // Check pcap file for see the results
        BOOST_CHECK(r_generic->getMatchs() == 1);
        BOOST_CHECK(r_generic->getTotalEvaluates() == 1);

	BOOST_CHECK(tcp6->getTotalPackets() == 86);
	BOOST_CHECK(tcp6->getTotalBytes() == 68823);
	BOOST_CHECK(tcp6->getTotalValidatedPackets() == 86);
	BOOST_CHECK(tcp6->getTotalMalformedPackets() == 0);

	BOOST_CHECK(flow_table_tcp->getTotalFlows() == 0); // The flow is on the cache
	BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 1);
	BOOST_CHECK(flow_cache_tcp->getTotalAcquires() == 1);
	BOOST_CHECK(flow_cache_tcp->getTotalFails() == 0);

        BOOST_CHECK(tcp_generic6->getTotalValidatedPackets() == 1);
        BOOST_CHECK(tcp_generic6->getTotalMalformedPackets() == 0);
        BOOST_CHECK(tcp_generic6->getTotalPackets() == 49);
        BOOST_CHECK(tcp_generic6->getTotalBytes() == 66067);
}

// A true negative test 
BOOST_FIXTURE_TEST_CASE(test_case_15,StackLanTest)
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        RegexManagerPtr rmng = RegexManagerPtr(new RegexManager());
        SharedPointer<Regex> r_generic = SharedPointer<Regex>(new Regex("generic exploit","^.*\x90\x90\x90\x90\x90\x90\x90\x90.*$"));

        rmng->addRegex(r_generic);

        tcp_generic->setRegexManager(rmng);
        tcp->setRegexManager(rmng);

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        pd->open("../pcapfiles/polymorphic_clet32bits_port1986.pcap");
        pd->run();
        pd->close();

        // Check pcap file for see the results
        BOOST_CHECK(r_generic->getMatchs() == 0);
        BOOST_CHECK(r_generic->getTotalEvaluates() == 1);

        BOOST_CHECK(tcp->getTotalPackets() == 8);
        BOOST_CHECK(tcp->getTotalBytes() == 620);
        BOOST_CHECK(tcp->getTotalValidatedPackets() == 8);
        BOOST_CHECK(tcp->getTotalMalformedPackets() == 0);

       	BOOST_CHECK(tcp_generic->getTotalBytes() == 348);
        BOOST_CHECK(tcp_generic->getTotalPackets() == 1);
}



// Test dual stack 
// use the same TCPGenericProtocol for IPv4 and IPv6
BOOST_FIXTURE_TEST_CASE(test_case_16,StackLanTest)
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        RegexManagerPtr rmng = RegexManagerPtr(new RegexManager());
        SharedPointer<Regex> r_generic = SharedPointer<Regex>(new Regex("generic exploit","^.*\\x90\\x90\\x90\\x90.*$"));

        ff_tcp->removeUpFlowForwarder(ff_tcp_generic6);
        ff_tcp6->removeUpFlowForwarder(ff_tcp_generic6);
        ff_tcp6->addUpFlowForwarder(ff_tcp_generic);

        rmng->addRegex(r_generic);
        tcp_generic->setRegexManager(rmng);
        tcp->setRegexManager(rmng);
        tcp6->setRegexManager(rmng);

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        pd->open("../pcapfiles/generic_exploit_ipv6_defcon20.pcap");
        pd->run();
        pd->close();

        // Check pcap file for see the results
        BOOST_CHECK(r_generic->getMatchs() == 1);
        BOOST_CHECK(r_generic->getTotalEvaluates() == 1);

        BOOST_CHECK(tcp->getTotalPackets() == 0);
        BOOST_CHECK(tcp->getTotalBytes() == 0);
        BOOST_CHECK(tcp->getTotalValidatedPackets() == 0);
        BOOST_CHECK(tcp->getTotalMalformedPackets() == 0);

        BOOST_CHECK(tcp6->getTotalPackets() == 86);
        BOOST_CHECK(tcp6->getTotalBytes() == 68823);
        BOOST_CHECK(tcp6->getTotalValidatedPackets() == 86);
        BOOST_CHECK(tcp6->getTotalMalformedPackets() == 0);

        BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 1);
        BOOST_CHECK(flow_table_tcp->getTotalFlows() == 0);
        BOOST_CHECK(flow_cache_tcp->getTotalAcquires() == 1);
        BOOST_CHECK(flow_cache_tcp->getTotalFails() == 0);

        BOOST_CHECK(tcp_generic->getTotalValidatedPackets() == 1);
        BOOST_CHECK(tcp_generic->getTotalMalformedPackets() == 0);
        BOOST_CHECK(tcp_generic->getTotalPackets() == 49);
        BOOST_CHECK(tcp_generic->getTotalBytes() == 66067);

	// Inject IPv4 pcap file
	// polymorphic_clet32bits_port1986.pcap
        pd->open("../pcapfiles/polymorphic_clet32bits_port1986.pcap");
        pd->run();
        pd->close();

        // Check pcap file for see the results
        BOOST_CHECK(r_generic->getMatchs() == 1);
        BOOST_CHECK(r_generic->getTotalEvaluates() == 2);

        BOOST_CHECK(tcp->getTotalPackets() == 8);
        BOOST_CHECK(tcp->getTotalBytes() == 620);
        BOOST_CHECK(tcp->getTotalValidatedPackets() == 8);
        BOOST_CHECK(tcp->getTotalMalformedPackets() == 0);

        BOOST_CHECK(tcp6->getTotalPackets() == 86);
        BOOST_CHECK(tcp6->getTotalBytes() == 68823);
        BOOST_CHECK(tcp6->getTotalValidatedPackets() == 86);
        BOOST_CHECK(tcp6->getTotalMalformedPackets() == 0);

        BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 2);
        BOOST_CHECK(flow_table_tcp->getTotalFlows() == 1);
        BOOST_CHECK(flow_cache_tcp->getTotalAcquires() == 2);
        BOOST_CHECK(flow_cache_tcp->getTotalFails() == 0);

        BOOST_CHECK(tcp_generic->getTotalValidatedPackets() == 2);
        BOOST_CHECK(tcp_generic->getTotalMalformedPackets() == 0);
        BOOST_CHECK(tcp_generic->getTotalPackets() == 49 + 1);
        BOOST_CHECK(tcp_generic->getTotalBytes() == 66067 + 348);

	//dumpFlows();
}

BOOST_FIXTURE_TEST_CASE(test_case_17,StackLanTest) // Test a IPv6 flow with  authentication header
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        pd->open("../pcapfiles/ipv6_ah.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(pd->getTotalPackets() == 10);
        BOOST_CHECK(mux_eth->getTotalForwardPackets() == 10);
        BOOST_CHECK(mux_eth->getTotalReceivedPackets() == 10);
        BOOST_CHECK(mux_eth->getTotalFailPackets() == 0);

        BOOST_CHECK(ip6->getTotalValidatedPackets() == 10);
        BOOST_CHECK(ip6->getTotalMalformedPackets() == 0);
        BOOST_CHECK(ip6->getTotalPackets() == 10);
        BOOST_CHECK(ip6->getTotalBytes() == 947);
        
        BOOST_CHECK(tcp6->getTotalValidatedPackets() == 10);
        BOOST_CHECK(tcp6->getTotalMalformedPackets() == 0);
        BOOST_CHECK(tcp6->getTotalPackets() == 10);

        BOOST_CHECK(tcp_generic6->getTotalValidatedPackets() == 1);
        BOOST_CHECK(tcp_generic6->getTotalMalformedPackets() == 0);
        BOOST_CHECK(tcp_generic6->getTotalPackets() == 2);
        BOOST_CHECK(tcp_generic6->getTotalBytes() == 103);
}

BOOST_FIXTURE_TEST_CASE(test_case_18,StackLanTest) // Tests timeouts with two different pcap files 
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        pd->open("../pcapfiles/accessgoogle.pcap");
        pd->run();
        pd->close();
	
	BOOST_CHECK(flow_table_udp->getTotalProcessFlows() == 1);
	BOOST_CHECK(flow_table_udp->getTotalFlows() == 1);
	BOOST_CHECK(flow_table_udp->getTotalTimeoutFlows() == 0);

	BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 1);
	BOOST_CHECK(flow_table_tcp->getTotalFlows() == 1);
	BOOST_CHECK(flow_table_tcp->getTotalTimeoutFlows() == 0);

        pd->open("../pcapfiles/amazon_4ssl_flows.pcap");
        pd->run();
        pd->close();

	BOOST_CHECK(flow_table_udp->getTotalProcessFlows() == 1);
	BOOST_CHECK(flow_table_udp->getTotalFlows() == 1);

	// There is no timeout for udp because there is no udp traffic
	// on the second pcapfile
	BOOST_CHECK(flow_table_udp->getTotalTimeoutFlows() == 0);

	BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 5);
	BOOST_CHECK(flow_table_tcp->getTotalFlows() == 4);
	BOOST_CHECK(flow_table_tcp->getTotalTimeoutFlows() == 1);
}

BOOST_FIXTURE_TEST_CASE(test_case_19,StackLanTest) // Tests timeouts with two different pcap files, timeout of one year
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

	flow_table_udp->setTimeout(60*60*24*365);
	flow_table_tcp->setTimeout(60*60*24*365);

        pd->open("../pcapfiles/accessgoogle.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(flow_table_udp->getTotalProcessFlows() == 1);
        BOOST_CHECK(flow_table_udp->getTotalFlows() == 1);
        BOOST_CHECK(flow_table_udp->getTotalTimeoutFlows() == 0);

        BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 1);
        BOOST_CHECK(flow_table_tcp->getTotalFlows() == 1);
        BOOST_CHECK(flow_table_tcp->getTotalTimeoutFlows() == 0);

        pd->open("../pcapfiles/amazon_4ssl_flows.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(flow_table_udp->getTotalProcessFlows() == 1);
        BOOST_CHECK(flow_table_udp->getTotalFlows() == 1);
        BOOST_CHECK(flow_table_udp->getTotalTimeoutFlows() == 0);

        BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 5);
        BOOST_CHECK(flow_table_tcp->getTotalFlows() == 5);
        BOOST_CHECK(flow_table_tcp->getTotalTimeoutFlows() == 0);
}

BOOST_FIXTURE_TEST_CASE(test_case_20,StackLanTest) // Tests for release the caches
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

	ssl->increaseAllocatedMemory(4);

        //flow_table_udp->setTimeout(60*60*24*365);
        //flow_table_tcp->setTimeout(60*60*24*365);

        pd->open("../pcapfiles/amazon_4ssl_flows.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 4);
        BOOST_CHECK(flow_table_tcp->getTotalFlows() == 4); 
        BOOST_CHECK(flow_table_tcp->getTotalTimeoutFlows() == 0);

	releaseCaches();

	for (auto &f: flow_table_tcp->getFlowTable()) {
		BOOST_CHECK(f->getSSLInfo() == nullptr);
	}
}

// Test Regex linked with a generic ipv6 exploit
BOOST_FIXTURE_TEST_CASE(test_case_21,StackLanTest)
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        RegexManagerPtr rmng = RegexManagerPtr(new RegexManager());
        SharedPointer<Regex> r1 = SharedPointer<Regex>(new Regex("generic exploit1","^(No hacker).*$"));
        SharedPointer<Regex> r2 = SharedPointer<Regex>(new Regex("generic exploit2","^(Upgrade Your Liquor Cabinet).*$"));
        SharedPointer<Regex> r3 = SharedPointer<Regex>(new Regex("generic exploit3","^\x90\x90\x90\x90\x90\x90\x90\x90.*$"));
        SharedPointer<Regex> r4 = SharedPointer<Regex>(new Regex("generic exploit4","^(Upgrade Your Liquor Cabinet).*$"));

	r1->setNextRegex(r2);
	r2->setNextRegex(r3);
	r3->setNextRegex(r4);

        rmng->addRegex(r1);

        tcp_generic6->setRegexManager(rmng);
        tcp6->setRegexManager(rmng);

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        pd->open("../pcapfiles/generic_exploit_ipv6_defcon20.pcap");
        pd->run();
        pd->close();

        // Check the regex for results
        BOOST_CHECK(r1->getMatchs() == 1);
        BOOST_CHECK(r1->getTotalEvaluates() == 1);

        BOOST_CHECK(r2->getMatchs() == 1);
        BOOST_CHECK(r2->getTotalEvaluates() == 46);
        
	BOOST_CHECK(r3->getMatchs() == 1);
        BOOST_CHECK(r3->getTotalEvaluates() == 1);
        
	BOOST_CHECK(r4->getMatchs() == 1);
        BOOST_CHECK(r4->getTotalEvaluates() == 1);
}

BOOST_FIXTURE_TEST_CASE(test_case_22,StackLanTest) // Tests for release the caches on SMTP
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());

        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        smtp->increaseAllocatedMemory(1);

        pd->open("../pcapfiles/smtp.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 1);
        BOOST_CHECK(flow_table_tcp->getTotalFlows() == 1);
        BOOST_CHECK(flow_table_tcp->getTotalTimeoutFlows() == 0);
      
	// there is only one flow	
	SharedPointer<Flow> f = *flow_table_tcp->getFlowTable().begin();
 
        BOOST_CHECK(f->getSMTPInfo() != nullptr);
       	SharedPointer<SMTPInfo> info = f->getSMTPInfo();
	BOOST_CHECK(info != nullptr);

	// Verify some information about the SMTPInfo object
	BOOST_CHECK(info->getTotalDataBytes() >= 20389);
	BOOST_CHECK(info->getTotalDataBlocks() == 1);
	BOOST_CHECK(info->getIsData() == false);
 
        BOOST_CHECK(f->getHTTPInfo() == nullptr);
        BOOST_CHECK(f->getSSLInfo() == nullptr);

        releaseCaches();

        BOOST_CHECK(f->layer7info == nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_case_23,StackLanTest) // Tests for release the caches and with ssl traffic 
{
	// The pcap contains 4 ssl flows with the same cert name
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        // connect with the stack
        pd->setDefaultMultiplexer(mux_eth);

        ssl->increaseAllocatedMemory(4);

        pd->open("../pcapfiles/amazon_4ssl_flows.pcap");
        pd->setPcapFilter("port 57077");
        pd->run();
        pd->close();

        BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 1);
        BOOST_CHECK(flow_table_tcp->getTotalFlows() == 1);
        BOOST_CHECK(flow_table_tcp->getTotalTimeoutFlows() == 0);

        // there is only one flow
        SharedPointer<Flow> f = *flow_table_tcp->getFlowTable().begin();

        BOOST_CHECK(f->getSSLInfo() != nullptr);
        BOOST_CHECK(f->getSSLInfo()->host != nullptr);
	SharedPointer<StringCache> str = f->getSSLInfo()->host;
        SharedPointer<SSLInfo> info = f->getSSLInfo();

	std::string cnname("images-na.ssl-images-amazon.com");
	BOOST_CHECK(cnname.compare(str->getName()) == 0);

        releaseCaches();

        BOOST_CHECK(f->getSSLInfo() == nullptr);
	// The str should be empty
	BOOST_CHECK(str->getNameSize() == 0);

	// Inject two flows 	
        pd->open("../pcapfiles/amazon_4ssl_flows.pcap");
        pd->setPcapFilter("port 57078 or port 57079");
	pd->run();
	pd->close();

        BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 3);
        BOOST_CHECK(flow_table_tcp->getTotalFlows() == 3);
        BOOST_CHECK(flow_table_tcp->getTotalTimeoutFlows() == 0);

        f = *flow_table_tcp->getFlowTable().begin();
	BOOST_CHECK(f->getSSLInfo() == nullptr);

	// Both flows points to ptr and now ptr contains the name
	BOOST_CHECK(cnname.compare(str->getName()) == 0);
	int process_flows = 0;
	for (auto &ff: flow_table_tcp->getFlowTable()) {
		if (ff != f) { 
			BOOST_CHECK(ff->getSSLInfo() != nullptr);
			BOOST_CHECK(ff->getSSLInfo()->host != nullptr);
			BOOST_CHECK(ff->getSSLInfo()->host == str);
			BOOST_CHECK(cnname.compare(str->getName()) == 0);
			++process_flows;
		}
	}	
	BOOST_CHECK(process_flows == 2);
        
	releaseCaches();
	
	for (auto &ff: flow_table_tcp->getFlowTable()) {
		BOOST_CHECK(ff->getSSLInfo() == nullptr);
	}
	BOOST_CHECK(str->getNameSize() == 0);
       
	// Inject the last flow 
	pd->open("../pcapfiles/amazon_4ssl_flows.pcap");
        pd->setPcapFilter("port 57080");
	pd->run();
	pd->close();

        BOOST_CHECK(flow_table_tcp->getTotalProcessFlows() == 4);
        BOOST_CHECK(flow_table_tcp->getTotalFlows() == 4);
        BOOST_CHECK(flow_table_tcp->getTotalTimeoutFlows() == 0);

	BOOST_CHECK(cnname.compare(str->getName()) == 0);

	for (auto &ff: flow_table_tcp->getFlowTable()) {
		if (ff->getSourcePort() == 57080) { 
			BOOST_CHECK(ff->getSSLInfo() != nullptr);
			BOOST_CHECK(ff->getSSLInfo()->host != nullptr);
			BOOST_CHECK(ff->getSSLInfo()->host == str);
			BOOST_CHECK(cnname.compare(str->getName()) == 0);
		} else {
			BOOST_CHECK(ff->getSSLInfo() == nullptr);
		}
	}	
}


BOOST_AUTO_TEST_SUITE_END( )

BOOST_AUTO_TEST_SUITE (test_suite_stack_lan) // Test cases for real stacks StackLan 

BOOST_AUTO_TEST_CASE ( test_case_1 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
	StackLanPtr stack = StackLanPtr(new StackLan());

	stack->setTotalTCPFlows(2);
	stack->enableFrequencyEngine(true);
	pd->setStack(stack);
	pd->open("../pcapfiles/two_http_flows_noending.pcap");
        pd->run();
        pd->close();

	FrequencyGroup<std::string> group_by_ip;

       	group_by_ip.setName("by destination IP");
	group_by_ip.agregateFlowsByDestinationAddress(stack->getTCPFlowManager().lock());
	group_by_ip.compute();

	BOOST_CHECK(group_by_ip.getReferenceFlows().size() == 2);
	BOOST_CHECK(group_by_ip.getTotalProcessFlows() == 2);
	BOOST_CHECK(group_by_ip.getTotalComputedFrequencies() == 2);

        FrequencyGroup<std::string> group_by_port;

	FlowManagerPtr fm = stack->getTCPFlowManager().lock();

        group_by_port.setName("by destination port");
        group_by_port.agregateFlowsByDestinationPort(fm);
        group_by_port.compute();

	BOOST_CHECK(group_by_port.getTotalProcessFlows() == 0);
	BOOST_CHECK(group_by_port.getTotalComputedFrequencies() == 0);

	// Check the relaseCache functionality with the frequencies

	for (auto &flow: fm->getFlowTable()) {
		BOOST_CHECK(flow->frequencies != nullptr);
		BOOST_CHECK(flow->packet_frequencies != nullptr);
	} 
	stack->releaseCaches();
	for (auto &flow: fm->getFlowTable()) {
		BOOST_CHECK(flow->frequencies == nullptr);
		BOOST_CHECK(flow->packet_frequencies == nullptr);
	} 
}

BOOST_AUTO_TEST_CASE ( test_case_2 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());
	LearnerEnginePtr learner = LearnerEnginePtr(new LearnerEngine());

        stack->setTotalTCPFlows(2);
        stack->enableFrequencyEngine(true);
        pd->setStack(stack);
        pd->open("../pcapfiles/two_http_flows_noending.pcap");
        pd->run();
        pd->close();

        FrequencyGroup<std::string> group_by_port;

        group_by_port.setName("by destination port");
        group_by_port.agregateFlowsByDestinationPort(stack->getTCPFlowManager().lock());
        group_by_port.compute();

        BOOST_CHECK(group_by_port.getTotalProcessFlows() == 2);
        BOOST_CHECK(group_by_port.getTotalComputedFrequencies() == 1);

	// pass the flows to the Learner engine
	learner->agregateFlows(group_by_port.getReferenceFlows());	
	learner->compute();
	std::string header("^\\x47\\x45\\x54\\x20\\x2f");// a GET on hexa
	std::string reg(learner->getRegularExpression());

	BOOST_CHECK(header.compare(0,header.length(),reg,0,header.length())== 0);
}


BOOST_AUTO_TEST_CASE ( test_case_3 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());
        LearnerEnginePtr learner = LearnerEnginePtr(new LearnerEngine());
	std::vector<WeakPointer<Flow>> flow_list;

        stack->setTotalTCPFlows(2);
        stack->enableFrequencyEngine(true);
        pd->setStack(stack);
        pd->open("../pcapfiles/two_http_flows_noending.pcap");
        pd->run();
        pd->close();

        FrequencyGroup<std::string> group_by_port;

        group_by_port.setName("by destination port");
        group_by_port.agregateFlowsByDestinationPort(stack->getTCPFlowManager().lock());
        group_by_port.compute();

        BOOST_CHECK(group_by_port.getTotalProcessFlows() == 2);
        BOOST_CHECK(group_by_port.getTotalComputedFrequencies() == 1);

        flow_list = group_by_port.getReferenceFlowsByKey("1443");

        // The flow_list should contains zero entries
        BOOST_CHECK(flow_list.size() == 0);

	flow_list = group_by_port.getReferenceFlowsByKey("80");

	// The flow_list should contains two entries
        BOOST_CHECK(flow_list.size() == 2);

        // pass the flows to the Learner engine
        learner->agregateFlows(flow_list);
        learner->compute();
        std::string header("^\\x47\\x45\\x54\\x20\\x2f");// a GET on hexa
        std::string reg(learner->getRegularExpression());

        BOOST_CHECK(header.compare(0,header.length(),reg,0,header.length())== 0);
}

BOOST_AUTO_TEST_CASE ( test_case_4 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());
        LearnerEnginePtr learner = LearnerEnginePtr(new LearnerEngine());
        std::vector<WeakPointer<Flow>> flow_list;

        stack->setTotalTCPFlows(4);
        stack->enableFrequencyEngine(true);
        pd->setStack(stack);
        pd->open("../pcapfiles/tor_4flows.pcap");
        pd->run();
        pd->close();
 
        FrequencyGroup<std::string> group_by_port;
 
        group_by_port.setName("by destination port");
        group_by_port.agregateFlowsByDestinationPort(stack->getTCPFlowManager().lock());
        group_by_port.compute();

        BOOST_CHECK(group_by_port.getTotalProcessFlows() == 4);
        BOOST_CHECK(group_by_port.getTotalComputedFrequencies() == 1);

        flow_list = group_by_port.getReferenceFlowsByKey("80");

        // The flow_list should contains two entries
        BOOST_CHECK(flow_list.size() == 4);
 
        // pass the flows to the Learner engine
        learner->agregateFlows(flow_list);
        learner->compute();
        std::string header("^\\x16\\x03\\x01\\x00\\xd1\\x01\\x00\\x00\\xcd\\x03\\x01\\x52\\xc1\\xd5\\x86\\xd0\\xd3\\x8f\\x87\\xb8\\xf1\\x6e\\x0f\\xe1\\x59\\xff");// a SSL header on hexa
        std::string reg(learner->getRegularExpression());
       
        BOOST_CHECK(header.compare(0,header.length(),reg,0,header.length())== 0);
}

BOOST_AUTO_TEST_CASE ( test_case_5 ) // integrate the learner and the FrequencyGroups 
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());
        LearnerEnginePtr learner = LearnerEnginePtr(new LearnerEngine());
        std::vector<WeakPointer<Flow>> flow_list;

        stack->setTotalTCPFlows(2);
        stack->enableFrequencyEngine(true);
        pd->setStack(stack);
        pd->open("../pcapfiles/two_http_flows_noending.pcap");
        pd->run();
        pd->close();

        FrequencyGroup<std::string> group;

        group.setName("by destination port");
        group.agregateFlowsByDestinationAddressAndPort(stack->getTCPFlowManager().lock());
        group.compute();

        BOOST_CHECK(group.getTotalProcessFlows() == 2);
        BOOST_CHECK(group.getTotalComputedFrequencies() == 2);

	auto it = group.begin();

	BOOST_CHECK( it != group.end());

	FrequencyGroupItemPtr fg = it->second;
	
	flow_list = fg->getReferenceFlows();
        BOOST_CHECK(flow_list.size() == 1);
	
	std::string cad_group("95.100.96.10:80");
	BOOST_CHECK(cad_group.compare(it->first) == 0);

        // pass the flows to the Learner engine
	learner->reset();
        learner->agregateFlows(flow_list);
        learner->compute();
        std::string header("^\\x47\\x45\\x54\\x20\\x2f\\x42");// a GET on hexa
        std::string reg(learner->getRegularExpression());

        BOOST_CHECK(header.compare(0,header.length(),reg,0,header.length())== 0);

	++it;

	cad_group = "95.100.96.48:80";

	BOOST_CHECK( it != group.end());
	BOOST_CHECK(cad_group.compare(it->first) == 0);
	fg = it->second;
	
	flow_list = fg->getReferenceFlows();
        BOOST_CHECK(flow_list.size() == 1);

        learner->reset();
        learner->agregateFlows(flow_list);
        learner->compute();

	header = "^\\x47\\x45\\x54\\x20\\x2f\\x63";
	reg = learner->getRegularExpression();

        BOOST_CHECK(header.compare(0,header.length(),reg,0,header.length())== 0);
	++it;
	BOOST_CHECK(it == group.end());

}

// Check the file format support for pcapng files
BOOST_AUTO_TEST_CASE ( test_case_6 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());

        stack->setTotalTCPFlows(1);
        stack->setTotalUDPFlows(2);
        pd->setStack(stack);

        pd->open("../pcapfiles/icq.pcapng");
        pd->run();
        pd->close();

        pd->open("../pcapfiles/4udppackets.pcap");
        pd->run();
        pd->close();

	FlowManagerPtr flows_tcp = stack->getTCPFlowManager().lock();
	FlowManagerPtr flows_udp = stack->getUDPFlowManager().lock();

	BOOST_CHECK(flows_tcp->getTotalFlows() == 1);
	BOOST_CHECK(flows_udp->getTotalFlows() == 1);

	auto ft = flows_tcp->getFlowTable();
	for (auto &flow: ft) {
		BOOST_CHECK(flow->getProtocol() == IPPROTO_TCP);
	}
	ft = flows_udp->getFlowTable();
	for (auto &flow: ft) {
		BOOST_CHECK(flow->getProtocol() == IPPROTO_UDP);
	}
}

// Test the IPset functionality 
BOOST_AUTO_TEST_CASE ( test_case_7 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());
	SharedPointer<IPSet> ipset_tcp = SharedPointer<IPSet>(new IPSet("IPSet on TCP"));
	SharedPointer<IPSetManager> ipset_mng = SharedPointer<IPSetManager>(new IPSetManager());

	ipset_tcp->addIPAddress("69.64.34.124");
	ipset_tcp->addIPAddress("69.64.34.125");

	ipset_mng->addIPSet(ipset_tcp);

	stack->setTCPIPSetManager(ipset_mng);
        stack->setTotalTCPFlows(1);
        pd->setStack(stack);

        pd->open("../pcapfiles/icq.pcapng");
        pd->run();
        pd->close();

	BOOST_CHECK(ipset_tcp->getTotalIPs() == 2);
	BOOST_CHECK(ipset_tcp->getTotalLookups() == 1);
	BOOST_CHECK(ipset_tcp->getTotalLookupsIn() == 1);
	BOOST_CHECK(ipset_tcp->getTotalLookupsOut() == 0);

        FlowManagerPtr flows_tcp = stack->getTCPFlowManager().lock();

        BOOST_CHECK(flows_tcp->getTotalFlows() == 1);

        auto ft = flows_tcp->getFlowTable();
        for (auto it = ft.begin(); it != ft.end(); ++it) {
                SharedPointer<Flow> flow = *it;

                BOOST_CHECK(flow->getProtocol() == IPPROTO_TCP);
        }
}

// Test the IPset functionality
BOOST_AUTO_TEST_CASE ( test_case_8 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());
        SharedPointer<IPSet> ipset_tcp = SharedPointer<IPSet>(new IPSet("IPSet 1"));
	SharedPointer<IPSetManager> ipset_mng = SharedPointer<IPSetManager>(new IPSetManager());

	ipset_mng->addIPSet(ipset_tcp);
        ipset_tcp->addIPAddress("69.64.34.1");

        stack->setTCPIPSetManager(ipset_mng);
        stack->setTotalTCPFlows(1);
        pd->setStack(stack);

        pd->open("../pcapfiles/icq.pcapng");
        pd->run();
        pd->close();

        BOOST_CHECK(ipset_tcp->getTotalIPs() == 1);
        BOOST_CHECK(ipset_tcp->getTotalLookups() == 1);
        BOOST_CHECK(ipset_tcp->getTotalLookupsIn() == 0);
        BOOST_CHECK(ipset_tcp->getTotalLookupsOut() == 1);
}

#ifdef HAVE_BLOOMFILTER 
// Test the IPBloomSet functionality
BOOST_AUTO_TEST_CASE ( test_case_9 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());
        SharedPointer<IPBloomSet> ipset_tcp = SharedPointer<IPBloomSet>(new IPBloomSet("IPBloomSet 1"));
        SharedPointer<IPSetManager> ipset_mng = SharedPointer<IPSetManager>(new IPSetManager());

        ipset_mng->addIPSet(ipset_tcp);
        ipset_tcp->addIPAddress("69.64.34.1");

        stack->setTCPIPSetManager(ipset_mng);
        stack->setTotalTCPFlows(1);
        pd->setStack(stack);

        pd->open("../pcapfiles/icq.pcapng");
        pd->run();
        pd->close();

        BOOST_CHECK(ipset_tcp->getTotalIPs() == 1);
        BOOST_CHECK(ipset_tcp->getTotalLookups() == 1);
        BOOST_CHECK(ipset_tcp->getTotalLookupsIn() == 0);
        BOOST_CHECK(ipset_tcp->getTotalLookupsOut() == 1);
}

BOOST_AUTO_TEST_CASE ( test_case_10 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());
        SharedPointer<IPBloomSet> ipset_tcp = SharedPointer<IPBloomSet>(new IPBloomSet("IPBloomSet 1"));
        SharedPointer<IPSetManager> ipset_mng = SharedPointer<IPSetManager>(new IPSetManager());

        ipset_mng->addIPSet(ipset_tcp);

	for (int i = 1 ; i < 255; ++i ) {
		std::stringstream ipstr;
		
		ipstr << "74.12.3." << i;
        	ipset_tcp->addIPAddress(ipstr.str());
	}

        stack->setTCPIPSetManager(ipset_mng);
        stack->setTotalTCPFlows(1);
        pd->setStack(stack);

        pd->open("../pcapfiles/icq.pcapng");
        pd->run();
        pd->close();

        BOOST_CHECK(ipset_tcp->getTotalIPs() == 254);
        BOOST_CHECK(ipset_tcp->getTotalLookups() == 1);
        BOOST_CHECK(ipset_tcp->getTotalLookupsIn() == 0);
        BOOST_CHECK(ipset_tcp->getTotalLookupsOut() == 1);
}

#endif // 

BOOST_AUTO_TEST_CASE ( test_case_11 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());
        LearnerEnginePtr learner = LearnerEnginePtr(new LearnerEngine());
        std::vector<WeakPointer<Flow>> flow_list;

        stack->setTotalTCPFlows(4);
        stack->enableFrequencyEngine(true);
        pd->setStack(stack);
        pd->open("../pcapfiles/amazon_4ssl_flows.pcap");
        pd->run();
        pd->close();

        FrequencyGroup<std::string> group_by_port;

        group_by_port.setName("by destination port");
        group_by_port.agregateFlowsByDestinationPort(stack->getTCPFlowManager().lock());
        group_by_port.compute();

        BOOST_CHECK(group_by_port.getTotalProcessFlows() == 4);
        BOOST_CHECK(group_by_port.getTotalComputedFrequencies() == 1);

        flow_list = group_by_port.getReferenceFlowsByKey("443");

        // The flow_list should contains four entries
        BOOST_CHECK(flow_list.size() == 4);

        // pass the flows to the Learner engine
        learner->agregateFlows(flow_list);
        learner->compute();

	std::string header("^\\x16\\x03\\x01\\x00\\xe1\\x01\\x00\\x00\\xdd\\x03\\x03\\x53\\x17\\xc4\\x30.{28}\\x00\\x00");
        std::string reg(learner->getRegularExpression());

        BOOST_CHECK(header.compare(0,header.length(),reg,0,header.length())== 0);
}

// Test the statistics method by passing the protocol name
BOOST_AUTO_TEST_CASE ( test_case_12 )
{
        NetworkStackPtr stack = NetworkStackPtr(new StackLan());
	const char *tempfile = "/tmp/sometempfilefortest12.txt";

	stack->setStatisticsLevel(1);

	std::streambuf *psbuf, *backup;
  	std::ofstream filestr;
  	filestr.open (tempfile);

  	backup = std::cout.rdbuf();     // back up cout's streambuf
  	psbuf = filestr.rdbuf();        // get file's streambuf
  	std::cout.rdbuf(psbuf);         // assign streambuf to cout
	stack->statistics("EthernetProtocol");
  	std::cout.rdbuf(backup);        // restore cout's original streambuf
  	filestr.close();

	BOOST_CHECK(boost::filesystem::file_size(tempfile) > 100); // Ugly check but works

	boost::filesystem::remove(tempfile);

  	filestr.open (tempfile);
  	backup = std::cout.rdbuf();     // back up cout's streambuf
  	psbuf = filestr.rdbuf();        // get file's streambuf
  	std::cout.rdbuf(psbuf);         // assign streambuf to cout
	stack->statistics("EthernetNoExiste");
  	std::cout.rdbuf(backup);        // restore cout's original streambuf
  	filestr.close();

	BOOST_CHECK(boost::filesystem::file_size(tempfile) == 0); // Ugly check but works
	boost::filesystem::remove(tempfile);
}

BOOST_AUTO_TEST_CASE (test_case_13) // Test the UDP regex 
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        NetworkStackPtr stack = NetworkStackPtr(new StackLan());
        RegexManagerPtr rmng = RegexManagerPtr(new RegexManager());
        SharedPointer<Regex> r_generic = SharedPointer<Regex>(new Regex("Netbios","^.*FACACA.*$"));

        // connect with the stack
        pd->setStack(stack);

        stack->setTotalUDPFlows(2);
        stack->enableLinkLayerTagging("vlan");

	stack->enableNIDSEngine(true);

        rmng->addRegex(r_generic);
        stack->setUDPRegexManager(rmng);

        pd->open("../pcapfiles/flow_vlan_netbios.pcap");
        pd->run();
        pd->close();

	BOOST_CHECK(r_generic->getMatchs() == 1);
	BOOST_CHECK(r_generic->getTotalEvaluates() == 1);
}

// Test the release cache funcionality
BOOST_AUTO_TEST_CASE ( test_case_14 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());

        stack->setTotalTCPFlows(2);
        stack->setTotalUDPFlows(2);
        pd->setStack(stack);

        pd->open("../pcapfiles/accessgoogle.pcap");
        pd->run();
        pd->close();

        FlowManagerPtr flows_tcp = stack->getTCPFlowManager().lock();

        BOOST_CHECK(flows_tcp->getTotalFlows() == 1);
        for (auto &flow: flows_tcp->getFlowTable()) {
                BOOST_CHECK(flow->getHTTPInfo() != nullptr);
                BOOST_CHECK(flow->getHTTPInfo()->ua != nullptr);
                BOOST_CHECK(flow->getHTTPInfo()->uri != nullptr);
        }
        FlowManagerPtr flows_udp = stack->getUDPFlowManager().lock();

        BOOST_CHECK(flows_udp->getTotalFlows() == 1);
        for (auto &flow: flows_udp->getFlowTable()) {
                BOOST_CHECK(flow->getDNSInfo() != nullptr);
        }

	stack->releaseCaches();

        BOOST_CHECK(flows_tcp->getTotalFlows() == 1);
        for (auto &flow: flows_tcp->getFlowTable()) {
                BOOST_CHECK(flow->getHTTPInfo() == nullptr);
        }

        BOOST_CHECK(flows_udp->getTotalFlows() == 1);
        for (auto &flow: flows_udp->getFlowTable()) {
                BOOST_CHECK(flow->getDNSInfo() == nullptr);
        }
}

// Test the chain of regex with the RegexManager
// To understand the values of the test, open the smtp.pcap file
BOOST_AUTO_TEST_CASE ( test_case_15 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());
        RegexManagerPtr rmbase = RegexManagerPtr(new RegexManager());
        SharedPointer<RegexManager> rm1 = SharedPointer<RegexManager>(new RegexManager());
        SharedPointer<RegexManager> rm2 = SharedPointer<RegexManager>(new RegexManager());

        SharedPointer<Regex> r1 = SharedPointer<Regex>(new Regex("r1","^\x26\x01"));
        SharedPointer<Regex> r2 = SharedPointer<Regex>(new Regex("r2","^\x26\x01"));
        SharedPointer<Regex> r3 = SharedPointer<Regex>(new Regex("r3","^AUTH"));

	r3->setNextRegexManager(rm1);
        rmbase->addRegex(r1);
        rmbase->addRegex(r2);
        rmbase->addRegex(r3);

        SharedPointer<Regex> r4 = SharedPointer<Regex>(new Regex("r4","^\x26\x01"));
        SharedPointer<Regex> r5 = SharedPointer<Regex>(new Regex("r5","^MAIL FROM"));

	rm1->addRegex(r4);
	rm1->addRegex(r5);

	r5->setNextRegexManager(rm2);

        SharedPointer<Regex> r6 = SharedPointer<Regex>(new Regex("r6","^250 OK"));
        SharedPointer<Regex> r7 = SharedPointer<Regex>(new Regex("r7","^QUIT"));

	r6->setNextRegex(r7);

	rm2->addRegex(r6);	
	
        stack->setTCPRegexManager(rmbase);

        stack->setTotalTCPFlows(8);
        stack->setTotalUDPFlows(8);
        pd->setStack(stack);

	stack->enableNIDSEngine(true);

        pd->open("../pcapfiles/smtp.pcap");
        pd->run();
        pd->close();

	BOOST_CHECK(r1->getMatchs() == 0);
	BOOST_CHECK(r1->getTotalEvaluates() == 4);
	BOOST_CHECK(r2->getMatchs() == 0);
	BOOST_CHECK(r2->getTotalEvaluates() == 4);
	BOOST_CHECK(r3->getMatchs() == 1);
	BOOST_CHECK(r3->getTotalEvaluates() == 4);

	BOOST_CHECK(r4->getMatchs() == 0);
	BOOST_CHECK(r4->getTotalEvaluates() == 6);
	BOOST_CHECK(r5->getMatchs() == 1);
	BOOST_CHECK(r5->getTotalEvaluates() == 6);

	BOOST_CHECK(r6->getMatchs() == 1);
	BOOST_CHECK(r6->getTotalEvaluates() == 1);
	BOOST_CHECK(r7->getMatchs() == 1);
	BOOST_CHECK(r7->getTotalEvaluates() == 21);
	
        FlowManagerPtr flows_tcp = stack->getTCPFlowManager().lock();

        BOOST_CHECK(flows_tcp->getTotalFlows() == 1);
        for (auto &flow: flows_tcp->getFlowTable()) {
                BOOST_CHECK(flow->regex.lock() == r7);
                BOOST_CHECK(flow->regex_mng.lock() == rm2);
        }
}

// Test the functionality of the SSDPProtocol
BOOST_AUTO_TEST_CASE ( test_case_16 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackLanPtr stack = StackLanPtr(new StackLan());

        stack->setTotalUDPFlows(5);
        pd->setStack(stack);
        pd->open("../pcapfiles/ssdp_flow.pcap");
        pd->run();
        pd->close();

        FlowManagerPtr fm = stack->getUDPFlowManager().lock();

        bool called = false;
        // Check the relaseCache functionality 
        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->getSSDPInfo() != nullptr);
                called = true;
        }

        stack->releaseCaches();

        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->layer7info == nullptr);
                called = true;
        }
	BOOST_CHECK(called == true);
}

BOOST_AUTO_TEST_CASE ( test_case_17 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        NetworkStackPtr stack = NetworkStackPtr(new StackLan());

        pd->setStack(stack);

        stack->setTotalTCPFlows(10);
	stack->increaseAllocatedMemory("POPProtocol",1);

        pd->open("../pcapfiles/pop_flow.pcap");
        pd->run();
        pd->close();

	FlowManagerPtr fm = stack->getTCPFlowManager().lock();

       	std::filebuf fb;
       	fb.open ("/dev/null",std::ios::out);
       	std::ostream outp(&fb);

        bool called = false;
        // Check the relaseCache functionality 
        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->getPOPInfo() != nullptr);
		flow->serialize(outp);
		// Check some of the values ?
                called = true;
        }
	BOOST_CHECK(called == true);
	called = false;
        
	stack->releaseCaches();
        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->layer7info == nullptr);
                called = true;
        }
	BOOST_CHECK(called == true);
       	
	// For exercise the statistics
       	stack->setStatisticsLevel(5);
       	stack->statistics(outp);
      	stack->showFlows(outp); 
       	fb.close();
}

BOOST_AUTO_TEST_CASE ( test_case_18 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        NetworkStackPtr stack = NetworkStackPtr(new StackLan());

        pd->setStack(stack);

        stack->setTotalUDPFlows(10);
	stack->increaseAllocatedMemory("CoAPProtocol",10);

        pd->open("../pcapfiles/ipv4_coap.pcap");
        pd->run();
        pd->close();

	FlowManagerPtr fm = stack->getUDPFlowManager().lock();

       	std::filebuf fb;
       	fb.open ("/dev/null",std::ios::out);
       	std::ostream outp(&fb);

        bool called = false;
        // Check the relaseCache functionality 
        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->getCoAPInfo() != nullptr);
		flow->serialize(outp);
		// Check some of the values ?
                called = true;
	}
	BOOST_CHECK( called == true);
}

BOOST_AUTO_TEST_CASE ( test_case_19 ) 
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        NetworkStackPtr stack = NetworkStackPtr(new StackLan());

        pd->setStack(stack);

        stack->setTotalTCPFlows(10);
	stack->increaseAllocatedMemory("ModbusProtocol",5);

        pd->open("../pcapfiles/modbus_five_flows.pcap");
        pd->run();
        pd->close();

	FlowManagerPtr fm = stack->getTCPFlowManager().lock();

	// TODO
}

BOOST_AUTO_TEST_CASE ( test_case_20 ) 
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        NetworkStackPtr stack = NetworkStackPtr(new StackLan());

        pd->setStack(stack);

        stack->setTotalTCPFlows(10);
	stack->increaseAllocatedMemory("IMAPProtocol",1);

        pd->open("../pcapfiles/imap_flow.pcap");
        pd->run();
        pd->close();

	FlowManagerPtr fm = stack->getTCPFlowManager().lock();

       	std::filebuf fb;
       	fb.open ("/dev/null",std::ios::out);
       	std::ostream outp(&fb);

        bool called = false;
        // Check the relaseCache functionality 
        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->getIMAPInfo() != nullptr);
                BOOST_CHECK(flow->getPOPInfo() == nullptr);
                BOOST_CHECK(flow->getSMTPInfo() == nullptr);
		flow->serialize(outp);
		// Check some of the values ?
		called = true;
	}
	BOOST_CHECK (called == true);	
        
	stack->releaseCaches();

        called = false;
        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->getIMAPInfo() == nullptr);
		called = true;
	}
	BOOST_CHECK (called == true);	
}

BOOST_AUTO_TEST_SUITE_END( )

BOOST_AUTO_TEST_SUITE (test_suite_stack_mobile) // Test cases for real stacks StackMobile

BOOST_AUTO_TEST_CASE ( test_case_1 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        NetworkStackPtr stack = NetworkStackPtr(new StackMobile());

        pd->setStack(stack);

        stack->setTotalUDPFlows(10);
	stack->increaseAllocatedMemory("SIPProtocol",10);

        pd->open("../pcapfiles/gprs_sip_flow.pcap");
        pd->run();
        pd->close();

	FlowManagerPtr fm = stack->getUDPFlowManager().lock();

        bool called = false;
        // Check the relaseCache functionality 
        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->getSIPInfo() != nullptr);
                called = true;
        }
	BOOST_CHECK(called == true);
	called = false;

        stack->releaseCaches();
        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->getSIPInfo() == nullptr);
                called = true;
        }
	BOOST_CHECK(called == true);
	
	// For exercise the statistics
       	std::filebuf fb;
       	fb.open ("/dev/null",std::ios::out);
       	std::ostream outp(&fb);

       	stack->setStatisticsLevel(5);
       	stack->statistics(outp);
      	stack->showFlows(outp); 
       
       	fb.close();
}

BOOST_AUTO_TEST_SUITE_END( )

BOOST_AUTO_TEST_SUITE (test_suite_stack_lan6) // Test cases for real stacks StackIPv6

BOOST_AUTO_TEST_CASE (test_case_1) // Test the TCP regex with IPv6 extension headers
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        NetworkStackPtr stack = NetworkStackPtr(new StackLanIPv6());
        RegexManagerPtr rmng = RegexManagerPtr(new RegexManager());
        SharedPointer<Regex> r_generic = SharedPointer<Regex>(new Regex("Bad http","^GET /bad.html"));

        pd->setStack(stack);

        stack->setTotalTCPFlows(1);

	stack->enableNIDSEngine(true);

        rmng->addRegex(r_generic);
        stack->setTCPRegexManager(rmng);

        pd->open("../pcapfiles/ipv6_ah.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(r_generic->getMatchs() == 1);
        BOOST_CHECK(r_generic->getTotalEvaluates() == 1);
}

BOOST_AUTO_TEST_CASE (test_case_2) // Test the DomainNames with DNS traffic
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        NetworkStackPtr stack = NetworkStackPtr(new StackLanIPv6());
	DomainNameManagerPtr dom = DomainNameManagerPtr(new DomainNameManager());
	DomainNamePtr d1 = DomainNamePtr(new DomainName("domain1","itojun.org"));
	DomainNamePtr d2 = DomainNamePtr(new DomainName("domain2","yahoo.com"));

	dom->addDomainName(d1);	
	dom->addDomainName(d2);	

        pd->setStack(stack);

        stack->setTotalTCPFlows(0);
        stack->setTotalUDPFlows(10);
	stack->increaseAllocatedMemory("DNSProtocol",10);

	stack->setDomainNameManager(*dom.get(),"DNSProtocol");

        pd->open("../pcapfiles/ipv6_mix_traffic.pcap");
        pd->setPcapFilter("port 53");
        pd->run();
        pd->close();

	BOOST_CHECK( d1->getMatchs() == 3);
	BOOST_CHECK( d2->getMatchs() == 1);
        
	FlowManagerPtr fm = stack->getUDPFlowManager().lock();

       	std::filebuf fb;
       	fb.open ("/dev/null",std::ios::out);
       	std::ostream outp(&fb);

        bool called = false;
        // Check the relaseCache functionality 
        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->getDNSInfo() != nullptr);

		// Execute the code for serialize the flows
		flow->serialize(outp);
                called = true;
        }
	BOOST_CHECK(called == true);
	called = false;

        stack->releaseCaches();

        for (auto &flow: fm->getFlowTable()) {
                BOOST_CHECK(flow->layer7info == nullptr);
                called = true;
        }
	BOOST_CHECK(called == true);

       	// For exercise the statistics
       	stack->setStatisticsLevel(5);
       	stack->statistics(outp);
      	stack->showFlows(outp); 
       
       	fb.close();
}

BOOST_AUTO_TEST_SUITE_END( )

BOOST_AUTO_TEST_SUITE (test_suite_stack_virtual) // Test cases for real stacks StackVirtual

BOOST_AUTO_TEST_CASE (test_case_1)
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        NetworkStackPtr stack = NetworkStackPtr(new StackVirtual());
        RegexManagerPtr rmng = RegexManagerPtr(new RegexManager());
        SharedPointer<Regex> r_generic = SharedPointer<Regex>(new Regex("Bin directory","^bin$"));

        pd->setStack(stack);

        stack->setTotalUDPFlows(32);
        stack->setTotalTCPFlows(1);

        rmng->addRegex(r_generic);
        stack->setTCPRegexManager(rmng);

        pd->open("../pcapfiles/gre_ssh.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(r_generic->getMatchs() == 0);
        BOOST_CHECK(r_generic->getTotalEvaluates() == 75);
}

BOOST_AUTO_TEST_CASE (test_case_2)
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        NetworkStackPtr stack = NetworkStackPtr(new StackVirtual());
        RegexManagerPtr rmng = RegexManagerPtr(new RegexManager());
        SharedPointer<Regex> r_generic = SharedPointer<Regex>(new Regex("Bin directory","^bin$"));

        pd->setStack(stack);

        stack->setTotalUDPFlows(32);
        stack->setTotalTCPFlows(1);

        rmng->addRegex(r_generic);
        stack->setTCPRegexManager(rmng);

        pd->open("../pcapfiles/vxlan_ftp.pcap");
        pd->run();
        pd->close();

        BOOST_CHECK(r_generic->getMatchs() == 1);
        BOOST_CHECK(r_generic->getTotalEvaluates() == 1);

       	// For exercise the statistics
       	std::filebuf fb;
       	fb.open ("/dev/null",std::ios::out);
       	std::ostream outp(&fb);

       	stack->setStatisticsLevel(5);
       	stack->statistics(outp);
      	stack->showFlows(outp); 
       
       	fb.close();
}

BOOST_AUTO_TEST_SUITE_END( )

BOOST_AUTO_TEST_SUITE (test_suite_stack_openflow) // Test cases for real stacks StackOpenFlow

BOOST_AUTO_TEST_CASE ( test_case_1 )
{
        PacketDispatcherPtr pd = PacketDispatcherPtr(new PacketDispatcher());
        StackOpenFlowPtr stack = StackOpenFlowPtr(new StackOpenFlow());
        RegexManagerPtr re = RegexManagerPtr(new RegexManager());
        SharedPointer<Regex> r = SharedPointer<Regex>(new Regex("a signature","^\x26\x01"));
        SharedPointer<IPSet> ipset_tcp = SharedPointer<IPSet>(new IPSet("IPSet 1"));
        SharedPointer<IPSetManager> ipset_mng = SharedPointer<IPSetManager>(new IPSetManager());

        ipset_mng->addIPSet(ipset_tcp);
        ipset_tcp->addIPAddress("192.168.2.14");

        stack->setTCPIPSetManager(ipset_mng);

        re->addRegex(r);
        stack->setTCPRegexManager(re);

        stack->setTotalTCPFlows(8);
        stack->setTotalUDPFlows(8);
        pd->setStack(stack);

        pd->open("../pcapfiles/openflow.pcap");
        pd->run();
        pd->close();

        FlowManagerPtr flows_tcp = stack->getTCPFlowManager().lock();

        BOOST_CHECK(flows_tcp->getTotalFlows() == 1);
        for (auto &flow: flows_tcp->getFlowTable()) {
                BOOST_CHECK(flow->regex.lock() == r);
                BOOST_CHECK(flow->ipset.lock() == ipset_tcp);
        }

        FlowManagerPtr flows_udp = stack->getUDPFlowManager().lock();
        BOOST_CHECK(flows_udp->getTotalFlows() == 1);
        for (auto &flow: flows_udp->getFlowTable()) {
                BOOST_CHECK(flow->getPacketAnomaly() == PacketAnomalyType::UDP_BOGUS_HEADER);
        }

       	// For exercise the statistics
       	std::filebuf fb;
       	fb.open ("/dev/null",std::ios::out);
       	//fb.open ("/tmp/test.txt",std::ios::out);
       	std::ostream outp(&fb);

       	stack->setStatisticsLevel(5);
       	stack->statistics(outp);
      	stack->showFlows(outp); 
       
       	fb.close();
}

BOOST_AUTO_TEST_SUITE_END( )

