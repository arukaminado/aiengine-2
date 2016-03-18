#ifndef _bitcoin_test_packets_H_
#define _bitcoin_test_packets_H_

static char *raw_packet_ethernet_ip_tcp_bc_flow1_syn = 
	"\x00\x11\x43\xd1\x1e\xac\x00\x0f\x34\x30\x6b\x71\x08\x00\x45\x00"
	"\x00\x34\x2e\x69\x40\x00\x74\x06\x3b\x8a\x50\xd4\xff\x52\xd5\x84"
	"\x77\x25\x40\xc5\x20\x8d\x42\x05\xee\x2c\x00\x00\x00\x00\x80\x02"
	"\x20\x00\x20\xc9\x00\x00\x02\x04\x05\xac\x01\x03\x03\x02\x01\x01"
	"\x04\x02";

int raw_packet_ethernet_ip_tcp_bc_flow1_syn_length = 66;

static char *raw_packet_ethernet_ip_tcp_bc_flow1_synack =
	"\x00\x0f\x34\x30\x6b\x71\x00\x11\x43\xd1\x1e\xac\x08\x00\x45\x00"
	"\x00\x34\x00\x00\x40\x00\x40\x06\x9d\xf3\xd5\x84\x77\x25\x50\xd4"
	"\xff\x52\x20\x8d\x40\xc5\x47\x75\x1a\x58\x42\x05\xee\x2d\x80\x12"
	"\x16\xd0\xc8\x0e\x00\x00\x02\x04\x05\xb4\x01\x01\x04\x02\x01\x03"
	"\x03\x06";

int raw_packet_ethernet_ip_tcp_bc_flow1_synack_length = 66;

static char *raw_packet_ethernet_ip_tcp_bc_flow1_ack = 
	"\x00\x11\x43\xd1\x1e\xac\x00\x0f\x34\x30\x6b\x71\x08\x00\x45\x00"
	"\x00\x28\x2e\x6b\x40\x00\x74\x06\x3b\x94\x50\xd4\xff\x52\xd5\x84"
	"\x77\x25\x40\xc5\x20\x8d\x42\x05\xee\x2d\x47\x75\x1a\x59\x50\x10"
	"\x41\x3a\xde\x75\x00\x00\x00\x00";

int raw_packet_ethernet_ip_tcp_bc_flow1_ack_length = 56;

static char *raw_packet_ethernet_ip_tcp_bc_flow1_ack_version =
	"\x00\x11\x43\xd1\x1e\xac\x00\x0f\x34\x30\x6b\x71\x08\x00\x45\x00"
	"\x00\x91\x2e\x6d\x40\x00\x74\x06\x3b\x29\x50\xd4\xff\x52\xd5\x84"
	"\x77\x25\x40\xc5\x20\x8d\x42\x05\xee\x2d\x47\x75\x1a\x59\x50\x18"
	"\x41\x3a\xe7\x1e\x00\x00\xf9\xbe\xb4\xd9\x76\x65\x72\x73\x69\x6f"
	"\x6e\x00\x00\x00\x00\x00\x55\x00\x00\x00\x64\x7d\x00\x00\x01\x00"
	"\x00\x00\x00\x00\x00\x00\xfb\x25\xee\x4d\x00\x00\x00\x00\x01\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\xff\xff\xd5\x84\x77\x25\x20\x8d\x01\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\x50\xd4\xff\x52"
	"\x20\x8d\x60\xba\x6d\xb8\xa0\xb1\x9c\x51\x00\xaf\xf8\x01\x00";

int raw_packet_ethernet_ip_tcp_bc_flow1_ack_version_length = 159;

// Contains a version, verack and  a getblocks
static char *raw_packet_ethernet_ip_tcp_bc_flow2_ack_multiple =
	"\x00\x0f\x34\x30\x6b\x71\x00\x11\x43\xd1\x1e\xac\x08\x00\x45\x00"
	"\x01\x59\x87\xb0\x40\x00\x40\x06\x15\x1e\xd5\x84\x77\x25\x50\xd4"
	"\xff\x52\x20\x8d\x40\xc5\x47\x75\x1a\x59\x42\x05\xee\x96\x50\x18"
	"\x00\x5c\x9e\x1c\x00\x00\xf9\xbe\xb4\xd9\x76\x65\x72\x73\x69\x6f"
	"\x6e\x00\x00\x00\x00\x00\x55\x00\x00\x00\x64\x7d\x00\x00\x01\x00"
	"\x00\x00\x00\x00\x00\x00\xf4\x25\xee\x4d\x00\x00\x00\x00\x01\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\xff\xff\x50\xd4\xff\x52\x40\xc5\x01\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xd5\x84\x77\x25"
	"\x20\x8d\x99\x8e\x6d\xa6\xa6\xcf\xeb\xb3\x00\x00\x00\x00\x00\xf9"
	"\xbe\xb4\xd9\x76\x65\x72\x61\x63\x6b\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\xf9\xbe\xb4\xd9\x67\x65\x74\x62\x6c\x6f\x63\x6b\x73"
	"\x00\x00\x00\x65\x00\x00\x00\x4a\xcf\xb3\x2d\x64\x7d\x00\x00\x02"
	"\x6f\xe2\x8c\x0a\xb6\xf1\xb3\x72\xc1\xa6\xa2\x46\xae\x63\xf7\x4f"
	"\x93\x1e\x83\x65\xe1\x5a\x08\x9c\x68\xd6\x19\x00\x00\x00\x00\x00"
	"\x6f\xe2\x8c\x0a\xb6\xf1\xb3\x72\xc1\xa6\xa2\x46\xae\x63\xf7\x4f"
	"\x93\x1e\x83\x65\xe1\x5a\x08\x9c\x68\xd6\x19\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\xf9\xbe\xb4\xd9\x61\x64\x64\x72\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x1f\x00\x00\x00\xb8\x7b\x5c\xcf\x01\xf4\x25\xee\x4d\x01\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff"
	"\xff\xd5\x84\x77\x25\x20\x8d";

int raw_packet_ethernet_ip_tcp_bc_flow2_ack_multiple_length = 359;

// Contains a multiple blocks operations 
static char *raw_packet_ethernet_ip_tcp_bc_flow3_ack_multiple_blocks =
	"\x00\x11\x43\xd1\x1e\xac\x00\x0f\x34\x30\x6b\x71\x08\x00\x45\x00"
	"\x05\xd4\x2e\x93\x40\x00\x74\x06\x35\xc0\x50\xd4\xff\x52\xd5\x84"
	"\x77\x25\x40\xc5\x20\x8d\x42\x06\x35\x64\x47\x75\x61\xf5\x50\x10"
	"\x40\xa3\x3d\x4c\x00\x00\xf9\xbe\xb4\xd9\x62\x6c\x6f\x63\x6b\x00"
	"\x00\x00\x00\x00\x00\x00\xd7\x00\x00\x00\x93\x4d\x27\x0a\x01\x00"
	"\x00\x00\x6f\xe2\x8c\x0a\xb6\xf1\xb3\x72\xc1\xa6\xa2\x46\xae\x63"
	"\xf7\x4f\x93\x1e\x83\x65\xe1\x5a\x08\x9c\x68\xd6\x19\x00\x00\x00"
	"\x00\x00\x98\x20\x51\xfd\x1e\x4b\xa7\x44\xbb\xbe\x68\x0e\x1f\xee"
	"\x14\x67\x7b\xa1\xa3\xc3\x54\x0b\xf7\xb1\xcd\xb6\x06\xe8\x57\x23"
	"\x3e\x0e\x61\xbc\x66\x49\xff\xff\x00\x1d\x01\xe3\x62\x99\x01\x01"
	"\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\xff\xff\xff\xff\x07\x04\xff\xff\x00\x1d\x01\x04"
	"\xff\xff\xff\xff\x01\x00\xf2\x05\x2a\x01\x00\x00\x00\x43\x41\x04"
	"\x96\xb5\x38\xe8\x53\x51\x9c\x72\x6a\x2c\x91\xe6\x1e\xc1\x16\x00"
	"\xae\x13\x90\x81\x3a\x62\x7c\x66\xfb\x8b\xe7\x94\x7b\xe6\x3c\x52"
	"\xda\x75\x89\x37\x95\x15\xd4\xe0\xa6\x04\xf8\x14\x17\x81\xe6\x22"
	"\x94\x72\x11\x66\xbf\x62\x1e\x73\xa8\x2c\xbf\x23\x42\xc8\x58\xee"
	"\xac\x00\x00\x00\x00\xf9\xbe\xb4\xd9\x62\x6c\x6f\x63\x6b\x00\x00"
	"\x00\x00\x00\x00\x00\xd7\x00\x00\x00\xb0\xf6\xda\x75\x01\x00\x00"
	"\x00\x48\x60\xeb\x18\xbf\x1b\x16\x20\xe3\x7e\x94\x90\xfc\x8a\x42"
	"\x75\x14\x41\x6f\xd7\x51\x59\xab\x86\x68\x8e\x9a\x83\x00\x00\x00"
	"\x00\xd5\xfd\xcc\x54\x1e\x25\xde\x1c\x7a\x5a\xdd\xed\xf2\x48\x58"
	"\xb8\xbb\x66\x5c\x9f\x36\xef\x74\x4e\xe4\x2c\x31\x60\x22\xc9\x0f"
	"\x9b\xb0\xbc\x66\x49\xff\xff\x00\x1d\x08\xd2\xbd\x61\x01\x01\x00"
	"\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\xff\xff\xff\xff\x07\x04\xff\xff\x00\x1d\x01\x0b\xff"
	"\xff\xff\xff\x01\x00\xf2\x05\x2a\x01\x00\x00\x00\x43\x41\x04\x72"
	"\x11\xa8\x24\xf5\x5b\x50\x52\x28\xe4\xc3\xd5\x19\x4c\x1f\xcf\xaa"
	"\x15\xa4\x56\xab\xdf\x37\xf9\xb9\xd9\x7a\x40\x40\xaf\xc0\x73\xde"
	"\xe6\xc8\x90\x64\x98\x4f\x03\x38\x52\x37\xd9\x21\x67\xc1\x3e\x23"
	"\x64\x46\xb4\x17\xab\x79\xa0\xfc\xae\x41\x2a\xe3\x31\x6b\x77\xac"
	"\x00\x00\x00\x00\xf9\xbe\xb4\xd9\x62\x6c\x6f\x63\x6b\x00\x00\x00"
	"\x00\x00\x00\x00\xd7\x00\x00\x00\x2d\xbf\xed\x7d\x01\x00\x00\x00"
	"\xbd\xdd\x99\xcc\xfd\xa3\x9d\xa1\xb1\x08\xce\x1a\x5d\x70\x03\x8d"
	"\x0a\x96\x7b\xac\xb6\x8b\x6b\x63\x06\x5f\x62\x6a\x00\x00\x00\x00"
	"\x44\xf6\x72\x22\x60\x90\xd8\x5d\xb9\xa9\xf2\xfb\xfe\x5f\x0f\x96"
	"\x09\xb3\x87\xaf\x7b\xe5\xb7\xfb\xb7\xa1\x76\x7c\x83\x1c\x9e\x99"
	"\x5d\xbe\x66\x49\xff\xff\x00\x1d\x05\xe0\xed\x6d\x01\x01\x00\x00"
	"\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\xff\xff\xff\xff\x07\x04\xff\xff\x00\x1d\x01\x0e\xff\xff"
	"\xff\xff\x01\x00\xf2\x05\x2a\x01\x00\x00\x00\x43\x41\x04\x94\xb9"
	"\xd3\xe7\x6c\x5b\x16\x29\xec\xf9\x7f\xff\x95\xd7\xa4\xbb\xda\xc8"
	"\x7c\xc2\x60\x99\xad\xa2\x80\x66\xc6\xff\x1e\xb9\x19\x12\x23\xcd"
	"\x89\x71\x94\xa0\x8d\x0c\x27\x26\xc5\x74\x7f\x1d\xb4\x9e\x8c\xf9"
	"\x0e\x75\xdc\x3e\x35\x50\xae\x9b\x30\x08\x6f\x3c\xd5\xaa\xac\x00"
	"\x00\x00\x00\xf9\xbe\xb4\xd9\x62\x6c\x6f\x63\x6b\x00\x00\x00\x00"
	"\x00\x00\x00\xd7\x00\x00\x00\x9c\x5f\x65\x7f\x01\x00\x00\x00\x49"
	"\x44\x46\x95\x62\xae\x1c\x2c\x74\xd9\xa5\x35\xe0\x0b\x6f\x3e\x40"
	"\xff\xba\xd4\xf2\xfd\xa3\x89\x55\x01\xb5\x82\x00\x00\x00\x00\x7a"
	"\x06\xea\x98\xcd\x40\xba\x2e\x32\x88\x26\x2b\x28\x63\x8c\xec\x53"
	"\x37\xc1\x45\x6a\xaf\x5e\xed\xc8\xe9\xe5\xa2\x0f\x06\x2b\xdf\x8c"
	"\xc1\x66\x49\xff\xff\x00\x1d\x2b\xfe\xe0\xa9\x01\x01\x00\x00\x00"
	"\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\xff\xff\xff\xff\x07\x04\xff\xff\x00\x1d\x01\x1a\xff\xff\xff"
	"\xff\x01\x00\xf2\x05\x2a\x01\x00\x00\x00\x43\x41\x04\x18\x4f\x32"
	"\xb2\x12\x81\x5c\x6e\x52\x2e\x66\x68\x63\x24\x03\x0f\xf7\xe5\xbf"
	"\x08\xef\xb2\x1f\x8b\x00\x61\x4f\xb7\x69\x0e\x19\x13\x1d\xd3\x13"
	"\x04\xc5\x4f\x37\xba\xa4\x0d\xb2\x31\xc9\x18\x10\x6b\xb9\xfd\x43"
	"\x37\x3e\x37\xae\x31\xa0\xbe\xfc\x6e\xca\xef\xb8\x67\xac\x00\x00"
	"\x00\x00\xf9\xbe\xb4\xd9\x62\x6c\x6f\x63\x6b\x00\x00\x00\x00\x00"
	"\x00\x00\xd7\x00\x00\x00\xd1\x59\x74\x07\x01\x00\x00\x00\x85\x14"
	"\x4a\x84\x48\x8e\xa8\x8d\x22\x1c\x8b\xd6\xc0\x59\xda\x09\x0e\x88"
	"\xf8\xa2\xc9\x96\x90\xee\x55\xdb\xba\x4e\x00\x00\x00\x00\xe1\x1c"
	"\x48\xfe\xcd\xd9\xe7\x25\x10\xca\x84\xf0\x23\x37\x0c\x9a\x38\xbf"
	"\x91\xac\x5c\xae\x88\x01\x9b\xee\x94\xd2\x45\x28\x52\x63\x44\xc3"
	"\x66\x49\xff\xff\x00\x1d\x1d\x03\xe4\x77\x01\x01\x00\x00\x00\x01"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\xff\xff\xff\xff\x07\x04\xff\xff\x00\x1d\x01\x20\xff\xff\xff\xff"
	"\x01\x00\xf2\x05\x2a\x01\x00\x00\x00\x43\x41\x04\x56\x57\x95\x36"
	"\xd1\x50\xfb\xce\x94\xee\x62\xb4\x7d\xb2\xca\x43\xaf\x0a\x73\x0a"
	"\x04\x67\xba\x55\xc7\x9e\x2a\x7e\xc9\xce\x4a\xd2\x97\xe3\x5c\xdb"
	"\xb8\xe4\x2a\x46\x43\xa6\x0e\xef\x7c\x9a\xbe\xe2\xf5\x82\x2f\x86"
	"\xb1\xda\x24\x2d\x9c\x23\x01\xc4\x31\xfa\xcf\xd8\xac\x00\x00\x00"
	"\x00\xf9\xbe\xb4\xd9\x62\x6c\x6f\x63\x6b\x00\x00\x00\x00\x00\x00"
	"\x00\xd7\x00\x00\x00\x29\x18\x99\x27\x01\x00\x00\x00\xfc\x33\xf5"
	"\x96\xf8\x22\xa0\xa1\x95\x1f\xfd\xbf\x2a\x89\x7b\x09\x56\x36\xad"
	"\x87\x17\x07\xbf\x5d\x31\x62\x72\x9b\x00\x00\x00\x00\x37\x9d\xfb"
	"\x96\xa5\xea\x8c\x81\x70\x0e\xa4\xac\x6b\x97\xae\x9a\x93\x12\xb2"
	"\xd4\x30\x1a\x29\x58\x0e\x92\x4e\xe6\x76\x1a\x25\x20\xad\xc4\x66"
	"\x49\xff\xff\x00\x1d\x18\x9c\x4c\x97\x01\x01\x00\x00\x00\x01\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff"
	"\xff\xff\xff\x07\x04\xff\xff\x00\x1d\x01\x23\xff\xff\xff\xff\x01"
	"\x00\xf2\x05\x2a\x01\x00\x00\x00\x43\x41\x04\x08\xce\x27\x91\x74"
	"\xb3\x4c\x07\x7c\x7b\x20\x43\xe3\xf3\xd4\x5a\x58\x8b\x85\xef\x4c"
	"\xa4\x66\x74\x0f\x84\x8e\xad\x7f\xb4\x98\xf0\xa7\x95\xc9\x82\x55"
	"\x2f\xdf\xa4\x16\x16\xa7\xc0\x33\x3a\x26\x9d\x62\x10\x85\x88\xe2"
	"\x60\xfd\x5a\x48\xac\x8e\x4d\xbf\x49\xe2\xbc\xac\x00\x00\x00\x00"
	"\xf9\xbe\xb4\xd9\x62\x6c\x6f\x63\x6b\x00\x00\x00\x00\x00\x00\x00"
	"\xd7\x00";

int raw_packet_ethernet_ip_tcp_bc_flow3_ack_multiple_blocks_length = 1506;

// Contains a transaction
static char *raw_packet_ethernet_ip_tcp_bc_flow4_ack_tx =
        "\x00\x11\x43\xd1\x1e\xac\x00\x0f\x34\x30\x6b\x71\x08\x00\x45\x00"
        "\x01\x42\x2d\xd2\x40\x00\x73\x06\xed\x65\x5f\xb2\x3f\x22\xd5\x84"
        "\x77\x25\xfa\xb3\x20\x8d\x3f\xaa\x6a\x53\x47\x9d\x88\x7a\x50\x18"
        "\x01\x00\x4a\x20\x00\x00\xf9\xbe\xb4\xd9\x74\x78\x00\x00\x00\x00" // tx(\x74\x78)
        "\x00\x00\x00\x00\x00\x00\x02\x01\x00\x00\xc3\xcf\x7e\xce\x01\x00"
        "\x00\x00\x01\x2c\x86\xce\xd9\x14\x3b\x00\x22\xe8\xd1\x1b\x13\x31"
        "\x3b\x2e\xe1\xfd\x24\xd4\x93\xce\x4f\x45\x46\x1a\x50\x8d\x6f\x1e"
        "\xc6\x71\x53\x00\x00\x00\x00\x8b\x48\x30\x45\x02\x20\x27\xcd\xaa"
        "\x31\xbb\xbd\xd8\x2b\xf9\xda\xb5\x55\x65\xfa\xb0\x6f\xee\xe0\x56"
        "\xbd\x79\x1f\x07\xd0\x6e\x9b\x1f\xda\xc7\xab\x98\xcf\x02\x21\x00"
        "\xa3\xcf\xcd\xf7\xd2\xea\xc0\xd7\xe2\xed\xda\x0a\x9f\x82\xd6\x20"
        "\x18\x7a\x28\x4a\x8f\xab\x50\x92\x5d\x66\x1c\xfe\x1a\xb7\x47\xd9"
        "\x01\x41\x04\xf1\xd0\x60\x66\x82\x9a\xe6\x10\x2f\xc7\x68\x67\xa9"
        "\x97\xec\xa1\x8e\xe5\x8e\xe3\x9d\x12\x52\xfe\xa9\xdf\x57\x8a\xd6"
        "\x04\xa9\x0e\x60\xb3\x21\xdd\x4f\x9e\x04\x89\x18\x31\x6b\x4a\x6e"
        "\xdf\x41\xad\x8f\x1e\x77\xaa\x36\x2b\x02\x61\x46\x4b\x63\xe8\xcb"
        "\x1a\x25\x78\xff\xff\xff\xff\x02\x00\x09\x3d\x00\x00\x00\x00\x00"
        "\x19\x76\xa9\x14\x6b\x16\xb2\x31\x68\x2b\x3f\x24\x83\xfe\x69\x4e"
        "\x1a\x68\xd7\xf9\xfb\x38\xdb\xff\x88\xac\x40\x4b\x4c\x00\x00\x00"
        "\x00\x00\x19\x76\xa9\x14\xd1\x45\xbc\xcf\xed\x33\x52\xf6\x27\x37"
        "\xee\x4e\x4e\xa7\x9f\x06\x9a\x3a\xc1\x2d\x88\xac\x00\x00\x00\x00";

int raw_packet_ethernet_ip_tcp_bc_flow4_ack_tx_length = 336;

#endif
