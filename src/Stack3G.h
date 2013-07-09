#ifndef _Stack3G_H_
#define _Stack3G_H_

#include <string>
#include "Multiplexer.h"
#include "FlowForwarder.h"
#include "./ethernet/EthernetProtocol.h"
#include "./ip/IPProtocol.h"
#include "./udp/UDPProtocol.h"
#include "./tcp/TCPProtocol.h"
#include "./icmp/ICMPProtocol.h"
#include "./http/HTTPProtocol.h"
#include "./ssl/SSLProtocol.h"
#include "./flow/FlowManager.h"
#include "./flow/FlowCache.h"
#include "NetworkStack.h"

class Stack3G: public NetworkStack
{
public:
	explicit Stack3G();
        virtual ~Stack3G() {};

        const char* getName() { return name_.c_str();}; 
        void setName(char *name) { name_ = name;};

        void setLinkLayerMultiplexer(MultiplexerPtrWeak mux) { };
        MultiplexerPtrWeak getLinkLayerMultiplexer() { return mux_eth_;};
	
	void statistics(std::basic_ostream<char>& out);
	void statistics() { statistics(std::cout);};

	void printFlows(std::basic_ostream<char>& out);
	void printFlows() { printFlows(std::cout);};

	void setTotalTCPFlows(int value) { flow_cache_tcp_->createFlows(value);};
	void setTotalUDPFlows(int value) { flow_cache_udp_->createFlows(value);};

private:
	std::string name_;
        //Protocols
        EthernetProtocolPtr eth_;
        IPProtocolPtr ip_;
        UDPProtocolPtr udp_;
        TCPProtocolPtr tcp_;
        ICMPProtocolPtr icmp_;
        HTTPProtocolPtr http_;
        SSLProtocolPtr ssl_;

        // Multiplexers
        MultiplexerPtr mux_eth_;
        MultiplexerPtr mux_ip_;
        MultiplexerPtr mux_udp_;
        MultiplexerPtr mux_tcp_;
        MultiplexerPtr mux_icmp_;

        // FlowManager and FlowCache
        FlowManagerPtr flow_table_udp_;
        FlowManagerPtr flow_table_tcp_;
        FlowCachePtr flow_cache_udp_;
        FlowCachePtr flow_cache_tcp_;

        // FlowForwarders
        FlowForwarderPtr ff_tcp_;
        FlowForwarderPtr ff_udp_;
        FlowForwarderPtr ff_http_;
        FlowForwarderPtr ff_ssl_;

};


#endif
