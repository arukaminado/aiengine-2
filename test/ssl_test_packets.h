#ifndef _ssl_test_packets_H_
#define _ssl_test_packets_H_

// Client hello from a tor connection
static unsigned char raw_ethernet_ip_tcp_port_8080_ssl_client_hello_heartbeat[309] = {
	0x24, 0x76, 0x7d, 0x39, 0xb8, 0x71, 0x8c, 0xa9, /* $v}9.q.. */
	0x82, 0x49, 0x43, 0x5c, 0x08, 0x00, 0x45, 0x00, /* .IC\..E. */
	0x01, 0x27, 0x01, 0x92, 0x40, 0x00, 0x40, 0x06, /* .'..@.@. */
	0x3e, 0x81, 0xc0, 0xa8, 0x01, 0x11, 0x25, 0xdc, /* >.....%. */
	0x12, 0x29, 0xbf, 0x8a, 0x1f, 0x90, 0x27, 0x22, /* .)....'" */
	0x6a, 0x0c, 0xb4, 0x22, 0xd3, 0xf2, 0x80, 0x18, /* j..".... */
	0x00, 0xe5, 0x9d, 0x9a, 0x00, 0x00, 0x01, 0x01, /* ........ */
	0x08, 0x0a, 0x00, 0xa4, 0x6b, 0xc7, 0x60, 0xa0, /* ....k.`. */
0xd3, 0x2b, 0x16, 0x03, 0x01, 0x00, 0xee, 0x01, /* .+...... */
0x00, 0x00, 0xea, 0x03, 0x03, 0x28, 0xd6, 0x93, /* .....(.. */
0x04, 0xae, 0x4f, 0x61, 0x8e, 0x89, 0x56, 0x36, /* ..Oa..V6 */
0xfe, 0x1c, 0x43, 0xe8, 0x2b, 0x77, 0x72, 0x71, /* ..C.+wrq */
0xc0, 0xde, 0x34, 0x23, 0x74, 0x48, 0x51, 0x21, /* ..4#tHQ! */
0x8c, 0xbc, 0x9b, 0x26, 0xd4, 0x00, 0x00, 0x30, /* ...&...0 */
0xc0, 0x2b, 0xc0, 0x2f, 0xc0, 0x0a, 0xc0, 0x09, /* .+./.... */
0xc0, 0x13, 0xc0, 0x14, 0xc0, 0x12, 0xc0, 0x07, /* ........ */
0xc0, 0x11, 0x00, 0x33, 0x00, 0x32, 0x00, 0x45, /* ...3.2.E */
0x00, 0x39, 0x00, 0x38, 0x00, 0x88, 0x00, 0x16, /* .9.8.... */
0x00, 0x2f, 0x00, 0x41, 0x00, 0x35, 0x00, 0x84, /* ./.A.5.. */
0x00, 0x0a, 0x00, 0x05, 0x00, 0x04, 0x00, 0xff, /* ........ */
0x01, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x20, /* .......  */
0x00, 0x1e, 0x00, 0x00, 0x1b, 0x77, 0x77, 0x77, /* .....www */
0x2e, 0x67, 0x69, 0x37, 0x6e, 0x33, 0x35, 0x61, /* .gi7n35a */
0x62, 0x6a, 0x36, 0x64, 0x65, 0x68, 0x6a, 0x6c, /* bj6dehjl */
0x67, 0x35, 0x67, 0x37, 0x2e, 0x63, 0x6f, 0x6d, /* g5g7.com */
0x00, 0x0b, 0x00, 0x04, 0x03, 0x00, 0x01, 0x02, /* ........ */
0x00, 0x0a, 0x00, 0x34, 0x00, 0x32, 0x00, 0x0e, /* ...4.2.. */
0x00, 0x0d, 0x00, 0x19, 0x00, 0x0b, 0x00, 0x0c, /* ........ */
0x00, 0x18, 0x00, 0x09, 0x00, 0x0a, 0x00, 0x16, /* ........ */
0x00, 0x17, 0x00, 0x08, 0x00, 0x06, 0x00, 0x07, /* ........ */
0x00, 0x14, 0x00, 0x15, 0x00, 0x04, 0x00, 0x05, /* ........ */
0x00, 0x12, 0x00, 0x13, 0x00, 0x01, 0x00, 0x02, /* ........ */
0x00, 0x03, 0x00, 0x0f, 0x00, 0x10, 0x00, 0x11, /* ........ */
0x00, 0x23, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x20, /* .#.....  */
	0x00, 0x1e, 0x06, 0x01, 0x06, 0x02, 0x06, 0x03, /* ........ */
	0x05, 0x01, 0x05, 0x02, 0x05, 0x03, 0x04, 0x01, /* ........ */
	0x04, 0x02, 0x04, 0x03, 0x03, 0x01, 0x03, 0x02, /* ........ */
	0x03, 0x03, 0x02, 0x01, 0x02, 0x02, 0x02, 0x03, /* ........ */
	0x00, 0x0f, 0x00, 0x01, 0x01                    /* ..... */
};

static int raw_ethernet_ip_tcp_port_8080_ssl_client_hello_heartbeat_length = 309;

#endif
