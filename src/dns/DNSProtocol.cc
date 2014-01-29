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
#include "DNSProtocol.h"
#include <iomanip> // setw

namespace aiengine {

#ifdef HAVE_LIBLOG4CXX
log4cxx::LoggerPtr DNSProtocol::logger(log4cxx::Logger::getLogger("aiengine.dns"));
#endif

void DNSProtocol::attachDNStoFlow(Flow *flow, std::string &domain) {

	SharedPointer<DNSDomain> dom_ptr = flow->dns_domain.lock();

        if (!dom_ptr) { // There is no DNS attached
		DomainMapType::iterator it = domain_map_.find(domain);
                if (it == domain_map_.end()) {
                	dom_ptr = domain_cache_->acquire().lock();
                        if (dom_ptr) {
                        	dom_ptr->setName(domain);
                                flow->dns_domain = dom_ptr;
                                domain_map_.insert(std::make_pair(domain,std::make_pair(dom_ptr,1)));
                        }
		} else {
			int *counter = &std::get<1>(it->second);
                        ++(*counter);
			flow->dns_domain = std::get<0>(it->second);
		}
	}
}

void DNSProtocol::processFlow(Flow *flow) {

	DomainNameManagerPtr dnm,ban_dnm;
	int length = flow->packet->getLength();
	total_bytes_ += length;
	++total_packets_;
	const unsigned char *payload = flow->packet->getPayload();

	// Just get the standard queries
	if (length > 10) { // Minimum header size consider
		// \x01 \x00 Standar query
		if (std::memcmp("\x01\x00",&payload[2],2) == 0) {
			int queries = payload[5];
			std::string domain;
			int i = 13,value;
		
			// Probably i will need to do it better :(	
			while (payload[i] != '\x00') {
				if(payload[i] < '\x17' )
					domain += ".";
				else
					domain += payload[i];
				++i;
				// TODO: extra check for bogus packets check length
			}

			if (domain.length() > 0) { // The domain is valid

				ban_dnm = ban_domain_mng_.lock();
				if (ban_dnm) {
					SharedPointer<DomainName> domain_candidate = ban_dnm->getDomainName(domain);
					if (domain_candidate) {
#ifdef HAVE_LIBLOG4CXX
						LOG4CXX_INFO (logger, "Flow:" << *flow << " matchs with banned domain " << domain_candidate->getName());
#endif
						++total_ban_queries_;
						return;
					}
				}

				++total_allow_queries_;
		
				attachDNStoFlow(flow,domain);	
				
				dnm = domain_mng_.lock();
				if (dnm) {
					SharedPointer<DomainName> domain_candidate = dnm->getDomainName(domain);
					if (domain_candidate) {
#ifdef HAVE_LIBLOG4CXX
						LOG4CXX_INFO (logger, "Flow:" << *flow << " matchs with " << domain_candidate->getName());
#endif
#ifdef PYTHON_BINDING
						if(domain_candidate->haveCallback()) {
							PyGILState_STATE state(PyGILState_Ensure());
							try {
								boost::python::call<void>(domain_candidate->getCallback(),boost::python::ptr(flow));
							} catch (std::exception &e) {
								std::cout << "ERROR:" << e.what() << std::endl;
							}
							PyGILState_Release(state);
						}							
#endif
					}
				}
			}
		}
	}	
	return;
}

void DNSProtocol::statistics(std::basic_ostream<char>& out)
{
	if(stats_level_ > 0)
	{
        	out << "DNSProtocol(" << this << ") statistics" << std::dec <<  std::endl;
        	out << "\t" << "Total packets:          " << std::setw(10) << total_packets_ <<std::endl;
        	out << "\t" << "Total bytes:            " << std::setw(10) << total_bytes_ <<std::endl;
		if(stats_level_ > 1)
		{
			out << "\t" << "Total validated packets:" << std::setw(10) << total_validated_packets_ <<std::endl;
			out << "\t" << "Total malformed packets:" << std::setw(10) << total_malformed_packets_ <<std::endl;
			if(stats_level_ > 3)
			{
				out << "\t" << "Total allow queries:    " << std::setw(10) << total_allow_queries_ <<std::endl;
				out << "\t" << "Total banned queries:   " << std::setw(10) << total_ban_queries_ <<std::endl;
			}
			if(stats_level_ > 2)	
			{	
				if(flow_forwarder_.lock())
					flow_forwarder_.lock()->statistics(out);
                                if(stats_level_ > 3)
                                {
                                        domain_cache_->statistics(out);
                                        if(stats_level_ > 4)
                                        {       
                                                out << "\tDNS Domains usage" << std::endl;
                                                for(auto it = domain_map_.begin(); it!=domain_map_.end(); ++it)
                                                {
                                                        SharedPointer<DNSDomain> domain = std::get<0>((*it).second);
                                                        int count = std::get<1>((*it).second);
                                                        if(domain)
                                                        	out << "\t\tDomain:" << domain->getName() <<":" << count << std::endl;
                                                }
                                        }
                                }
			}
		}
	}
}

} // namespace aiengine

