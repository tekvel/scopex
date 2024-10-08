/*
 * ScopeX
 * Copyright (C) 2024 Tekvel R&D Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <pcap.h>
#include <arpa/inet.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
#include <utility>

#include "sv_subs_factory.h"

#define ETHER_ADDR_LEN 6

/* Ethernet header */
struct ethernet_header
{
    u_int8_t ether_dhost[ETHER_ADDR_LEN]; // Destination host address
    u_int8_t ether_shost[ETHER_ADDR_LEN]; // Source host address
    u_int16_t ether_type;                 // Ethernet type
    u_int16_t APPID;                      // APPID
    u_int16_t Length;                     // Length
    u_int16_t res1;                       // Reserved 1
    u_int16_t res2;                       // Reserved 2
};
/* Tagged Ethernet header */
struct tag_ethernet_header
{
    u_char ether_dhost[ETHER_ADDR_LEN]; // Destination host address 
    u_char ether_shost[ETHER_ADDR_LEN]; // Source host address 
    u_short tpid;                       // Tag Protocol ID (0x8100)
    u_short tci;                        // Tag Control Information
    u_short ether_type;                 // Ethernet type 
    u_int16_t APPID;                      // APPID
    u_int16_t Length;                     // Length
    u_int16_t res1;                       // Reserved 1
    u_int16_t res2;                       // Reserved 2
};

// Network interface class
class NIF
{
public:
    NIF() : device_list(std::make_shared<std::vector<std::string>>()), devs(nullptr), noIrrelevantFrames(0), isCapturing(false) {}
    ~NIF();

    std::shared_ptr<std::vector<std::string>> get_device_list();
    bool select_device(int id);
    std::string get_current_device();
    int start_capture(char *filter_exp, std::string callback);
    void stop_capture();

    int noIrrelevantFrames;
    bool isCapturing;

private:
    std::shared_ptr<std::vector<std::string>> device_list;
    pcap_if_t *devs;
    pcap_if_t *current_device = nullptr;

    char errbuf[PCAP_ERRBUF_SIZE]; // Error buffer
    struct bpf_program fp;         // The compiled filter
    bpf_u_int32 netp;              // network address
    bpf_u_int32 maskp;             // network mask
    pcap_t *handle;                // Session handle
    struct pcap_pkthdr header;     // The header that pcap gives us
    const u_char *packet;          // The actual packet
};

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
void parse_sv_streams(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
void process_sv_data(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

#endif // NETWORK_INTERFACE_H
