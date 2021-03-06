AUTOMAKE_OPTIONS = gnu

all: aiengine

python_PYTHON = pyai_tests.py __init__.py

@CODE_COVERAGE_RULES@
CODE_COVERAGE_LCOV_OPTIONS_DEFAULT = $(CODE_COVERAGE_LCOV_OPTIONS_GCOVPATH) --directory . --no-external 

AM_CPPFLAGS= -g -fprofile-arcs -ftest-coverage

LUA_INCLUDE=@LUA_INCLUDE@
AM_JAVAFLAGS=@AM_JAVAFLAGS@
# EXTRA_FLAGS = -flto not supported on the freebsd compiler
# EXTRA_FLAGS = -O3 '-fprofile-arcs -ftest-coverage -g' For enable gcov tool
EXTRA_FLAGS = -O3

EXTRA_DIST = 	python_wrapper.cc python_help.h ruby_wrapper.i setup.py MANIFEST.in \
		ruai_tests.rb java_wrapper.i JavaAiengineTestsStackLan.java JavaAiengineTestsStackLanIPv6.java \
		JavaAiengineTestsStackVirtual.java JunitTestSuite.java \
		luai_tests.lua lua_wrapper.i	

noinst_HEADERS =        Packet.h Pointer.h RawPacket.h \
			Protocol.h AnomalyManager.h RejectManager.h \
			PacketDispatcher.h EvidenceManager.h \
			NetworkStack.h \
			StackLan.h \
			StackLanIPv6.h \
			Cache.h CacheManager.h IPAddress.h \
			StackMobile.h StackVirtual.h \
			StackOpenFlow.h \
			Multiplexer.h \
			FlowForwarder.h \
			Signature.h Callback.h JaiCallback.h \
			Interpreter.h PyGilContext.h \
                        flow/FlowCache.h \
                        flow/FlowManager.h \
                        Flow.h FlowInfo.h FlowDirection.h StringCache.h \
			ipset/IPAbstractSet.h \
			ipset/IPBloomSet.h \
			ipset/IPSet.h \
			ipset/IPSetManager.h \
			learner/LearnerEngine.h \
			names/DomainName.h \
			names/DomainNode.h \
			names/DomainNameManager.h \
			protocols/ethernet/EthernetProtocol.h \
			protocols/vlan/VLanProtocol.h \
			protocols/mpls/MPLSProtocol.h \
			protocols/ip/IPProtocol.h \
			protocols/ip6/IPv6Protocol.h \
			protocols/icmp/ICMPProtocol.h \
			protocols/icmp6/ICMPv6Header.h \
			protocols/icmp6/ICMPv6Protocol.h \
			protocols/udp/UDPProtocol.h \
			protocols/tcp/TCPHeader.h \
			protocols/tcp/TCPProtocol.h \
			protocols/tcpgeneric/TCPGenericProtocol.h \
			protocols/udpgeneric/UDPGenericProtocol.h \
			protocols/gprs/GPRSProtocol.h \
			protocols/vxlan/VxLanProtocol.h \
			protocols/openflow/OpenFlowProtocol.h \
			protocols/gre/GREProtocol.h \
			protocols/http/HTTPProtocol.h \
			protocols/ssl/SSLProtocol.h \
			protocols/smtp/SMTPProtocol.h \
			protocols/imap/IMAPProtocol.h \
			protocols/pop/POPProtocol.h \
			protocols/bitcoin/BitcoinProtocol.h \
			protocols/modbus/ModbusProtocol.h \
			protocols/dns/DNSProtocol.h \
			protocols/dns/DNSInfo.h \
			protocols/sip/SIPProtocol.h \
			protocols/sip/SIPInfo.h \
			protocols/dhcp/DHCPProtocol.h \
			protocols/ntp/NTPProtocol.h \
			protocols/snmp/SNMPProtocol.h \
			protocols/ssdp/SSDPProtocol.h \
			protocols/ssdp/SSDPInfo.h \
			protocols/coap/CoAPProtocol.h \
			protocols/coap/CoAPInfo.h \
			protocols/rtp/RTPProtocol.h \
			protocols/mqtt/MQTTProtocol.h \
			protocols/netbios/NetbiosProtocol.h \
			protocols/frequency/Frequencies.h \
			protocols/frequency/PacketFrequencies.h \
			protocols/frequency/FrequencyProtocol.h \
			System.h \
			StackTest.h StackLanTest.h \
			Serializable.h \
			DatabaseAdaptor.h 

aiengine_core_sources =	Multiplexer.cc \
			FlowForwarder.cc \
			PacketDispatcher.cc \
			Flow.cc CacheManager.cc \
			Protocol.cc AnomalyManager.cc \
			Callback.cc StringCache.cc \
			Interpreter.cc \
			flow/FlowManager.cc \
			protocols/ethernet/EthernetProtocol.cc \
			protocols/vlan/VLanProtocol.cc \
			protocols/mpls/MPLSProtocol.cc \
			protocols/ip/IPProtocol.cc \
			ipset/IPAbstractSet.cc \
			ipset/IPSet.cc \
			ipset/IPBloomSet.cc \
			ipset/IPSetManager.cc \
			protocols/ip6/IPv6Protocol.cc \
			protocols/icmp/ICMPProtocol.cc \
			protocols/icmp6/ICMPv6Header.cc \
			protocols/icmp6/ICMPv6Protocol.cc \
			protocols/udp/UDPProtocol.cc \
			protocols/tcp/TCPInfo.cc \
			protocols/tcp/TCPHeader.cc \
			protocols/tcp/TCPProtocol.cc \
			protocols/tcpgeneric/TCPGenericProtocol.cc \
			protocols/udpgeneric/UDPGenericProtocol.cc \
			protocols/gprs/GPRSInfo.cc \
			protocols/gprs/GPRSProtocol.cc \
			protocols/vxlan/VxLanProtocol.cc \
			protocols/openflow/OpenFlowProtocol.cc \
			protocols/gre/GREProtocol.cc \
			protocols/http/HTTPInfo.cc \
			protocols/http/HTTPProtocol.cc \
			protocols/http/HTTPUriSet.cc \
			protocols/ssl/SSLInfo.cc \
			protocols/ssl/SSLProtocol.cc \
			protocols/smtp/SMTPProtocol.cc \
			protocols/smtp/SMTPInfo.cc \
			protocols/imap/IMAPProtocol.cc \
			protocols/imap/IMAPInfo.cc \
			protocols/pop/POPProtocol.cc \
			protocols/pop/POPInfo.cc \
			protocols/bitcoin/BitcoinProtocol.cc \
			protocols/bitcoin/BitcoinInfo.cc \
			protocols/modbus/ModbusProtocol.cc \
			protocols/dns/DNSInfo.cc \
			protocols/dns/DNSProtocol.cc \
			protocols/sip/SIPInfo.cc \
			protocols/sip/SIPProtocol.cc \
			protocols/dhcp/DHCPProtocol.cc \
			protocols/ntp/NTPProtocol.cc \
			protocols/snmp/SNMPProtocol.cc \
			protocols/ssdp/SSDPProtocol.cc \
			protocols/ssdp/SSDPInfo.cc \
			protocols/coap/CoAPProtocol.cc \
			protocols/coap/CoAPInfo.cc \
			protocols/rtp/RTPProtocol.cc \
			protocols/mqtt/MQTTProtocol.cc \
			protocols/mqtt/MQTTInfo.cc \
			protocols/netbios/NetbiosProtocol.cc \
			regex/Regex.cc \
			regex/RegexManager.cc RejectManager.cc \
			protocols/frequency/FrequencyProtocol.cc \
			protocols/frequency/FrequencyCounter.cc \
			learner/LearnerEngine.cc \
			names/DomainNameManager.cc \
			names/DomainName.cc \
			System.cc \
			NetworkStack.cc \
			StackMobile.cc StackLan.cc \
			StackLanIPv6.cc StackVirtual.cc \
			StackOpenFlow.cc \
			EvidenceManager.cc 

if HAVE_LIBLOG4CXX
  LOG4CXX_FLAGS = -llog4cxx
else
  LOG4CXX_FLAGS = 
endif

BOOST_LINK_CORE = -lboost_system -lboost_iostreams 

aiengine_SOURCES =      $(aiengine_core_sources) main.cc
aiengine_CPPFLAGS = 	$(EXTRA_FLAGS) $(BOOST_CPPFLAGS)
aiengine_LDFLAGS = 	-DBOOST_ASIO_DISABLE_EPOLL -lpthread -lstdc++ \
			$(BOOST_LDFLAGS) $(BOOST_LINK_CORE) $(PCRE_LIB) \
			-lboost_program_options \
			-lboost_filesystem -lpcap $(LOG4CXX_FLAGS) 

aiengine_LDADD = $(BOOST_LIBS)

bin_PROGRAMS = aiengine

TEST = tests 

check_PROGRAMS = tests 

tests_SOURCES = $(aiengine_core_sources) \
		protocols/ethernet/test_ethernet.cc \
		protocols/vlan/test_vlan.cc \
		protocols/mpls/test_mpls.cc \
		flow/test_flows.cc \
		ipset/test_ipset.cc \
		protocols/ip/test_ip.cc \
		protocols/ip6/test_ip6.cc \
		protocols/udp/test_udp.cc \
		protocols/tcp/test_tcp.cc \
		protocols/tcpgeneric/test_tcpgeneric.cc \
		protocols/udpgeneric/test_udpgeneric.cc \
		protocols/dns/test_dns.cc \
		protocols/gprs/test_gprs.cc \
		protocols/vxlan/test_vxlan.cc \
		protocols/openflow/test_openflow.cc \
		protocols/gre/test_gre.cc \
		protocols/http/test_http.cc \
		protocols/icmp/test_icmp.cc \
		protocols/icmp6/test_icmp6.cc \
		protocols/frequency/test_frequency.cc \
		protocols/ssl/test_ssl.cc \
		protocols/smtp/test_smtp.cc \
		protocols/imap/test_imap.cc \
		protocols/pop/test_pop.cc \
		protocols/bitcoin/test_bitcoin.cc \
		protocols/modbus/test_modbus.cc \
		protocols/sip/test_sip.cc \
		protocols/dhcp/test_dhcp.cc \
		protocols/ntp/test_ntp.cc \
		protocols/snmp/test_snmp.cc \
		protocols/ssdp/test_ssdp.cc \
		protocols/coap/test_coap.cc \
		protocols/rtp/test_rtp.cc \
		protocols/mqtt/test_mqtt.cc \
		protocols/netbios/test_netbios.cc \
		regex/test_regex.cc \
		user/test_users.cc \
		learner/test_learner.cc \
		names/test_names.cc \
		tests.cc 

tests_LDFLAGS = 	-DBOOST_ASIO_DISABLE_EPOLL -lpthread -lstdc++ \
			$(BOOST_LDFLAGS) $(BOOST_LINK_CORE) $(PCRE_LIB) \
			-lboost_program_options \
			-lboost_filesystem -lpcap $(LOG4CXX_FLAGS) $(CODE_COVERAGE_LDFLAGS) 

ruby_objects = 	NetworkStack.o Protocol.o EthernetProtocol.o \
		AnomalyManager.o CacheManager.o Multiplexer.o FlowForwarder.o PacketDispatcher.o \
		Flow.o Callback.o StringCache.o \
		RegexManager.o Regex.o \
		VLanProtocol.o MPLSProtocol.o \
		IPProtocol.o IPv6Protocol.o \
		TCPInfo.o TCPHeader.o TCPProtocol.o UDPProtocol.o \
		TCPGenericProtocol.o UDPGenericProtocol.o \
		HTTPProtocol.o HTTPInfo.o \
		SSLProtocol.o SSLInfo.o \
		DNSProtocol.o DNSInfo.o \
		GPRSInfo.o GPRSProtocol.o \
		NTPProtocol.o SNMPProtocol.o \
		SMTPProtocol.o SMTPInfo.o \
		IMAPProtocol.o IMAPInfo.o \
		DHCPProtocol.o \
		SIPProtocol.o SIPInfo.o \
		SSDPProtocol.o SSDPInfo.o \
		POPProtocol.o POPInfo.o \
		GREProtocol.o \
		OpenFlowProtocol.o VxLanProtocol.o \
		BitcoinProtocol.o BitcoinInfo.o \
		ModbusProtocol.o \
		RTPProtocol.o \
		CoAPProtocol.o CoAPInfo.o \
		MQTTProtocol.o MQTTInfo.o \
		NetbiosProtocol.o \
		FrequencyProtocol.o LearnerEngine.o \
		ICMPProtocol.o ICMPv6Protocol.o ICMPv6Header.o \
		StackLan.o StackMobile.o StackLanIPv6.o StackVirtual.o StackOpenFlow.o \
		FlowManager.o Interpreter.o \
		IPSetManager.o IPAbstractSet.o IPSet.o HTTPUriSet.o \
		DomainNameManager.o DomainName.o RejectManager.o EvidenceManager.o

java_objects = 	RegexManager.o Regex.o \
		NetworkStack.o Protocol.o EthernetProtocol.o \
		FlowManager.o Flow.o Callback.o StringCache.o \
                AnomalyManager.o CacheManager.o Multiplexer.o FlowForwarder.o PacketDispatcher.o \
                VLanProtocol.o MPLSProtocol.o \
                IPProtocol.o IPv6Protocol.o \
		TCPInfo.o TCPHeader.o TCPProtocol.o UDPProtocol.o \
                TCPGenericProtocol.o UDPGenericProtocol.o \
                HTTPProtocol.o HTTPInfo.o SSLProtocol.o SSLInfo.o \
                DNSProtocol.o DNSInfo.o \
		GPRSInfo.o GPRSProtocol.o \
                NTPProtocol.o SNMPProtocol.o \
                SMTPProtocol.o SMTPInfo.o \
		IMAPProtocol.o IMAPInfo.o \
                DHCPProtocol.o SIPProtocol.o SIPInfo.o \
		SSDPProtocol.o SSDPInfo.o \
                POPProtocol.o POPInfo.o \
		GREProtocol.o \
		RTPProtocol.o \
		CoAPProtocol.o CoAPInfo.o \
		BitcoinProtocol.o BitcoinInfo.o ModbusProtocol.o \
		MQTTProtocol.o MQTTInfo.o \
		NetbiosProtocol.o \
                OpenFlowProtocol.o VxLanProtocol.o \
                FrequencyProtocol.o LearnerEngine.o \
                ICMPProtocol.o ICMPv6Protocol.o ICMPv6Header.o \
                StackLan.o StackMobile.o StackLanIPv6.o StackVirtual.o StackOpenFlow.o \
                Interpreter.o \
                IPSetManager.o IPAbstractSet.o IPSet.o HTTPUriSet.o \
                DomainNameManager.o DomainName.o RejectManager.o EvidenceManager.o

lua_objects =  	NetworkStack.o Protocol.o EthernetProtocol.o \
                AnomalyManager.o CacheManager.o Multiplexer.o FlowForwarder.o PacketDispatcher.o \
                Flow.o Callback.o StringCache.o \
                RegexManager.o Regex.o \
                VLanProtocol.o MPLSProtocol.o \
                IPProtocol.o IPv6Protocol.o \
                TCPInfo.o TCPHeader.o TCPProtocol.o UDPProtocol.o \
                TCPGenericProtocol.o UDPGenericProtocol.o \
                HTTPProtocol.o HTTPInfo.o \
                SSLProtocol.o SSLInfo.o \
                DNSProtocol.o DNSInfo.o \
                GPRSInfo.o GPRSProtocol.o \
                NTPProtocol.o SNMPProtocol.o \
                SMTPProtocol.o SMTPInfo.o \
                IMAPProtocol.o IMAPInfo.o \
                DHCPProtocol.o \
                SIPProtocol.o SIPInfo.o \
                SSDPProtocol.o SSDPInfo.o \
                POPProtocol.o POPInfo.o \
                GREProtocol.o \
                OpenFlowProtocol.o VxLanProtocol.o \
                BitcoinProtocol.o BitcoinInfo.o \
                ModbusProtocol.o \
                RTPProtocol.o \
		NetbiosProtocol.o \
                CoAPProtocol.o CoAPInfo.o \
                MQTTProtocol.o MQTTInfo.o \
                FrequencyProtocol.o LearnerEngine.o \
                ICMPProtocol.o ICMPv6Protocol.o ICMPv6Header.o \
                StackLan.o StackMobile.o StackLanIPv6.o StackVirtual.o StackOpenFlow.o \
                FlowManager.o Interpreter.o \
                IPSetManager.o IPAbstractSet.o IPSet.o HTTPUriSet.o \
                DomainNameManager.o DomainName.o RejectManager.o EvidenceManager.o

tests_CPPFLAGS = -DTESTING $(BOOST_CPPFLAGS) $(CODE_COVERAGE_CXXFLAGS) # -fprofile-arcs -ftest-coverage 
# Dont forget to put -lgcov if need on test_LDADD
tests_LDADD   = $(BOOST_LIBS) $(BOOST_LINK_CORE) -lboost_unit_test_framework -lpcap -lpthread $(LOG4CXX_FLAGS) $(CODE_COVERAGE_LDFLAGS) 

CLEANFILES = *.o *.lo *.so *.class *_wrap.cxx *_wrap.h swigluarun.h \
		`rm -rf $(srcdir)/buildjava`

MAINTAINERCLEANFILES = \
        $(srcdir)/Makefile \
        $(srcdir)/Makefile.in \
        `rm -rf $(srcdir)/build `

python:
	@echo "**************************************************************************************"
	@echo "* Depending on your machine this compilation could take some time, please be patient.*"
	@echo "* You can use alternatively the setup.py script (python setup.py build_ext -i).      *"
	@echo "**************************************************************************************"
	$(CXX) -g -O0 -fPIC $(EXTRA_FLAGS) -DHAVE_CONFIG_H -DPYTHON_BINDING \
		-I. -I.. $(BOOST_CPPFLAGS) $(BOOST_CPPFLAGS)/boost/python \
		`pkg-config --cflags python-$(PYTHON_VERSION)` \
		$(aiengine_core_sources) python_wrapper.cc \
		-std=c++11 -lpthread -lstdc++ \
		$(BOOST_LDFLAGS) $(BOOST_LINK_CORE) \
		-l$(BOOST_PYTHON_LIB) \
		`pkg-config --libs python-$(PYTHON_VERSION)` -lpcap \
		$(LOG4CXX_FLAGS) $(PCRE_LIB) -o pyaiengine.so -shared

ruby:
if HAVE_RUBY_SUPPORT
	@echo "**************************************************************************************"
	@echo "* Compiling Ruby extension.                                                          *"
	@echo "**************************************************************************************"
	@echo "Generating interface" $(SWIG)	
	swig -DRUBY_BINDING -DHAVE_CONFIG_H -globalmodule -c++ -ruby ruby_wrapper.i 	
	@echo "Compiling ruby interface"
	$(CXX) -fPIC $(EXTRA_FLAGS) -DHAVE_CONFIG_H -DRUBY_BINDING -I.. -I. \
		-std=c++11 -c $(aiengine_core_sources) ruby_wrapper_wrap.cxx \
		-I/usr/include/ruby -I/usr/local/lib/ruby/1.6/i686-linux 
	@echo "Linking ruby interface"
	$(CXX) -g -fPIC $(EXTRA_FLAGS) -DHAVE_CONFIG_H -DRUBY_BINDING -I.. \
		-shared -std=c++11 $(ruby_objects) ruby_wrapper_wrap.o -o ruaiengine.so $(BOOST_LINK_CORE) \
		$(LOG4CXX_FLAGS) -lpcap -lruby -lpcre
endif
java:
if HAVE_JAVA_SUPPORT
	@echo "**************************************************************************************"
	@echo "* Compiling Java extension.                                                          *"
	@echo "**************************************************************************************"
	@echo "Generating Java interface" 
	$(MKDIR_P) buildjava 2>/dev/null
	swig -DJAVA_BINDING -DHAVE_CONFIG_H -outdir buildjava -c++ -java java_wrapper.i 	
	@echo "Compiling Java interface"
	$(CXX) -fPIC $(EXTRA_FLAGS) -DHAVE_CONFIG_H -DJAVA_BINDING -I./buildjava -I.. -I. $(CPPFLAGS) $(AM_JAVAFLAGS) \
		-std=c++11 -c $(aiengine_core_sources) java_wrapper_wrap.cxx 
	@echo "Linking Java interface"
	$(CXX) -g -fPIC $(EXTRA_FLAGS) -DHAVE_CONFIG_H -DJAVA_BINDING -I.. -I./buildjava  \
		-shared -std=c++11 $(java_objects) java_wrapper_wrap.o -o jaaiengine.so -lpcap \
		$(LOG4CXX_FLAGS) $(BOOST_LINK_CORE) -lpcre 
	@echo "Compiling Java Unit tests"
	$(JAVA_CC) -d buildjava/ -sourcepath buildjava/ -cp /usr/share/java/junit.jar  *.java
endif
lua:
if HAVE_LUA_SUPPORT
	@echo "**************************************************************************************"
	@echo "* Compiling Lua extension. i                                                         *"
	@echo "**************************************************************************************"
	@echo "Generating Lua interface" 
	swig -lua -external-runtime 
	swig -DLUA_BINDING -DHAVE_CONFIG_H -c++ -lua lua_wrapper.i 
	@echo "Compiling Lua interface"
	$(CXX) -fPIC $(EXTRA_FLAGS) -DSWIG_TYPE_TABLE=luaiengine -DHAVE_CONFIG_H -DLUA_BINDING -I.. -I. $(CPPFLAGS) $(LUA_INCLUDE) -I/usr/include \
		-std=c++11 -c $(aiengine_core_sources) lua_wrapper_wrap.cxx 
	@echo "Linking lua interface"
	$(CXX) -g -fPIC $(EXTRA_FLAGS) -DHAVE_CONFIG_H -DLUA_BINDING -I.. \
		-shared -std=c++11 $(lua_objects) lua_wrapper_wrap.o -o luaiengine.so $(BOOST_LINK_CORE) \
		$(LOG4CXX_FLAGS) -lpcap $(LUA_LIB) -lpcre -L/lib64/liblua.so
endif
