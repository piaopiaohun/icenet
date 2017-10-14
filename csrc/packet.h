#ifndef __ICENET_PACKET_H__
#define __ICENET_PACKET_H__

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define NET_IP_ALIGN 2
#define ETH_MAX_WORDS 190
#define ETH_MAX_BYTES 1518
#define MAC_ADDR_BYTES 6
#define BCAST_MAC 0xffffffffffffL

struct network_packet {
	uint64_t data[ETH_MAX_WORDS];
	int len;
	int idx;
};

static inline void network_packet_init(struct network_packet *packet, int len)
{
    packet->len = len;
    packet->idx = 0;
    memset(packet->data, 0, ETH_MAX_WORDS * sizeof(uint64_t));
}

static inline void network_packet_add(network_packet *packet, uint64_t data)
{
    packet->data[packet->idx] = data;
    packet->idx++;
}

static inline uint64_t network_packet_dstmac(network_packet *packet)
{
    uint64_t dstmac = 0;

    memcpy(&dstmac, ((char *) packet->data) + NET_IP_ALIGN, MAC_ADDR_BYTES);
    return dstmac;
}

static inline network_packet *network_packet_copy(network_packet *packet)
{
    network_packet *packet_copy = new network_packet;
    packet_copy->len = packet->len;
    memcpy(packet_copy->data, packet->data, packet->len * sizeof(uint64_t));
    return packet_copy;
}

static inline bool network_packet_complete(network_packet *packet)
{
	return packet->idx == packet->len;
}

static inline uint64_t random_macaddr(void)
{
    uint64_t macaddr;

    // Generate random MAC according to
    // https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/5/html/Virtualization/sect-Virtualization-Tips_and_tricks-Generating_a_new_unique_MAC_address.html
    srandom(time(0));
    macaddr = random() & 0xffff7f;
    macaddr <<= 24;
    macaddr |= 0x3e1600;

    return macaddr;
}

#endif
