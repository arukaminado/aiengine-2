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
#include <iostream>
#include "RegexManager.h"

namespace aiengine {

void RegexManager::addRegex(const std::string& name,const std::string& expression) {

        SharedPointer<Regex> sig = SharedPointer<Regex>(new Regex(name,expression));

        addRegex(sig);
}

void RegexManager::addRegex(const SharedPointer<Regex>& sig) {

//	std::cout << "RegexManager::addRegex2::" << std::addressof(sig) << ":" << std::addressof(*sig.get()) << std::endl;
        signatures_.push_back(sig);
}

void RegexManager::evaluate(boost::string_ref &data, bool *result) {

	current_signature_.reset();

        for (auto &sig: signatures_) {

                if (sig->evaluate(data)) {
                        ++total_matched_signatures_;
                        current_signature_ = sig;
                        (*result) = true;
                        break;
                }
        }
        return;
}

void RegexManager::statistics(const std::string &name ) {

        std::cout << "RegexManager(" << std::addressof(*this) << ")[" << name << "]" << std::dec <<  std::endl;
        for (auto &it : signatures_ ) {
                SharedPointer<Regex> ssig = it;
                std::ostringstream tabs;

                bool no_more_regex = false;

                while (no_more_regex == false) {
                        tabs << "\t";
			if (name.compare(ssig->getName()) == 0) {
                        	std::cout << tabs.str() << "Regex:" << ssig->getName() << " matches:" << ssig->getMatchs() << std::endl;
			}
                        if (ssig->isTerminal() == false) {

                                if (ssig->getNextRegexManager())
                                        break;

                                no_more_regex = false;
                                SharedPointer<Regex> raux = ssig->getNextRegex();
                                if (raux)
                                        ssig = raux;
                        } else {
                                no_more_regex = true;
                        }
                }
        }
}


std::ostream& operator<< (std::ostream& out, const RegexManager& sig) {

	out << "RegexManager(" << std::addressof(sig) << ") statistics" << std::dec <<  std::endl;	
        for (auto it = sig.signatures_.begin(); it != sig.signatures_.end(); ++it) {
                SharedPointer<Regex> ssig = (*it);
		std::ostringstream tabs;	

		bool no_more_regex = false;

		while (no_more_regex == false) {
			tabs << "\t";
			out << tabs.str() << "Regex:" << ssig->getName() << " matches:" << ssig->getMatchs() << std::endl;
			if (ssig->isTerminal() == false) {

				if (ssig->getNextRegexManager()) 
					break;
 
				no_more_regex = false;
				SharedPointer<Regex> raux = ssig->getNextRegex();
				if (raux)
					ssig = raux;
			} else {
				no_more_regex = true;
			}
		}	
	}
	return out;
}

} // namespace aiengine
