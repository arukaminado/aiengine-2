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
#include "IPv6Protocol.h"
#include <iomanip> // setw

namespace aiengine {

char* IPv6Protocol::getSrcAddrDotNotation() const {

	static char straddr_src[INET6_ADDRSTRLEN];

	inet_ntop(AF_INET6,(struct in6_addr*)&(ip6_header_->ip6_src),straddr_src,INET6_ADDRSTRLEN);

	return straddr_src;
}

char* IPv6Protocol::getDstAddrDotNotation() const {

        static char straddr_dst[INET6_ADDRSTRLEN];

        inet_ntop(AF_INET6,&ip6_header_->ip6_dst,straddr_dst,INET6_ADDRSTRLEN);

        return straddr_dst;
}

bool IPv6Protocol::processPacket(Packet& packet) {

        MultiplexerPtr mux = mux_.lock();
	uint16_t next_proto = getProtocol();
	int extension_length = 0;
	bool have_extension_hdr = false;
	int iter = 0;
        int bytes = packet.getLength();

        ++total_packets_;

        mux->total_length = bytes;
        total_bytes_ += bytes; 

	mux->address.setSourceAddress6(getSourceAddress());
	mux->address.setDestinationAddress6(getDestinationAddress());

	unsigned char *ipv6pkt = getPayload();

	do {
		++iter;
		// I dont like switch statements but sometimes.....
		switch (next_proto) {
			case IPPROTO_DSTOPTS:
			case IPPROTO_ROUTING:
			case IPPROTO_HOPOPTS: {
				ipv6pkt = &ipv6pkt[extension_length];

				struct ip6_ext *ip6_generic_ext = reinterpret_cast <struct ip6_ext*> (ipv6pkt); 

				next_proto = ip6_generic_ext->ip6e_nxt;
				extension_length += (ip6_generic_ext->ip6e_len + 1) * 8;  /* length in units of 8 octets.  */

				if (have_extension_hdr) {
					packet.setPacketAnomaly(PacketAnomalyType::IPV6_LOOP_EXTENSION_HEADERS);
					anomaly_->incAnomaly(PacketAnomalyType::IPV6_LOOP_EXTENSION_HEADERS);
				}
				++total_extension_header_packets_;
				have_extension_hdr = true;
				break;
			}
			case IPPROTO_AH: {
				struct ip6_ext *ip6_generic_ext = reinterpret_cast <struct ip6_ext*> (ipv6pkt); 

				next_proto = ip6_generic_ext->ip6e_nxt;
				extension_length = (ip6_generic_ext->ip6e_len) * 6;  /* length in units of 6 octets.  */
				
				if (have_extension_hdr) {
					packet.setPacketAnomaly(PacketAnomalyType::IPV6_LOOP_EXTENSION_HEADERS);
					anomaly_->incAnomaly(PacketAnomalyType::IPV6_LOOP_EXTENSION_HEADERS);
				}
				
				++total_extension_header_packets_;
				have_extension_hdr = true;
				break;
			}
			case IPPROTO_UDP: 
			case IPPROTO_TCP:
			case IPPROTO_ICMPV6: {

                                packet.net_packet.setPayload(packet.getPayload());
                                packet.net_packet.setLength(bytes);

        			mux->setHeaderSize(header_size + extension_length);
       				mux->setNextProtocolIdentifier(next_proto);
       				packet.setPrevHeaderSize(header_size + extension_length);
				return true;
			} 
			case IPPROTO_FRAGMENT: 
				++total_frag_packets_;
				packet.setPacketAnomaly(PacketAnomalyType::IPV6_FRAGMENTATION);
				anomaly_->incAnomaly(PacketAnomalyType::IPV6_FRAGMENTATION);
				return false; // The packet can not progress through the stack
			case IPPROTO_NONE:
				++total_no_header_packets_;
				return false;	
			default:
				++total_other_extension_header_packets_;
				break;
		} 
	} while ( iter < 3);

	// Update the mux but the packet is not gonna be forwarder
       	mux->setHeaderSize(header_size + extension_length);
       	mux->setNextProtocolIdentifier(0);
	return false;
}

void IPv6Protocol::statistics(std::basic_ostream<char>& out) {

        if (stats_level_ > 0) {
                int alloc_memory = getAllocatedMemory();
                std::string unit = "Bytes";

                unitConverter(alloc_memory,unit);

                out << getName() << "(" << this <<") statistics" << std::dec << std::endl;
                out << "\t" << "Total allocated:        " << std::setw(9 - unit.length()) << alloc_memory << " " << unit <<std::endl;
                out << "\t" << "Total packets:          " << std::setw(10) << total_packets_ <<std::endl;
                out << "\t" << "Total bytes:        " << std::setw(14) << total_bytes_ <<std::endl;
                if (stats_level_ > 1) {
                        out << "\t" << "Total validated packets:" << std::setw(10) << total_validated_packets_ <<std::endl;
                        out << "\t" << "Total malformed packets:" << std::setw(10) << total_malformed_packets_ <<std::endl;
                        if(stats_level_ > 3) {
                                out << "\t" << "Total fragment packets: " << std::setw(10) << total_frag_packets_ <<std::endl;
                                out << "\t" << "Total no hdr packets:   " << std::setw(10) << total_no_header_packets_ <<std::endl;
                                out << "\t" << "Total extension packets:" << std::setw(10) << total_extension_header_packets_ <<std::endl;
                                out << "\t" << "Total other ext packets:" << std::setw(10) << total_other_extension_header_packets_ <<std::endl;
                        }

                        if (stats_level_ > 2) {
                                if(mux_.lock())
                                        mux_.lock()->statistics(out);
                        }
                }
        }
}

#if defined(PYTHON_BINDING) || defined(RUBY_BINDING)
#if defined(PYTHON_BINDING)
boost::python::dict IPv6Protocol::getCounters() const {
        boost::python::dict counters;
#elif defined(RUBY_BINDING)
VALUE IPv6Protocol::getCounters() const {
        VALUE counters = rb_hash_new();
#endif
        addValueToCounter(counters,"packets", total_packets_);
        addValueToCounter(counters,"bytes", total_bytes_);
        addValueToCounter(counters,"fragmented packets", total_frag_packets_);
	addValueToCounter(counters,"extension header packets", total_extension_header_packets_);

        return counters;
}

#endif

} // namespace aiengine
