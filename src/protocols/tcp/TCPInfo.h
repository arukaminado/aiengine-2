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
#ifndef SRC_PROTOCOLS_TCP_TCPINFO_H_ 
#define SRC_PROTOCOLS_TCP_TCPINFO_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include "TCPStates.h"
#include "FlowInfo.h"

namespace aiengine {

class TCPInfo: public FlowInfo 
{
public:
    	explicit TCPInfo() { reset(); }
    	virtual ~TCPInfo() {}

        void reset();
	void serialize(std::ostream& stream); 

	// TCP State
        short state_prev;
        short state_curr;

	// TCP Flags
	int16_t syn;
	int16_t syn_ack;
	int16_t ack;
	int16_t fin;
	int16_t rst;
	int16_t push;

#if defined(HAVE_TCP_QOS_METRICS)
	// http://www.thevisiblenetwork.com/2015/04/12/5-key-tcp-metrics-for-performance-monitoring/
	int16_t last_sample_time;
	int16_t connection_setup_time;
	int16_t server_reset_rate;
	int16_t last_client_data_time;
	int16_t application_response_time;
#endif
	// TCP Sequence numbers 0 for upstream and 1 for downstream FlowDirection
	uint32_t seq_num[2];

        friend std::ostream& operator<< (std::ostream& out, const TCPInfo& ti) {
        
                out << "Flg[S(" << ti.syn << ")SA(" << ti.syn_ack << ")A(" << ti.ack;
                out << ")F(" << ti.fin << ")R(" << ti.rst << ")P(" << ti.push << ")Seq(" << ti.seq_num[0] << "," << ti.seq_num[1] << ")]";
#if defined(HAVE_TCP_QOS_METRICS)
		out << "QoS[ST(" << ti.connection_setup_time << ")RR(" << ti.server_reset_rate << ")";
		out << "RT(" << ti.application_response_time << ")]";
#endif
                return out;
        }
};

} // namespace aiengine
 
#endif  // SRC_PROTOCOLS_TCP_TCPINFO_H_
