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
#include "SIPProtocol.h"
#include <iomanip> // setw

namespace aiengine {

#ifdef HAVE_LIBLOG4CXX
log4cxx::LoggerPtr SIPProtocol::logger(log4cxx::Logger::getLogger("aiengine.sip"));
#endif

// List of support request methods 
std::vector<SipMethodType> SIPProtocol::methods_ {
	std::make_tuple("REGISTER"	,8,	"registers"	,0),
	std::make_tuple("INVITE"	,6,	"invites"	,0),
	std::make_tuple("ACK"		,3,	"acks"		,0),
	std::make_tuple("CANCEL"	,6,	"cancels"	,0),
	std::make_tuple("BYE"		,3,	"byes"		,0),
	std::make_tuple("MESSAGE"	,7,	"messages"	,0),
	std::make_tuple("OPTIONS"	,7,	"options"	,0),
	std::make_tuple("PUBLISH"	,7,	"publishs"	,0),
	std::make_tuple("SUBSCRIBE"	,9,	"subcribes"	,0),
	std::make_tuple("NOTIFY"	,6,	"notifies"	,0),
	std::make_tuple("REFER"		,5,	"refers"	,0),
	std::make_tuple("INFO"		,4,	"infos"		,0),
	std::make_tuple("PING"		,4,	"pings"		,0)
};

int64_t SIPProtocol::getAllocatedMemory() const {

        int64_t value = 0;

        value = sizeof(SIPProtocol);
        value += info_cache_->getAllocatedMemory();
        value += uri_cache_->getAllocatedMemory();
        value += via_cache_->getAllocatedMemory();
        value += from_cache_->getAllocatedMemory();
        value += to_cache_->getAllocatedMemory();

        return value;
}

void SIPProtocol::releaseCache() {

        FlowManagerPtr fm = flow_mng_.lock();

        if (fm) {
                auto ft = fm->getFlowTable();

                std::ostringstream msg;
                msg << "Releasing " << getName() << " cache";

                infoMessage(msg.str());

                int32_t total_bytes_released = 0;
                int32_t total_bytes_released_by_flows = 0;
                int32_t release_flows = 0;
                int32_t release_from = from_map_.size();
                int32_t release_uris = uri_map_.size();
                int32_t release_to = to_map_.size();
                int32_t release_via = via_map_.size();

                // Compute the size of the strings used as keys on the map
                std::for_each (from_map_.begin(), from_map_.end(), [&total_bytes_released] (PairStringCacheHits const &f) {
                        total_bytes_released += f.first.size();
                });
                std::for_each (uri_map_.begin(), uri_map_.end(), [&total_bytes_released] (PairStringCacheHits const &u) {
                        total_bytes_released += u.first.size();
                });
                std::for_each (to_map_.begin(), to_map_.end(), [&total_bytes_released] (PairStringCacheHits const &t) {
                        total_bytes_released += t.first.size();
                });
                std::for_each (via_map_.begin(), via_map_.end(), [&total_bytes_released] (PairStringCacheHits const &t) {
                        total_bytes_released += t.first.size();
                });

                for (auto &flow: ft) {
			SharedPointer<SIPInfo> sinfo = flow->getSIPInfo();
			if (sinfo) {
                        	SharedPointer<StringCache> sc = sinfo->uri;
				if (sc) {	
					sinfo->uri.reset();
					total_bytes_released_by_flows += sc->getNameSize();
					uri_cache_->release(sc);
				}
                        	sc = sinfo->from;
				if (sc) {	
                                	sinfo->from.reset();
                                	total_bytes_released_by_flows += sc->getNameSize();
                                	from_cache_->release(sc);
                        	}
                        	sc = sinfo->to;
				if (sc) {	
                                	sinfo->to.reset();
                                	total_bytes_released_by_flows += sc->getNameSize();
                                	to_cache_->release(sc);
                        	}
                        	sc = sinfo->via;
				if (sc) {	
                                	sinfo->via.reset();
                                	total_bytes_released_by_flows += sc->getNameSize();
                                	to_cache_->release(sc);
                        	}
                        	++release_flows;
	
				flow->layer7info.reset();
				info_cache_->release(sinfo);
			}
                } 
                uri_map_.clear();
                from_map_.clear();
                to_map_.clear();
                via_map_.clear();

                double cache_compression_rate = 0;

                if (total_bytes_released_by_flows > 0 ) {
                        cache_compression_rate = 100 - ((total_bytes_released*100)/total_bytes_released_by_flows);
                }
        
        	msg.str("");
                msg << "Release " << release_uris << " uris, " << release_via << " vias, " << release_from;
                msg << " froms, " << release_to << " tos, " << release_flows << " flows";
                msg << ", " << total_bytes_released << " bytes, compression rate " << cache_compression_rate << "%";
                infoMessage(msg.str());
        }
}

void SIPProtocol::extract_via_value(SIPInfo *info, boost::string_ref &header) {

        if (sip_via_->matchAndExtract(header)) {

		boost::string_ref via_raw(sip_via_->getExtract());
		boost::string_ref via(via_raw.substr(5,via_raw.size()-7)); // remove also the \r\n
                
                attach_via_to_flow(info,via);
        }
}

void SIPProtocol::extract_from_value(SIPInfo *info, boost::string_ref &header) {

	if (sip_from_->matchAndExtract(header)) {

		boost::string_ref from_raw(sip_from_->getExtract());
		boost::string_ref from(from_raw.substr(6,from_raw.size()-8)); // remove also the \r\n
 
		attach_from_to_flow(info,from);
	}
}


void SIPProtocol::attach_from_to_flow(SIPInfo *info, boost::string_ref &from) {

	if (!info->from) {
		GenericMapType::iterator it = from_map_.find(from);
		if (it == from_map_.end()) {
			SharedPointer<StringCache> from_ptr = from_cache_->acquire();
			if (from_ptr) {
				from_ptr->setName(from.data(),from.length());
				info->from = from_ptr;
				from_map_.insert(std::make_pair(boost::string_ref(from_ptr->getName()),
					std::make_pair(from_ptr,1)));
			}
		} else {
			int *counter = &std::get<1>(it->second);
			++(*counter);
			info->from = std::get<0>(it->second);
		}
	}
}

void SIPProtocol::extract_to_value(SIPInfo *info, boost::string_ref &header) {

	if (sip_to_->matchAndExtract(header)) {

		boost::string_ref to_raw(sip_to_->getExtract());
		boost::string_ref to(to_raw.substr(4,to_raw.size()-6)); // remove also the \r\n

		attach_to_to_flow(info,to);
	}
}

void SIPProtocol::attach_to_to_flow(SIPInfo *info, boost::string_ref &to) {

	if (!info->to) {
		GenericMapType::iterator it = to_map_.find(to);
		if (it == to_map_.end()) {
			SharedPointer<StringCache> to_ptr = to_cache_->acquire();
			if (to_ptr) {
				to_ptr->setName(to.data(),to.length());
				info->to = to_ptr;
				to_map_.insert(std::make_pair(boost::string_ref(to_ptr->getName()),
					std::make_pair(to_ptr,1)));
			}	
		} else {
			int *counter = &std::get<1>(it->second);
			++(*counter);
			info->to = std::get<0>(it->second);	
		}
	}

}

void SIPProtocol::attach_via_to_flow(SIPInfo *info, boost::string_ref &via) {

	if (!info->via) {
                GenericMapType::iterator it = via_map_.find(via);
                if (it == via_map_.end()) {
                        SharedPointer<StringCache> via_ptr = via_cache_->acquire();
                        if (via_ptr) {
                                via_ptr->setName(via.data(),via.length());
                                info->via = via_ptr;
                                via_map_.insert(std::make_pair(boost::string_ref(via_ptr->getName()),
					std::make_pair(via_ptr,1)));
                        }
                } else {
                        int *counter = &std::get<1>(it->second);
                        ++(*counter);
                        info->via = std::get<0>(it->second);
                }
        }
}


void SIPProtocol::attach_uri_to_flow(SIPInfo *info, boost::string_ref &uri) {

	GenericMapType::iterator it = uri_map_.find(uri);
        if (it == uri_map_.end()) {
        	SharedPointer<StringCache> uri_ptr = uri_cache_->acquire();
                if (uri_ptr) {
                	uri_ptr->setName(uri.data(),uri.length());
                        info->uri = uri_ptr;
                        uri_map_.insert(std::make_pair(boost::string_ref(uri_ptr->getName()),
				std::make_pair(uri_ptr,1)));
                } 
        } else {
		// Update the URI of the flow
                int *counter = &std::get<1>(it->second);
                ++(*counter);
		info->uri = std::get<0>(it->second);
	}
}


void SIPProtocol::extract_uri_value(SIPInfo *info, boost::string_ref &header) {

	int offset = 0;
	bool found = false;

	// Check if is a response 
        if (std::memcmp("SIP/2.",&header[0],6) == 0) {
                ++total_responses_;

                // No uri to extract
		return;
	} 

	for (auto &method: methods_) {
		const char *m = std::get<0>(method);
		offset = std::get<1>(method);

		if (std::memcmp(m,&header[0],offset) == 0) {
			int32_t *hits = &std::get<3>(method);

			found = true;
			++offset;
			++(*hits);
			break;
		}
	}

	if ((found)and(offset > 0)) {
		int end = header.find("SIP/2.");
		if (end > 0) {
			boost::string_ref uri(header.substr(offset,(end-offset)-1));
	
			++total_requests_;	
			attach_uri_to_flow(info,uri);	
		}
	}else{
		++total_sip_others_;
	}
}

void SIPProtocol::processFlow(Flow *flow) {

	++total_packets_;	
	int length = flow->packet->getLength();
	total_bytes_ += length;
	++flow->total_packets_l7;

	SharedPointer<SIPInfo> sinfo = flow->getSIPInfo();

        if(!sinfo) {
                sinfo = info_cache_->acquire();
                if (!sinfo) {
                        return;
                }
                flow->layer7info = sinfo;
        }

	boost::string_ref header(reinterpret_cast <const char*> (flow->packet->getPayload()),length);

	// std::cout << __FILE__ << ":" << __func__ << ":header:" << header << std::endl;

	extract_uri_value(sinfo.get(),header);
	
	extract_via_value(sinfo.get(),header);
		
	extract_from_value(sinfo.get(),header);	

	extract_to_value(sinfo.get(),header);
	
}


void SIPProtocol::createSIPInfos(int number) {

	info_cache_->create(number);
	uri_cache_->create(number);
	from_cache_->create(number);
	to_cache_->create(number);
	via_cache_->create(number);
}


void SIPProtocol::destroySIPInfos(int number) {

	info_cache_->destroy(number);
	uri_cache_->destroy(number);
	from_cache_->destroy(number);
	to_cache_->destroy(number);
	via_cache_->destroy(number);
}

void SIPProtocol::statistics(std::basic_ostream<char>& out) {

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
			
				out << "\t" << "Total requests:         " << std::setw(10) << total_requests_ <<std::endl;
				out << "\t" << "Total responses:        " << std::setw(10) << total_responses_ <<std::endl;
				for (auto &method: methods_) {
					const char *label = std::get<2>(method);
					int32_t hits = std::get<3>(method);
					out << "\t" << "Total " << label << ":" << std::right << std::setfill(' ') << std::setw(27 - strlen(label)) << hits <<std::endl;

				}
				out << "\t" << "Total others:           " << std::setw(10) << total_sip_others_ <<std::endl;
			}
			if (stats_level_ > 2) {
				if (flow_forwarder_.lock())
					flow_forwarder_.lock()->statistics(out);
				if (stats_level_ > 3) {
					info_cache_->statistics(out);
					uri_cache_->statistics(out);
					via_cache_->statistics(out);
					from_cache_->statistics(out);
					to_cache_->statistics(out);
					if(stats_level_ > 4) {
						showCacheMap(out,uri_map_,"SIP Uris","Uri");
						showCacheMap(out,via_map_,"SIP Vias","Via");
						showCacheMap(out,from_map_,"SIP Froms","From");
						showCacheMap(out,to_map_,"SIP Tos","To");
					}
				}
			}
		}
	}
}

#if defined(PYTHON_BINDING) || defined(RUBY_BINDING)
#if defined(PYTHON_BINDING)
boost::python::dict SIPProtocol::getCounters() const {
        boost::python::dict counters;
#elif defined(RUBY_BINDING)
VALUE SIPProtocol::getCounters() const {
        VALUE counters = rb_hash_new();
#endif
        addValueToCounter(counters,"packets", total_packets_);
        addValueToCounter(counters,"bytes", total_bytes_);

        addValueToCounter(counters,"requests", total_requests_);
        addValueToCounter(counters,"responses", total_requests_);
	for (auto &method: methods_) {
		const char *label = std::get<2>(method);

		addValueToCounter(counters,label,std::get<3>(method));
	}
        addValueToCounter(counters,"others", total_sip_others_);
        return counters;
}

#endif

} // namespace aiengine 
