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
#ifndef _LearnerEngine_H_
#define _LearnerEngine_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <memory>
#include <iomanip> // setw
#include <unordered_map>
#include <boost/format.hpp>
#include "../frequency/PacketFrequencies.h"
#include "../flow/FlowManager.h"

using namespace std;

class LearnerEngine
{
public:

    	explicit LearnerEngine():items_(0),length_(0),
		raw_expression_(""),max_raw_expression_(64) {};
    	virtual ~LearnerEngine() {};

	void reset(); 

	void statistics(std::basic_ostream<char>& out);
	void statistics() { statistics(std::cout);};	

	void agregatePacketFlow(PacketFrequenciesPtr pkt_freq); 
	
	void compute();

	int getQualityByte(int offset);

	std::string &getRegularExpression() { return raw_expression_;};

        void agregateFlows(std::vector<FlowPtrWeak>& flows);

	void setMaxLenghtForRegularExpression(int value) { max_raw_expression_ = value;};
private:
	std::array<std::unordered_map<int,int>,5000> q_array_;
	int length_;
	int items_;
	int max_raw_expression_;
	std::string raw_expression_;	
};

typedef std::shared_ptr<LearnerEngine> LearnerEnginePtr;

#endif