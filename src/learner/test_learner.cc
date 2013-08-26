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
#include "test_learner.h"

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE learnertest
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE (test1_learner)
{
	LearnerEnginePtr le = LearnerEnginePtr(new LearnerEngine());
	PacketFrequenciesPtr pkt_f = PacketFrequenciesPtr(new PacketFrequencies());

	char *payload = "\x16\x04\xaa\xaa";
        unsigned char *pkt = reinterpret_cast <unsigned char*> (payload);
        int length = 4;

	pkt_f->addPayload(pkt,length);

	for (int i = 0;i< 10;++i)
	{
		le->agregatePacketFlow(pkt_f);
	}
	le->statistics();
	std::cout << le->getQualityByte(0);
}


//BOOST_AUTO_TEST_SUITE_END( )

