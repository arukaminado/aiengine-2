#ifndef _pop_test_packets_H_
#define _pop_test_packets_H_

static char *raw_packet_ethernet_ip_tcp_pop_server_banner =
	"\x00\x14\x2a\x33\x74\x7e\x00\x0d\xb4\x01\xf9\xd8\x08\x00\x45\x00"
	"\x00\x63\x4d\x47\x40\x00\x40\x06\xea\x83\xc0\xa8\xc3\x64\xc0\xa8"
	"\xbe\x14\x00\x6e\x82\xed\xb2\x48\x8e\x63\x29\x3b\x2c\x21\x80\x18"
	"\x05\xa8\x3c\xe7\x00\x00\x01\x01\x08\x0a\x00\x43\xc3\x32\x00\x1c"
	"\x64\xa8\x2b\x4f\x4b\x20\x72\x65\x61\x64\x79\x20\x20\x3c\x32\x39"
	"\x30\x36\x2e\x31\x32\x35\x38\x38\x38\x36\x39\x35\x34\x40\x76\x69"
	"\x73\x74\x65\x2d\x66\x61\x6d\x69\x6c\x79\x2e\x6e\x65\x74\x3e\x0d"
	"\x0a";

int raw_packet_ethernet_ip_tcp_pop_server_banner_length = 113;

static char *raw_packet_ethernet_ip_tcp_pop_apop_client = 
	"\x00\x0d\xb4\x01\xf9\xd8\x00\x14\x2a\x33\x74\x7e\x08\x00\x45\x00"
	"\x00\x63\x48\x8a\x40\x00\x40\x06\xef\x40\xc0\xa8\xbe\x14\xc0\xa8"
	"\xc3\x64\x82\xed\x00\x6e\x29\x3b\x2c\x21\xb2\x48\x8e\x92\x80\x18"
	"\x00\x5c\x09\x95\x00\x00\x01\x01\x08\x0a\x00\x1c\x64\xb6\x00\x43"
	"\xc3\x32\x41\x50\x4f\x50\x20\x6d\x61\x74\x65\x75\x73\x7a\x20\x38"
	"\x62\x63\x65\x38\x63\x39\x61\x66\x35\x34\x33\x30\x61\x33\x30\x65"
	"\x66\x39\x31\x30\x37\x38\x61\x65\x63\x61\x65\x61\x31\x36\x62\x0d"
	"\x0a";

int raw_packet_ethernet_ip_tcp_pop_apop_client_length = 113;

// packets raw_packet_ethernet_ip_tcp_pop_capa_server and raw_packet_ethernet_ip_tcp_pop_user_client
// are on the same conversation

static char *raw_packet_ethernet_ip_tcp_pop_capa_server = 
	"\x00\x16\xe6\x54\xc8\x97\x00\x1a\x2b\x03\x8a\x0c\x08\x00\x45\x00"
	"\x00\xa2\xad\xbc\x40\x00\xfc\x06\x41\x30\xd9\x0c\x0a\x3e\xac\x1a"
	"\x00\x04\x00\x6e\x88\xa0\x77\x91\x4c\xd2\x1b\x6e\xcd\xd0\x80\x18"
	"\x81\x60\x71\xb3\x00\x00\x01\x01\x08\x0a\x60\xc6\x0e\xea\x00\x59"
	"\x5f\x38\x2b\x4f\x4b\x20\x43\x41\x50\x41\x20\x6c\x69\x73\x74\x20"
	"\x66\x6f\x6c\x6c\x6f\x77\x73\x0d\x0a\x45\x58\x50\x49\x52\x45\x20"
	"\x4e\x45\x56\x45\x52\x0d\x0a\x49\x4d\x50\x4c\x45\x4d\x45\x4e\x54"
	"\x41\x54\x49\x4f\x4e\x20\x70\x6f\x70\x67\x61\x74\x65\x20\x32\x2e"
	"\x34\x35\x0d\x0a\x50\x49\x50\x45\x4c\x49\x4e\x49\x4e\x47\x0d\x0a"
	"\x52\x45\x53\x50\x2d\x43\x4f\x44\x45\x53\x0d\x0a\x54\x4f\x50\x0d"
	"\x0a\x55\x49\x44\x4c\x0d\x0a\x55\x53\x45\x52\x0d\x0a\x2e\x0d\x0a";

int raw_packet_ethernet_ip_tcp_pop_capa_server_length = 176;

static char *raw_packet_ethernet_ip_tcp_pop_user_client = 
	"\x00\x1a\x2b\x03\x8a\x0c\x00\x16\xe6\x54\xc8\x97\x08\x00\x45\x00"
	"\x00\x4e\x93\x36\x40\x00\x40\x06\x18\x0b\xac\x1a\x00\x04\xd9\x0c"
	"\x0a\x3e\x88\xa0\x00\x6e\x1b\x6e\xcd\xd0\x77\x91\x4d\x40\x80\x18"
	"\x00\x2e\x8f\xa9\x00\x00\x01\x01\x08\x0a\x00\x59\x5f\x4a\x60\xc6"
	"\x0e\xea\x55\x53\x45\x52\x20\x78\x78\x78\x78\x78\x78\x74\x65\x73"
	"\x74\x40\x79\x61\x68\x6f\x6f\x2e\x65\x73\x0d\x0a";

int raw_packet_ethernet_ip_tcp_pop_user_client_length = 92;

#endif

