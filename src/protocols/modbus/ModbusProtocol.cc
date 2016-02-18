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
#include "ModbusProtocol.h"
#include <iomanip>

namespace aiengine {

void ModbusProtocol::processFlow(Flow *flow) {

	int length = flow->packet->getLength();
	total_bytes_ += length;

	++total_packets_;

	if (length > header_size) {
		setHeader(flow->packet->getPayload());	
		if (ntohs(modbus_header_->length) >= sizeof(modbus_hdr)) {
			struct modbus_hdr *hdr = reinterpret_cast<struct modbus_hdr*>(modbus_header_->data);

			if (hdr->code == MB_CODE_READ_COILS ) {
				std::cout << "yes" << std::endl;
			}
		}
	}
}

void ModbusProtocol::statistics(std::basic_ostream<char>& out){ 

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
				/***
                                out << "\t" << "Total discovers:        " << std::setw(10) << total_dhcp_discover_ <<std::endl;
                                out << "\t" << "Total offers:           " << std::setw(10) << total_dhcp_offer_ <<std::endl;
                                out << "\t" << "Total requests:         " << std::setw(10) << total_dhcp_request_ <<std::endl;
                                out << "\t" << "Total declines:         " << std::setw(10) << total_dhcp_decline_ <<std::endl;
                                out << "\t" << "Total acks:             " << std::setw(10) << total_dhcp_ack_ <<std::endl;
                                out << "\t" << "Total naks:             " << std::setw(10) << total_dhcp_nak_ <<std::endl;
                                out << "\t" << "Total releases:         " << std::setw(10) << total_dhcp_release_ <<std::endl;
                                out << "\t" << "Total informs:          " << std::setw(10) << total_dhcp_inform_ <<std::endl;
				**/
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


#if defined(PYTHON_BINDING) || defined(RUBY_BINDING)

#if defined(PYTHON_BINDING)
boost::python::dict ModbusProtocol::getCounters() const {
        boost::python::dict counters;
#elif defined(RUBY_BINDING)
VALUE ModbusProtocol::getCounters() const {
        VALUE counters = rb_hash_new();
#endif
        addValueToCounter(counters,"packets",total_packets_);
        addValueToCounter(counters,"bytes", total_bytes_);

        return counters;
}

#endif

} // namespace aiengine
