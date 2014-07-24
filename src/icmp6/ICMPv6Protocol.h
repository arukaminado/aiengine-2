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
#ifndef SRC_ICMP6_ICMPV6PROTOCOL_H_
#define SRC_ICMP6_ICMPV6PROTOCOL_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../Multiplexer.h"
#include "../Protocol.h"
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace aiengine {

class ICMPv6Protocol: public Protocol 
{
public:
    	explicit ICMPv6Protocol():Protocol("ICMPv6Protocol"),stats_level_(0),
		mux_(),icmp_header_(nullptr) {}

    	virtual ~ICMPv6Protocol() {}

	static const u_int16_t id = IPPROTO_ICMP;
	static const int header_size = 8;

	int getHeaderSize() const { return header_size;}

	int64_t getTotalPackets() const { return total_packets_;}
	int64_t getTotalValidatedPackets() const { return total_validated_packets_;}
	int64_t getTotalMalformedPackets() const { return total_malformed_packets_;}

	void processFlow(Flow *flow) { /* No flow to manage */ } 
	void processPacket(Packet& packet);

	void setStatisticsLevel(int level) { stats_level_ = level;}
	void statistics(std::basic_ostream<char>& out);
	void statistics() { statistics(std::cout);}

        void setMultiplexer(MultiplexerPtrWeak mux) { mux_ = mux; }
        MultiplexerPtrWeak getMultiplexer() { return mux_;}

        void setFlowForwarder(FlowForwarderPtrWeak ff) {}
        FlowForwarderPtrWeak getFlowForwarder() { FlowForwarderPtrWeak ptr; return ptr; }

#ifdef PYTHON_BINDING
        void setDatabaseAdaptor(boost::python::object &dbptr) {} ;
#endif

        void setHeader(unsigned char *raw_packet) { 
       
#if defined(__FREEBSD__) || defined(__OPENBSD__) 
                icmp_header_ = reinterpret_cast <struct icmp*> (raw_packet);
#else
                icmp_header_ = reinterpret_cast <struct icmp6_hdr*> (raw_packet);
#endif
        }

	// Condition for say that a packet is icmp 
	bool icmp6Checker(Packet &packet) { 
	
                int length = packet.getLength();

                setHeader(packet.getPayload());

		if (length >= header_size) {
			++total_validated_packets_; 
			return true;
		} else {
			++total_malformed_packets_;
			return false;
		}
	}

#if defined(__FREEBSD__) || defined(__OPENBSD__) 
        u_int8_t getType() const { return icmp_header_->icmp_type; }
        u_int8_t getCode() const { return icmp_header_->icmp_code; }
        u_int16_t getId() const { return ntohs(icmp_header_->icmp_id); }
        u_int16_t getSequence() const { return ntohs(icmp_header_->icmp_seq); }
#else
        u_int8_t getType() const { return icmp_header_->icmp6_type; }
        u_int8_t getCode() const { return icmp_header_->icmp6_code; }
        u_int16_t getId() const { return ntohs(icmp_header_->icmp6_id); }
        u_int16_t getSequence() const { return ntohs(icmp_header_->icmp6_seq); }
#endif

private:
	int stats_level_;
	MultiplexerPtrWeak mux_;
#if defined(__FREEBSD__) || defined(__OPENBSD__) 
	struct icmp *icmp_header_;
#else
	struct icmp6_hdr *icmp_header_;
#endif 
};

typedef std::shared_ptr<ICMPv6Protocol> ICMPv6ProtocolPtr;

} // namespace aiengine

#endif  // SRC_ICMP6_ICMPV6PROTOCOL_H_