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
#pragma GCC diagnostic ignored "-Wwrite-strings"
#ifndef SRC_PROTOCOLS_UDP_UDPPROTOCOL_H_
#define SRC_PROTOCOLS_UDP_UDPPROTOCOL_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Multiplexer.h"
#include "Protocol.h"
#include <netinet/udp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "flow/FlowManager.h"
#include "flow/FlowCache.h"
#include "FlowForwarder.h"
#include "DatabaseAdaptor.h"

namespace aiengine {

class UDPProtocol: public Protocol 
{
public:
    	explicit UDPProtocol(const std::string& name,const std::string& short_name):
		Protocol(name,short_name),
		stats_level_(0),
		flow_table_(),flow_cache_(),sigs_(),
		udp_header_(nullptr),current_flow_(nullptr),total_bytes_(0),
		last_timeout_(0),packet_time_(0) {

		addRejectFunction(std::bind(&UDPProtocol::default_reject_function,this,std::placeholders::_1));
	}
    	
	explicit UDPProtocol():UDPProtocol(UDPProtocol::default_name,UDPProtocol::default_short_name) {}

    	virtual ~UDPProtocol() {}

        static constexpr char *default_name = "UDPProtocol";
        static constexpr char *default_short_name = "udp";
	static const uint16_t id = IPPROTO_UDP;
	static const int header_size = 8;
	int getHeaderSize() const { return header_size;}

	int64_t getTotalBytes() const { return total_bytes_; }
	int64_t getTotalPackets() const { return total_packets_;}
	int64_t getTotalValidatedPackets() const { return total_validated_packets_;}
	int64_t getTotalMalformedPackets() const { return total_malformed_packets_;}

	void processFlow(Flow *flow) {} // This protocol generates flows but not for destination.
	bool processPacket(Packet& packet);

	void setStatisticsLevel(int level) { stats_level_ = level;}
	void statistics(std::basic_ostream<char>& out);
	void statistics() { statistics(std::cout);}

        void releaseCache() {} // No need to free cache

        void setHeader(unsigned char *raw_packet) {
        
                udp_header_ = reinterpret_cast <struct udphdr*> (raw_packet);
        }

	// Condition for say that a packet is UDP 
	bool udpChecker(Packet &packet){ 
	
                int length = packet.getLength();

		if(length >= header_size) {
			setHeader(packet.getPayload());
			++total_validated_packets_; 
			return true;
		} else {
			++total_malformed_packets_;
			return false;
		}
	}

#if defined(__FREEBSD__) || defined(__OPENBSD__) || defined(__DARWIN__)
	uint16_t getSourcePort() const { return ntohs(udp_header_->uh_sport); }
    	uint16_t getDestinationPort() const { return ntohs(udp_header_->uh_dport); }
    	uint16_t getLength() const { return ntohs(udp_header_->uh_ulen); }
    	unsigned int getPayloadLength() const { return ntohs(udp_header_->uh_ulen) - sizeof(struct udphdr); }
#else
	uint16_t getSourcePort() const { return ntohs(udp_header_->source); }
    	uint16_t getDestinationPort() const { return ntohs(udp_header_->dest); }
    	uint16_t getLength() const { return ntohs(udp_header_->len); }
    	unsigned int getPayloadLength() const { return ntohs(udp_header_->len) - sizeof(udphdr); }
#endif
    	unsigned int getHeaderLength() const { return sizeof(struct udphdr); }
	unsigned char* getPayload() const { return (unsigned char*)udp_header_ +getHeaderLength(); }

	void setFlowManager(FlowManagerPtr flow_mng) { flow_table_ = flow_mng;}
	FlowManagerPtr getFlowManager() { return flow_table_; }
	void setFlowCache(FlowCachePtr flow_cache) { flow_cache_ = flow_cache;}
	FlowCachePtr getFlowCache() { return flow_cache_;}

	void addRejectFunction(std::function <void (Flow*)> reject) { reject_func_ = reject; }

	void setRegexManager(const SharedPointer<RegexManager>& sig) { sigs_ = sig;}

	Flow *getCurrentFlow() { return current_flow_;} // used just for testing pourposes

	int64_t getAllocatedMemory() const;

#if defined(PYTHON_BINDING)
        boost::python::dict getCounters() const;
#elif defined(RUBY_BINDING)
        VALUE getCounters() const;
#elif defined(JAVA_BINDING)
        JavaCounters getCounters() const  { JavaCounters counters; return counters; }
#elif defined(LUA_BINDING)
        LuaCounters getCounters() const  { LuaCounters counters; return counters; }
#endif

	void setAnomalyManager(SharedPointer<AnomalyManager> amng) { anomaly_ = amng; }

private:
	SharedPointer<Flow> getFlow(const Packet& packet); 
	void default_reject_function(Flow *flow) {}

	int stats_level_;	
	FlowManagerPtr flow_table_;
	FlowCachePtr flow_cache_;
	SharedPointer<RegexManager> sigs_;
	struct udphdr *udp_header_;
	Flow *current_flow_;
	int64_t total_bytes_;
	time_t last_timeout_;
	time_t packet_time_;

	std::function <void (Flow*)> reject_func_;
	SharedPointer<AnomalyManager> anomaly_;
};

typedef std::shared_ptr<UDPProtocol> UDPProtocolPtr;

} // namespace aiengine

#endif  // SRC_PROTOCOLS_UDP_UDPPROTOCOL_H_
