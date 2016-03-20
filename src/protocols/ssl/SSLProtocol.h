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
#ifndef SRC_PROTOCOLS_SSL_SSLPROTOCOL_H_
#define SRC_PROTOCOLS_SSL_SSLPROTOCOL_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_LIBLOG4CXX
#include "log4cxx/logger.h"
#endif
#include "Protocol.h"
#include "SSLInfo.h"
#include "Cache.h"
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <boost/utility/string_ref.hpp> 
#include "flow/FlowManager.h"

namespace aiengine {

// Minium SSL header
typedef struct {
	uint8_t type; 		// SSL record type 
	uint16_t version; 	// SSL version (major/minor)
	uint16_t length; 	// Length of data in the record (excluding the header itself), The maximum SSL supports is 16384 (16K). 
	u_char data[0];		// 
} __attribute__((packed)) ssl_record;

// The only supported versions
#define SSL3_VERSION 0x0300
#define TLS1_VERSION 0x0301
#define TLS1_1_VERSION 0x0302
#define TLS1_2_VERSION 0x0303

// Record types of the ssl_handshake_record

#define SSL3_MT_HELLO_REQUEST            0   //(x'00')
#define SSL3_MT_CLIENT_HELLO             1   //(x'01')
#define SSL3_MT_SERVER_HELLO             2   //(x'02')
#define SSL3_MT_CERTIFICATE             11   //(x'0B')
#define SSL3_MT_SERVER_KEY_EXCHANGE     12   // (x'0C')
#define SSL3_MT_CERTIFICATE_REQUEST     13   // (x'0D')
#define SSL3_MT_SERVER_DONE             14   // (x'0E')
#define SSL3_MT_CERTIFICATE_VERIFY      15   // (x'0F')
#define SSL3_MT_CLIENT_KEY_EXCHANGE     16   // (x'10')
#define SSL3_MT_FINISHED                20   // (x'14')

typedef struct {
	u_char handshake_type[2];
	uint16_t length;
	uint16_t version;
	u_char random[32];
	uint8_t session_id_length;
	u_char data[0];
} __attribute__((packed)) ssl_hello;

typedef struct {
	uint16_t type;
	short length;
	u_char data[0];
} __attribute__((packed)) ssl_extension;

typedef struct {
	uint16_t list_length;
	uint8_t type;
	uint16_t length;
	u_char data[0];
} __attribute__((packed)) ssl_server_name;

// record_type
// SSL3_RT_CHANGE_CIPHER_SPEC      20   (x'14')
// SSL3_RT_ALERT                   21   (x'15')
// SSL3_RT_HANDSHAKE               22   (x'16')
// SSL3_RT_APPLICATION_DATA        23   (x'17')

class SSLProtocol: public Protocol 
{
public:
    	explicit SSLProtocol():Protocol("SSLProtocol","ssl"),
		stats_level_(0),
		ssl_header_(nullptr),total_bytes_(0),
		total_client_hellos_(0),total_server_hellos_(0),
		total_certificates_(0),total_records_(0),total_ban_hosts_(0),
		total_allow_hosts_(0),
		info_cache_(new Cache<SSLInfo>("SSL Info cache")),
		host_cache_(new Cache<StringCache>("Host cache")),
		host_map_(),
		domain_mng_(),ban_domain_mng_(),
		flow_mng_(),
		current_flow_(nullptr) {

		CacheManager::getInstance()->setCache(info_cache_);
	}

    	virtual ~SSLProtocol() {}

	static const uint16_t id = 0;
	static const int header_size = 2;
	int getHeaderSize() const { return header_size;}

	int64_t getTotalBytes() const { return total_bytes_; }
	int64_t getTotalPackets() const { return total_packets_;}
	int64_t getTotalValidatedPackets() const { return total_validated_packets_;}
	int64_t getTotalMalformedPackets() const { return total_malformed_packets_;}

	bool processPacket(Packet& packet) { return true; }
	void processFlow(Flow *flow);

	void setStatisticsLevel(int level) { stats_level_ = level;}
	void statistics(std::basic_ostream<char>& out);
	void statistics() { statistics(std::cout);}

	void releaseCache();

        void setHeader(unsigned char *raw_packet) {
        
                ssl_header_ = reinterpret_cast<ssl_record*>(raw_packet);
        }

	// Condition for say that a payload is ssl 
	bool sslChecker(Packet &packet) { 
	
		if (std::memcmp("\x16\x03",packet.getPayload(),2)==0) {
			setHeader(packet.getPayload());
			++total_validated_packets_; 
			return true;
		} else {
			++total_malformed_packets_;
			return false;
		}
	}

	int32_t getTotalClientHellos() const { return total_client_hellos_; }
	int32_t getTotalServerHellos() const { return total_server_hellos_; }
	int32_t getTotalCertificates() const { return total_certificates_; }
	int32_t getTotalRecords() const { return total_records_; }
	int32_t getTotalBanHosts() const { return total_ban_hosts_; }
	int32_t getTotalAllowHosts() const { return total_allow_hosts_; }

        void increaseAllocatedMemory(int value);
        void decreaseAllocatedMemory(int value);

	void setDomainNameManager(DomainNameManagerPtrWeak dnm) override { domain_mng_ = dnm;}
	void setDomainNameBanManager(DomainNameManagerPtrWeak dnm) override { ban_domain_mng_ = dnm;}

	void setFlowManager(FlowManagerPtrWeak flow_mng) { flow_mng_ = flow_mng; }

	int64_t getAllocatedMemory() const;

#if defined(PYTHON_BINDING)
        boost::python::dict getCounters() const;
	boost::python::dict getCache() const; 
#elif defined(RUBY_BINDING)
        VALUE getCounters() const;
        VALUE getCache() const;
#elif defined(JAVA_BINDING)
        JavaCounters getCounters() const  { JavaCounters counters; return counters; }
#endif

#if defined(STAND_ALONE)
	// Just for testing purposes on the unit test
	Cache<StringCache>::CachePtr getHostCache() const { return host_cache_; }
#endif

private:
        void release_ssl_info_cache(SSLInfo *info);
        int32_t release_ssl_info(SSLInfo *info);

	int stats_level_;
	ssl_record *ssl_header_;
        int64_t total_bytes_;
	int32_t total_client_hellos_;
	int32_t total_server_hellos_;
	int32_t total_certificates_;
	int32_t total_records_;
	int32_t total_ban_hosts_;
	int32_t total_allow_hosts_;

	Cache<SSLInfo>::CachePtr info_cache_;
	Cache<StringCache>::CachePtr host_cache_;

        GenericMapType host_map_;

        DomainNameManagerPtrWeak domain_mng_;
        DomainNameManagerPtrWeak ban_domain_mng_;
	FlowManagerPtrWeak flow_mng_;

	PacketAnomalyType handle_client_hello(SSLInfo *info,int length,int offset, unsigned char *data);
	void handle_server_hello(SSLInfo *info,int offset, unsigned char *data);
	void handle_certificate(SSLInfo *info,int offset, unsigned char *data);

	void attach_host(SSLInfo *info, boost::string_ref &servername); 

#ifdef HAVE_LIBLOG4CXX
        static log4cxx::LoggerPtr logger;
#endif
	Flow *current_flow_; // For accessing for logging
};

typedef std::shared_ptr<SSLProtocol> SSLProtocolPtr;

} // namespace aiengine

#endif  // SRC_PROTOCOLS_SSL_SSLPROTOCOL_H_
