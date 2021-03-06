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
#ifndef SRC_LEARNER_LEARNERENGINE_H_
#define SRC_LEARNER_LEARNERENGINE_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <memory>
#include <iomanip> // setw
#include <unordered_map>
#include <boost/format.hpp>

#ifdef PYTHON_BINDING
#include <boost/python.hpp>
#endif

#include "protocols/frequency/PacketFrequencies.h"
#include "protocols/frequency/FrequencyGroup.h"
#include "flow/FlowManager.h"

namespace aiengine {

class LearnerEngine
{
public:

    	explicit LearnerEngine():length_(0),items_(0),max_raw_expression_(64),
		raw_expression_(""),regex_expression_(""),
		freq_group_(),q_array_() {}

    	virtual ~LearnerEngine() {};

	void reset(); 

	void statistics(std::basic_ostream<char>& out);
	void statistics() { statistics(std::cout);};	

	void agregatePacketFlow(const SharedPointer<PacketFrequencies>& pkt_freq); 
	
	void compute();
	void setMaxBufferSize(int size) { max_raw_expression_ = size; }

	int getQualityByte(int offset);

	std::string getRegularExpression() { return regex_expression_;};
	std::string getRawExpression() { return raw_expression_;};
	std::string getAsciiExpression();

	void setMaxLenghtForRegularExpression(int value) { max_raw_expression_ = value;};

	void setFrequencyGroup(const FrequencyGroup<std::string>::Ptr& fg) { freq_group_ = fg;};

	int getTotalFlowsProcess() const { return items_;};
#if defined(PYTHON_BINDING) 
	void agregateFlows(boost::python::list flows);
#elif defined(RUBY_BINDING)
	void agregateFlows(VALUE flows);
#else
	void agregateFlows(const std::vector<WeakPointer<Flow>>& flows);
#endif

private:
	int length_;
	int items_;
	int max_raw_expression_;
	std::string raw_expression_;	
	std::string regex_expression_;	
	FrequencyGroup<std::string>::PtrWeak freq_group_;
	std::array<std::unordered_map<unsigned short,int>,MAX_PACKET_FREQUENCIES_VALUES> q_array_;
};

typedef std::shared_ptr<LearnerEngine> LearnerEnginePtr;

} // namespace aiengine

#endif  // SRC_LEARNER_LEARNERENGINE_H_
