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
#include "Flow.h"
#include "Protocol.h"

namespace aiengine {

void Flow::setFiveTuple(uint32_t src_a,uint16_t src_p,uint16_t proto,uint32_t dst_a,uint16_t dst_p) {
	
	address_.setSourceAddress(src_a);
	address_.setDestinationAddress(dst_a);
	source_port_ = src_p;
	dest_port_ = dst_p;
	protocol_ = proto;
}

void Flow::setFiveTuple6(struct in6_addr *src_a,uint16_t src_p,uint16_t proto,struct in6_addr *dst_a,uint16_t dst_p) {

        address_.setSourceAddress6(src_a);
        address_.setDestinationAddress6(dst_a);
        source_port_ = src_p;
        dest_port_ = dst_p;
        protocol_ = proto;
}

void Flow::reset() {

	hash_ = 0;
	total_bytes = 0;
	total_packets = 0;
	total_packets_l7 = 0;
	address_.reset();
	source_port_ = 0;
	dest_port_ = 0;
	protocol_ = 0;
	have_tag_ = false;
	reject_ = false;
	partial_reject_ = false;
	have_evidence_ = false;
	tag_ = 0xffffffff;	
	ipset.reset();	
	forwarder.reset();
	frequencies.reset();
	http_info.reset();
	ssl_info.reset();
	sip_info.reset();
	smtp_info.reset();
	imap_info.reset();
	pop_info.reset();
	ssdp_info.reset();
	regex.reset();
	dns_info.reset();
	tcp_info.reset();
	gprs_info.reset();
	regex_mng.reset();
	packet = nullptr;
	frequency_engine_inspected = false;
	prev_direction_ = direction_ = FlowDirection::FORWARD;
	pa_ = PacketAnomalyType::NONE;
	arrive_time_ = 0;
	current_time_ = 0;
}

void Flow::serialize(std::ostream& stream) {

#ifdef HAVE_FLOW_SERIALIZATION_COMPRESSION 

	// In order to optimize the data transfer with databases/files
	// We decide to remove key words and substitute by simple bytes
	// that allow to reduce the data transfer, similar as msgpack does
	// Here is the meaning
	// 	5tuple: The tuple connection
	// 	b:	Number of bytes transfer
	// 	i:	IPSet name associated to the flow
	//	a:	Anomaly name associated to the flow
	//	p:	Short layer7 protocol name
	//	t:	TCPInfo (flags,QoS)
	//	h:	Hostname of the HTTP flow if is HTTP 
	//	s:	Hostname of the client hello in SSL
	//	d:	DNSname
	//	g:	GPRS information if StackMobile is running
	//	r:	Regex matched on the flow
		
        stream << "{";
        stream << "\"5tuple\":\"" << address_.getSrcAddrDotNotation() << ":";
        stream << source_port_ << ":";
        stream << protocol_ << ":";
        stream << address_.getDstAddrDotNotation() << ":";
        stream << dest_port_ << "\",";

	stream << "\"b\":" << total_bytes; 

	if (!ipset.expired()) 
                stream << ",\"i\":\"" << ipset.lock()->getName() << "\"";
	
	if(pa_ != PacketAnomalyType::NONE)
		stream << ",\"a\":\"" << AnomalyManager::getInstance()->getName(pa_) << "\"";

	// The protocol name are like HTTPProtocol, SMTPProtocol, SSLProtocol and so on
	// So for reduce the number of bytes transmited we remove the word Protocol.
	boost::string_ref sname(getL7ProtocolName());

	if (sname.length() > 4) { 
		boost::string_ref pname(sname.substr(0,sname.length()-8));

		stream << ",\"p\":\"" << pname << "\"";
	} else {
		stream << ",\"p\":\"" << sname << "\"";
	}
        if (protocol_ == IPPROTO_TCP) {
                if(tcp_info)
                        stream << ",\"t\":\"" << *tcp_info.get() << "\"";

		if (http_info) {
			if (http_info->host)	
                        	stream << ",\"h\":\"" << http_info->host->getName() << "\"";
		} else {
                	if (ssl_info) {
				if (ssl_info->host)
                        		stream << ",\"s\":\"" << ssl_info->host->getName() << "\"";
			}
		}
        } else { // UDP
                if(dns_info) {
			if (dns_info->name)
                        	stream << ",\"d\":\"" << dns_info->name->getName() << "\"";
              	} 
		if(gprs_info)
                        stream << ",\"g\":\"" << gprs_info->getIMSIString() << "\"";
        }
        if(!regex.expired())
                stream << ",\"r\":\"" << regex.lock()->getName() << "\"";
	
	stream << "}";

#else

	stream << "{";
	stream << "\"ipsrc\":\"" << address_.getSrcAddrDotNotation() << "\",";
	stream << "\"portsrc\":" << source_port_ << ",";
	stream << "\"proto\":" << protocol_ << ",";
	stream << "\"ipdst\":\"" << address_.getDstAddrDotNotation() << "\",";
	stream << "\"portdst\":" << dest_port_ << ",";

	stream << "\"bytes\":" << total_bytes; 

	if (!ipset.expired())
		stream << ",\"ipset\":\"" << ipset.lock()->getName() << "\"";

	if (pa_ != PacketAnomalyType::NONE)
		stream << ",\"anomaly\":\"" << AnomalyManager::getInstance()->getName(pa_) << "\"";

	stream << ",\"layer7\":\"" << getL7ProtocolName() << "\"";

	if (protocol_ == IPPROTO_TCP) {
		if (tcp_info)	
			stream << ",\"tcpflags\":\"" << *tcp_info.get() << "\"";
	
		if (http_info) {
			if (http_info->host)	
				stream << ",\"httphost\":\"" << http_info->host->getName() << "\"";
		} else {	
			if (ssl_info){
				stream << ",\"sslhost\":\"" << ssl_info->host->getName() << "\"";
			}
		}
	} else { // UDP
		if (dns_info) {
			if (dns_info->name) 	
				stream << ",\"dnsdomain\":\"" << dns_info->name->getName() << "\"";
		}
		if (gprs_info)	
			stream << ",\"imsi\":\"" << gprs_info->getIMSIString() << "\"";
	}
	if (!regex.expired())	
		stream << ",\"matchs\":\"" << regex.lock()->getName() << "\"";
	
	stream << "}";
#endif
}

void Flow::showFlowInfo(std::ostream& out) const {

	if (haveTag() == true) {
        	out << " Tag:" << getTag();
        }

        if (getPacketAnomaly() != PacketAnomalyType::NONE)
		out << " Anomaly:" << AnomalyManager::getInstance()->getName(pa_);

        if (ipset.lock()) out << " IPset:" << ipset.lock()->getName();

	if (protocol_ == IPPROTO_TCP) {
		if (tcp_info) out << " TCP:" << *tcp_info.get();

		if (http_info) {
                	out << " Req(" << http_info->getTotalRequests() << ")Res(" << http_info->getTotalResponses() << ")Code(" << http_info->getResponseCode() << ") ";
                	if (http_info->getIsBanned()) out << "Banned ";
                	if (http_info->host) out << "Host:" << http_info->host->getName();
                	if (http_info->ua) out << " UserAgent:" << http_info->ua->getName();
        	} else {
			if (ssl_info) {
				out << " Pdus:" << ssl_info->getTotalDataPdus();
				if (ssl_info->host) out << " Host:" << ssl_info->host->getName();
			} else {
				if (smtp_info) {
					if (smtp_info->from) out << " From:" << smtp_info->from->getName();
					if (smtp_info->to) out << " To:" << smtp_info->to->getName();
				} else {
					if (pop_info) {
						if (pop_info->user_name) out << " User:" << pop_info->user_name->getName();
					} else {
						if (imap_info) {
							if (imap_info->user_name) out << " User:" << imap_info->user_name->getName();
						}
					}
				} 
			} 
		} 
	} else {
		if (gprs_info) {
			out << " GPRS:" << *gprs_info.get();
		} 

		if (dns_info) {
			if (dns_info->name) out << " Domain:" << dns_info->name->getName();	
		} else {
			if (sip_info) {
                		if (sip_info->uri) out << " SIPUri:" << sip_info->uri->getName();
                		if (sip_info->from) out << " SIPFrom:" << sip_info->from->getName();
                		if (sip_info->to) out << " SIPTo:" << sip_info->to->getName();
                		if (sip_info->via) out << " SIPVia:" << sip_info->via->getName();
			}
        	}
	}

        if (!regex.expired()) out << " Regex:" << regex.lock()->getName();

	if (isPartialReject()) out << " Rejected";

	if (frequencies) {
		out << " Dispersion(" << frequencies->getDispersion() << ")";
		out << "Enthropy(" << std::setprecision(4) << frequencies->getEnthropy() << ") ";
		out << boost::format("%-8s") % frequencies->getFrequenciesString();
	}
	return;
}

const char* Flow::getL7ProtocolName() const {

	const char *proto_name = "None";

        if (forwarder.lock()) {
        	ProtocolPtr proto = forwarder.lock()->getProtocol();
                if (proto) proto_name = proto->getName();
	}
        return proto_name;
}

} // namespace aiengine 

