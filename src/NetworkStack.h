#ifndef _NetworkStack_H_
#define _NetworkStack_H_

#include <iostream>
#include <fstream>
#include "Multiplexer.h"

class NetworkStack 
{
public:
    	NetworkStack() {};
    	virtual ~NetworkStack() {};

	virtual void statistics(std::basic_ostream<char>& out) = 0;
	virtual void statistics() = 0;
	virtual void printFlows(std::basic_ostream<char>& out) = 0;
	virtual void printFlows() = 0;

	virtual const char* getName() = 0;
	virtual void setName(char *name) = 0;

	void virtual setLinkLayerMultiplexer(MultiplexerPtrWeak mux) = 0;
	MultiplexerPtrWeak virtual getLinkLayerMultiplexer() = 0; 

	virtual void setTotalTCPFlows(int value) = 0;
	virtual void setTotalUDPFlows(int value) = 0;
	
};

typedef std::shared_ptr <NetworkStack> NetworkStackPtr;

#endif