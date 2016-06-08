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
#ifndef SRC_IPSET_IPABSTRACTSET_H_
#define SRC_IPSET_IPABSTRACTSET_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <memory>
#include <string>
#include <iostream>
#include "../regex/RegexManager.h"

#if defined(PYTHON_BINDING) || defined(RUBY_BINDING) || defined(JAVA_BINDING) || defined(LUA_BINDING)
#include "Callback.h"
#endif

namespace aiengine {

class Flow;

class IPAbstractSet 
{
public:
    	explicit IPAbstractSet(const std::string &name):total_ips_(0),
		total_ips_not_on_set_(0),total_ips_on_set_(0),
#if defined(PYTHON_BINDING) || defined(RUBY_BINDING) || defined(JAVA_BINDING) || defined(LUA_BINDING)
		call(),
#endif
		name_(name),
		rmng_(),
		have_regex_mng_(false) 
	{}
    	
    	explicit IPAbstractSet():IPAbstractSet("Generic IPAbstractSet") {}

	virtual ~IPAbstractSet() {}

	const char *getName() const { return name_.c_str();}

	virtual void addIPAddress(const std::string &ip) = 0;
	virtual bool lookupIPAddress(const std::string &ip) = 0; 
	virtual int getFalsePositiveRate() = 0;

	int32_t getTotalIPs() const { return total_ips_; }
	int32_t getTotalLookups() const { return (total_ips_on_set_ + total_ips_not_on_set_); }
	int32_t getTotalLookupsIn() const { return total_ips_on_set_; }
	int32_t getTotalLookupsOut() const { return total_ips_not_on_set_; }

#ifdef PYTHON_BINDING
	void setCallback(PyObject *callback) { call.setCallback(callback); }
	PyObject *getCallback() const { return call.getCallback(); }
#endif

	int32_t total_ips_;
	int32_t total_ips_not_on_set_;
	int32_t total_ips_on_set_;
#if defined(PYTHON_BINDING) || defined(RUBY_BINDING) || defined(JAVA_BINDING) || defined(LUA_BINDING)
	Callback call;	
#endif

	void setRegexManager(const SharedPointer<RegexManager>& rmng); 
	SharedPointer<RegexManager> getRegexManager() const { return rmng_; }

	bool haveRegexManager() const { return have_regex_mng_; }

#if defined(RUBY_BINDING)
        void setRegexManager(RegexManager& regex_mng); 
#elif defined(JAVA_BINDING)
        void setRegexManager(RegexManager *regex_mng); 
#endif

private:
	std::string name_;
	SharedPointer<RegexManager> rmng_;
	bool have_regex_mng_;
};

typedef std::shared_ptr<IPAbstractSet> IPAbstractSetPtr;
typedef std::weak_ptr<IPAbstractSet> IPAbstractSetPtrWeak;

} // namespace aiengine

#endif  // SRC_IPSET_IPABSTRACTSET_H_
