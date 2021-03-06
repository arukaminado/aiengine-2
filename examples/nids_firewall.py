#!/usr/bin/env python

""" Example for integrate NIDS functionality with iptables """

__author__ = "Luis Campo Giralte"
__copyright__ = "Copyright (C) 2013-2016 by Luis Campo Giralte"
__revision__ = "$Id$"
__version__ = "0.1"

import sys
import os
sys.path.append("../src/")
import pyaiengine

def callback_drop_packets(flow):
    """ Send a command to the Iptables in other to drop the packets """
#    os.system("iptables -A INPUT -i eth0 -s %s -j DROP" % flow.src_ip)

def loadSignaturesForTcp():
     """ Load the signatures from source, Snort, Suricata, etc. """

     sm = pyaiengine.RegexManager()

     sig = pyaiengine.Regex("Shellcode Generic Exploit","\x90\x90\x90\x90\x90\x90\x90\x90\x90")

     """ Sets a specific callback to the signature created """
     sig.callback = callback_drop_packets
     sm.add_regex(sig)

     return sm

if __name__ == '__main__':

     # Load an instance of a Network Stack
     st = pyaiengine.StackLan()

     # Load Signatures/Rules in order to detect the traffic
     s_tcp = loadSignaturesForTcp()
     st.tcp_regex_manager = s_tcp

     st.enable_nids_engine(True)

     st.tcp_flows = 327680
     st.udp_flows = 163840

     with pyaiengine.PacketDispatcher("eth0") as pd:
         pd.stack = st
         pd.run()

     # Dump on file the statistics of the stack
     st.stats_level = 5
     f = open("statistics.log","w")
     f.write(str(st))
     f.close()

     sys.exit(0)

