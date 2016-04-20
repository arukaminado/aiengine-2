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
#ifndef SRC_PROTOCOLS_VLAN_VLANPROTOCOL_H_
#define SRC_PROTOCOLS_VLAN_VLANPROTOCOL_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Protocol.h"
#include <arpa/inet.h>

namespace aiengine {

struct vlan_tag {
        uint16_t       vlan_tpid;              /* ETH_P_8021Q/ETHERTYPE_VLAN */
        uint16_t       vlan_tci;               /* VLAN TCI */
} __attribute__((packed));

#define VLAN_TAG_LEN    4

class VLanProtocol: public Protocol 
{
public:
    	explicit VLanProtocol():
		Protocol("VLanProtocol","vlan"),
		stats_level_(0),
		vlan_header_(nullptr),total_bytes_(0) {}

    	virtual ~VLanProtocol() {}

	static const uint16_t id = ETHERTYPE_VLAN;	
	static const int header_size = VLAN_TAG_LEN;

	int getHeaderSize() const { return header_size;}

	int64_t getTotalBytes() const { return total_bytes_;}
	int64_t getTotalPackets() const { return total_packets_;}
	int64_t getTotalValidatedPackets() const { return total_validated_packets_;}
	int64_t getTotalMalformedPackets() const { return total_malformed_packets_;}

       	void processFlow(Flow *flow) {} // This protocol dont generate any flow 
	bool processPacket(Packet &packet);

	void setStatisticsLevel(int level) { stats_level_ = level;}
	void statistics(std::basic_ostream<char>& out);
	void statistics() { statistics(std::cout);}

	void releaseCache() {} // No need to free cache

	void setHeader(unsigned char *raw_packet){ 

		vlan_header_ = reinterpret_cast <struct vlan_tag*> (raw_packet);
	}

	// Condition for say that a packet its vlan 802.1q 
	bool vlanChecker(Packet &packet){ 
	
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

	uint16_t getEthernetType() const { return ntohs(vlan_header_->vlan_tci);}
	uint16_t getVlanId() const { return (ntohs(vlan_header_->vlan_tpid) & 0x0FFF);} 
	int64_t getAllocatedMemory() const { return sizeof(VLanProtocol); }

#if defined(PYTHON_BINDING)
        boost::python::dict getCounters() const;
#elif defined(RUBY_BINDING)
        VALUE getCounters() const;
#elif defined(JAVA_BINDING)
        JavaCounters getCounters() const  { JavaCounters counters; return counters; }
#endif

private:
	int stats_level_;
	struct vlan_tag *vlan_header_;
	int64_t total_bytes_;
};

typedef std::shared_ptr<VLanProtocol> VLanProtocolPtr;

} // namespace aiengine

#endif  // SRC_PROTOCOLS_VLAN_VLANPROTOCOL_H_
