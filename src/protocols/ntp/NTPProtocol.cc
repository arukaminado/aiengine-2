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
#include "NTPProtocol.h"
#include <iomanip>

namespace aiengine {

void NTPProtocol::processFlow(Flow *flow) {

	setHeader(flow->packet->getPayload());	
	uint8_t mode = getMode();
	total_bytes_ += flow->packet->getLength();

	++total_packets_;

	if (mode == NTP_MODE_CLIENT) {
		++total_ntp_client_;
	} else if (mode == NTP_MODE_SERVER) {
		++total_ntp_server_;
	} else if (mode == NTP_MODE_UNSPEC) {
		++total_ntp_unspecified_;
	} else if (mode == NTP_MODE_SYM_ACT) {
		++total_ntp_sym_active_;
	} else if (mode == NTP_MODE_SYM_PAS) {
		++total_ntp_sym_passive_;
	} else if (mode == NTP_MODE_BROADCAST) {
		++total_ntp_broadcast_;
	} else if (mode == NTP_MODE_RES1) {
		++total_ntp_reserved_;	
	} else if (mode == NTP_MODE_RES2) {
		++total_ntp_reserved_;	
	}	
}

void NTPProtocol::statistics(std::basic_ostream<char>& out){ 

	if (stats_level_ > 0) {
                int alloc_memory = getAllocatedMemory();
                std::string unit = "Bytes";

                unitConverter(alloc_memory,unit);

                out << getName() << "(" << this <<") statistics" << std::dec << std::endl;
                out << "\t" << "Total allocated:        " << std::setw(9 - unit.length()) << alloc_memory << " " << unit <<std::endl;
		out << "\t" << "Total packets:          " << std::setw(10) << total_packets_ <<std::endl;
		out << "\t" << "Total bytes:        " << std::setw(14) << total_bytes_ <<std::endl;
		if (stats_level_> 1) {
			out << "\t" << "Total validated packets:" << std::setw(10) << total_validated_packets_ <<std::endl;
			out << "\t" << "Total malformed packets:" << std::setw(10) << total_malformed_packets_ <<std::endl;
                        if (stats_level_ > 3) {

                                out << "\t" << "Total clients:          " << std::setw(10) << total_ntp_client_ <<std::endl;
                                out << "\t" << "Total servers:          " << std::setw(10) << total_ntp_server_ <<std::endl;
                                out << "\t" << "Total unspecifieds:     " << std::setw(10) << total_ntp_unspecified_ <<std::endl;
                                out << "\t" << "Total sym actives:      " << std::setw(10) << total_ntp_sym_active_ <<std::endl;
                                out << "\t" << "Total sym passives:     " << std::setw(10) << total_ntp_sym_passive_ <<std::endl;
                                out << "\t" << "Total broadcasts:       " << std::setw(10) << total_ntp_broadcast_ <<std::endl;
                                out << "\t" << "Total reserveds:        " << std::setw(10) << total_ntp_reserved_ <<std::endl;
                        } 
			if (stats_level_ > 2) {
				if(mux_.lock())
					mux_.lock()->statistics(out);
                                if (flow_forwarder_.lock())
                                        flow_forwarder_.lock()->statistics(out);
			}
		}
	}
}

#if defined(PYTHON_BINDING) || defined(RUBY_BINDING) || defined(LUA_BINDING)
#if defined(PYTHON_BINDING)
boost::python::dict NTPProtocol::getCounters() const {
        boost::python::dict counters;
#elif defined(RUBY_BINDING)
VALUE NTPProtocol::getCounters() const {
        VALUE counters = rb_hash_new();
#elif defined(LUA_BINDING)
LuaCounters NTPProtocol::getCounters() const {
	LuaCounters counters;
#endif
        addValueToCounter(counters,"packets", total_packets_);
        addValueToCounter(counters,"bytes", total_bytes_);
	addValueToCounter(counters,"clients", total_ntp_client_);
	addValueToCounter(counters,"servers", total_ntp_server_);
	addValueToCounter(counters,"unspecifieds", total_ntp_unspecified_);
	addValueToCounter(counters,"sym actives", total_ntp_sym_active_);
	addValueToCounter(counters,"sym passives", total_ntp_sym_passive_);
	addValueToCounter(counters,"broadcasts", total_ntp_broadcast_);
	addValueToCounter(counters,"reserveds", total_ntp_reserved_);
	
        return counters;
}

#endif

} // namespace aiengine
