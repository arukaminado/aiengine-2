/*
 * AIEngine a deep packet inspector reverse engineering engine.
 *
 * Copyright (C) 2013  Luis Campo Giralte
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
 * Written by Luis Campo Giralte <luis.camp0.2009@gmail.com> 2013
 *
 */
#ifndef SRC_IP_IPPROTOCOL_H_
#define SRC_IP_IPPROTOCOL_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../Multiplexer.h"
#include "../Protocol.h"
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

namespace aiengine {

class IPProtocol: public Protocol 
{
public:
    	explicit IPProtocol():ip_header_(nullptr),total_bytes_(0),
		total_frag_packets_(0),stats_level_(0) { name_="IPProtocol";}
    	virtual ~IPProtocol() {}
	
	static const u_int16_t id = ETHERTYPE_IP;
	static const int header_size = 20;
	int getHeaderSize() const { return header_size;}

	int64_t getTotalBytes() const { return total_bytes_;}
	int64_t getTotalPackets() const { return total_packets_;}
	int64_t getTotalValidatedPackets() const { return total_validated_packets_;}
	int64_t getTotalMalformedPackets() const { return total_malformed_packets_;}

        const char *getName() { return name_.c_str();}

       	void processFlow(Flow *flow); 
	void processPacket(Packet& packet);

	void setStatisticsLevel(int level) { stats_level_ = level;}
	void statistics(std::basic_ostream<char>& out);
	void statistics() { statistics(std::cout);}

        void setFlowForwarder(FlowForwarderPtrWeak ff) {}
        FlowForwarderPtrWeak getFlowForwarder() { FlowForwarderPtrWeak ptr; return ptr; }

        void setMultiplexer(MultiplexerPtrWeak mux) { mux_ = mux; }
        MultiplexerPtrWeak getMultiplexer() { return mux_;}

#ifdef PYTHON_BINDING
        void setDatabaseAdaptor(boost::python::object &dbptr) {} ;
#endif

        void setHeader(unsigned char *raw_packet) {
        
                ip_header_ = reinterpret_cast <struct ip*> (raw_packet);
        }

	// Condition for say that a packet is IP 
	bool ipChecker(Packet &packet) { 
	
		int length = packet.getLength();

		setHeader(packet.getPayload());
		if ((length >= header_size)&&(isIPver4())) {
			++total_validated_packets_;
			return true;
		} else {
			++total_malformed_packets_;
			return false;
		}
	}

	/* Fields from IP headers */
    	u_int8_t getTTL() const { return ip_header_->ip_ttl; }
    	u_int16_t getPacketLength() const { return ntohs(ip_header_->ip_len); }
    	u_int16_t getIPHeaderLength() const { return ip_header_->ip_hl * 4; }
    	bool isIP() const { return ip_header_ ? true : false ; }
    	bool isIPver4() const { return ip_header_->ip_v == 4; }
    	bool isFragment() const { return (ip_header_->ip_off & IP_MF); }
    	u_int16_t getID() const { return ntohs(ip_header_->ip_id); }
    	int getVersion() const { return ip_header_->ip_v; }
    	u_int16_t getProtocol () const { return ip_header_->ip_p; }
    	u_int32_t getSrcAddr() const { return ip_header_->ip_src.s_addr; }
    	u_int32_t getDstAddr() const { return ip_header_->ip_dst.s_addr; }
    	const char* getSrcAddrDotNotation() const { return inet_ntoa(ip_header_->ip_src); }
    	const char* getDstAddrDotNotation() const { return inet_ntoa(ip_header_->ip_dst); }
    	u_int32_t getIPPayloadLength() const { return getPacketLength() - getIPHeaderLength(); }

private:
	int stats_level_;
	MultiplexerPtrWeak mux_;
	struct ip *ip_header_;
	int64_t total_bytes_;
	int32_t total_frag_packets_;
};

typedef std::shared_ptr<IPProtocol> IPProtocolPtr;

} // namespace aiengine

#endif  // SRC_IP_IPPROTOCOL_H_
