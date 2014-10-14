#ifndef _openflow_packets_H_
#define _openflow_packets_H_

// A Mini database of different openflow packets 

static char *raw_packet_ethernet_ip_tcp_of_hello = 
	"\xec\x55\xf9\x27\xcf\xaf\x44\x37\xe6\xb1\x23\x48\x08\x00\x45\xc0"
	"\x00\x3c\xb0\xee\x40\x00\x40\x06\x03\xab\xc0\xa8\x02\x04\xc0\xa8"
	"\x02\x0e\xb0\x93\x19\xe9\xe4\xec\xbb\xaf\x87\x0a\xe1\xe2\x80\x18"
	"\x00\x73\x85\x91\x00\x00\x01\x01\x08\x0a\x03\x91\x0b\x42\x00\x13"
	"\x65\x3e\x01\x00\x00\x08\x00\x00\x00\x0b";

static int raw_packet_ethernet_ip_tcp_of_hello_length = 74;

static char *raw_packet_ethernet_ip_tcp_of_set_config = 
	"\x44\x37\xe6\xb1\x23\x48\xec\x55\xf9\x27\xcf\xaf\x08\x00\x45\xc0"
	"\x00\x40\x84\xdf\x40\x00\x40\x06\x2f\xb6\xc0\xa8\x02\x0e\xc0\xa8"
	"\x02\x04\x19\xe9\xb0\x93\x87\x0a\xe1\xf2\xe4\xec\xbb\xb7\x80\x18"
	"\x00\x72\xa6\xf9\x00\x00\x01\x01\x08\x0a\x00\x13\x65\x3e\x03\x91"
	"\x0b\x42\x01\x09\x00\x0c\x00\x00\x00\x03\x00\x00\x00\x80";

static int raw_packet_ethernet_ip_tcp_of_set_config_length = 78;

static char *raw_packet_ethernet_ip_tcp_of_features_reply =
	"\xec\x55\xf9\x27\xcf\xaf\x44\x37\xe6\xb1\x23\x48\x08\x00\x45\xc0"
	"\x01\x14\xb0\xf1\x40\x00\x40\x06\x02\xd0\xc0\xa8\x02\x04\xc0\xa8"
	"\x02\x0e\xb0\x93\x19\xe9\xe4\xec\xbb\xb7\x87\x0a\xe1\xf2\x80\x18"
	"\x00\x73\x86\x69\x00\x00\x01\x01\x08\x0a\x03\x91\x0b\x43\x00\x13"
	"\x65\x3e\x01\x06\x00\xe0\x00\x00\x00\x02\x00\x00\x44\x37\xe6\xb1"
	"\x23\x48\x00\x00\x01\x00\xfe\x00\x00\x00\x00\x00\x00\xc7\x00\x00"
	"\x0f\xff\x00\x03\xfe\xff\xff\xff\xff\xff\x76\x69\x66\x32\x2e\x30"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x01\x44\x37\xe6\xb1\x23\x48\x65\x6d\x31\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x02\x88\x00\x00\x02\xaf\x00\x00\x02\xaf\x00\x00"
	"\x00\x00\x00\x02\xfe\xff\xff\xff\xff\xff\x76\x69\x66\x31\x2e\x30"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\xff\xfe\x44\x37\xe6\xb1\x23\x48\x78\x65\x6e\x62\x72\x31"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00";

static int raw_packet_ethernet_ip_tcp_of_features_reply_length = 290;

static char *raw_packet_ethernet_ip_tcp_of_pktin_ethernet_arp =
	"\xec\x55\xf9\x27\xcf\xaf\x44\x37\xe6\xb1\x23\x48\x08\x00\x45\xc0"
	"\x00\x82\xb0\xf3\x40\x00\x40\x06\x03\x60\xc0\xa8\x02\x04\xc0\xa8"
	"\x02\x0e\xb0\x93\x19\xe9\xe4\xec\xbc\x97\x87\x0a\xe1\xfe\x80\x18"
	"\x00\x73\x85\xd7\x00\x00\x01\x01\x08\x0a\x03\x91\x0b\xd6\x00\x13"
	"\x65\x4a\x01\x0a\x00\x4e\x00\x00\x00\x00\xff\xff\xff\x00\x00\x3c"
	"\x00\x01\x00\x00\xff\xff\xff\xff\xff\xff\xac\x5d\x10\x31\x37\x79"
	"\x08\x06\x00\x01\x08\x00\x06\x04\x00\x01\xac\x5d\x10\x31\x37\x79"
	"\xc0\xa8\x02\xfe\xff\xff\xff\xff\xff\xff\xc0\xa8\x02\x05\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

static int raw_packet_ethernet_ip_tcp_of_pktin_ethernet_arp_length = 144;

static char *raw_packet_ethernet_ip_tcp_of_pktin_ethernet_ip_udp =
	"\xec\x55\xf9\x27\xcf\xaf\x44\x37\xe6\xb1\x23\x48\x08\x00\x45\xc0"
	"\x00\xc6\xb0\xf7\x40\x00\x40\x06\x03\x18\xc0\xa8\x02\x04\xc0\xa8"
	"\x02\x0e\xb0\x93\x19\xe9\xe4\xec\xbd\x33\x87\x0a\xe2\x2e\x80\x18"
	"\x00\x73\x86\x1b\x00\x00\x01\x01\x08\x0a\x03\x91\x0c\x28\x00\x13"
	"\x66\x1e\x01\x0a\x00\x92\x00\x00\x00\x00\x00\x00\x01\x01\x04\xf4"
	"\x00\x01\x00\x00\x01\x00\x5e\x7f\xff\xfa\x00\x23\xee\xe8\x12\x50"
	"\x08\x00\x45\x40\x04\xe6\x66\x4b\x00\x00\x01\x11\x9b\xd1\xc0\xa8"
	"\x02\x08\xef\xff\xff\xfa\x04\x14\x1f\x92\x04\xd2\xd9\x2b\x02\xc5"
	"\x77\x59\x44\xa6\x47\xe7\x49\x86\xdb\x7e\x88\xa7\x4c\xf1\x95\x00"
	"\x21\x00\x10\x00\x31\x04\x99\xd5\x31\x59\x63\xd9\xc6\x72\x3f\x1a"
	"\x9d\xb0\x4c\x8c\xc0\x69\x77\x65\x2d\x53\x64\x14\xe6\x3d\x99\x4e"
	"\x4f\x54\x49\x46\x59\x20\x2a\x20\x48\x54\x54\x50\x2f\x31\x2e\x31"
	"\x0d\x0a\x78\x2d\x74\x79\x70\x65\x3a\x20\x64\x76\x72\x0d\x0a\x78"
	"\x2d\x6c\x6f\x63";

static int raw_packet_ethernet_ip_tcp_of_pktin_ethernet_ip_udp_length = 212;

static char *raw_packet_ethernet_ip_tcp_of_pktin_ethernet_ip_tcp_ssh1 =
	"\xec\x55\xf9\x27\xcf\xaf\x44\x37\xe6\xb1\x23\x48\x08\x00\x45\xc0"
	"\x00\xb8\xb0\xfd\x40\x00\x40\x06\x03\x20\xc0\xa8\x02\x04\xc0\xa8"
	"\x02\x0e\xb0\x93\x19\xe9\xe4\xec\xbe\xe9\x87\x0a\xe2\x76\x80\x18"
	"\x00\x73\x86\x0d\x00\x00\x01\x01\x08\x0a\x03\x91\x12\x95\x00\x13"
	"\x6c\x08\x01\x0a\x00\x84\x00\x00\x00\x00\x00\x00\x01\x04\x00\x72"
	"\xff\xfe\x00\x00\xec\x55\xf9\x27\xcf\xaf\x44\x37\xe6\xb1\x23\x48"
	"\x08\x00\x45\x10\x00\x64\xe2\x13\x40\x00\x40\x06\xd3\x0d\xc0\xa8"
	"\x02\x04\xc0\xa8\x02\x0e\xb7\x4e\x00\x16\x61\x53\x8b\x01\xcc\xc6"
	"\x66\x60\x80\x18\x01\x4b\x96\x58\x00\x00\x01\x01\x08\x0a\x03\x91"
	"\x12\x95\x00\x13\x64\x6a\x5f\x8c\x75\x58\xad\x4b\x14\x7a\xff\x9d"
	"\x24\x77\x7f\x66\xf4\xad\x3f\x33\xb2\xe4\xa9\xdf\x1c\x3d\xf0\x3b"
	"\x3b\xdb\x5b\xf9\x8e\xa5\xc6\xa4\x0f\x59\x8c\xf5\x57\x1e\xf8\x72"
	"\xdf\xb2\x70\x34\x07\xd0";
static int raw_packet_ethernet_ip_tcp_of_pktin_ethernet_ip_tcp_ssh1_length = 198;

static char *raw_packet_ethernet_ip_tcp_of_pktin_ethernet_ip_tcp_ssh2 = 
	"\xec\x55\xf9\x27\xcf\xaf\x44\x37\xe6\xb1\x23\x48\x08\x00\x45\xc0"
	"\x00\xb8\xb0\xff\x40\x00\x40\x06\x03\x1e\xc0\xa8\x02\x04\xc0\xa8"
	"\x02\x0e\xb0\x93\x19\xe9\xe4\xec\xbf\x6d\x87\x0a\xe2\x8e\x80\x18"
	"\x00\x73\x86\x0d\x00\x00\x01\x01\x08\x0a\x03\x91\x12\x97\x00\x13"
	"\x6c\x91\x01\x0a\x00\x84\x00\x00\x00\x00\x00\x00\x01\x05\x00\x72"
	"\x00\x01\x00\x00\x44\x37\xe6\xb1\x23\x48\xec\x55\xf9\x27\xcf\xaf"
	"\x08\x00\x45\x10\x00\x64\x23\x02\x40\x00\x40\x06\x92\x1f\xc0\xa8"
	"\x02\x0e\xc0\xa8\x02\x04\x00\x16\xb7\x4e\xcc\xc6\x66\x60\x61\x53"
	"\x8b\x31\x80\x18\x01\x08\x91\x50\x00\x00\x01\x01\x08\x0a\x00\x13"
	"\x6c\x92\x03\x91\x12\x95\x26\x01\x8f\xa1\xd1\x59\xbd\x01\xfa\x07"
	"\x2a\x72\x3c\x5f\xa4\x2f\x20\xeb\x09\xa0\xdc\x9c\x46\xbe\xb6\x9c"
	"\xaf\x18\xf2\x38\xb0\x21\xe9\xbf\x78\xbd\x6a\x1d\xb5\xda\x7f\x7c"
	"\x0a\xf1\x7a\x07\xdf\x05";
static int raw_packet_ethernet_ip_tcp_of_pktin_ethernet_ip_tcp_ssh2_length = 198;

#endif
