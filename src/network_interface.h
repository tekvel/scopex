#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <pcap.h>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

class nif
{
public:
    nif() : device_list(std::make_shared<std::vector<std::string>>()), devs(nullptr) {}
    ~nif()
    {
        if (devs)
        {
            pcap_freealldevs(devs);
            devs = nullptr;
        }
    }

    std::shared_ptr<std::vector<std::string>> get_device_list();
    bool select_device(int id); // Now returns bool to indicate success or failure

private:
    std::shared_ptr<std::vector<std::string>> device_list;
    pcap_if_t *devs;
    pcap_if_t *current_device = nullptr;

    char errbuf[PCAP_ERRBUF_SIZE]; // Error buffer
    bpf_u_int32 netp;              // network address
    bpf_u_int32 maskp;             // network mask
    pcap_t *handle;                // Session handle
    struct pcap_pkthdr header;     // The header that pcap gives us
    const u_char *packet;          // The actual packet
};

// #define ETHER_ADDR_LEN 6
// /* Ethernet header */
// struct ethernet_header
// {
//     u_int8_t ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
//     u_int8_t ether_shost[ETHER_ADDR_LEN]; /* Source host address */
//     u_int16_t ether_type;                 /* Ethernet type */
// };
// /* Tagged Ethernet header */
// struct tag_ethernet_header
// {
//     u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
//     u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
//     u_short tpid;                       /* Tag Protocol ID (0x8100)*/
//     u_short tci;                        /* Tag Control Information*/
//     u_short ether_type;                 /* Ethernet type */
// };

#endif // NETWORK_INTERFACE_H
