#include "network_interface.h"

NIF::~NIF()
{
    if (devs)
    {
        pcap_freealldevs(devs);
        pcap_freecode(&fp);
        pcap_close(handle);
        devs = nullptr;
    }
}

std::shared_ptr<std::vector<std::string>> NIF::get_device_list()
{
    // Find a capture device
    if (pcap_findalldevs(&devs, errbuf) == -1)
    {
        std::cerr << "Error in pcap_findalldevs: " << errbuf << std::endl;
        return nullptr;
    }

    device_list->clear(); // Clear existing list before refilling

    for (pcap_if_t *temp = devs; temp; temp = temp->next)
    {
        device_list->push_back(temp->name);
    }

    return device_list;
}

bool NIF::select_device(int id)
{
    if (id < 0 || !devs || id >= device_list->size())
    {
        std::cerr << "Invalid device ID or device list is empty." << std::endl;
        return false;
    }

    int i = 0;

    for (pcap_if_t *device = devs; device != NULL; device = device->next)
    {
        if (i++ == id)
        {
            current_device = device;
            break;
        }
    }

    if (!current_device)
    {
        std::cerr << "Device not found." << std::endl;
        return false;
    }
    // Get network number and mask associated with capture device
    if (pcap_lookupnet(current_device->name, &netp, &maskp, errbuf) == -1)
    {
        std::cerr << "Error in pcap_lookupnet: " << errbuf << std::endl;
        return false;
    }
    // Open capture device
    handle = pcap_open_live(current_device->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr)
    {
        std::cerr << "Couldn't open device: " << errbuf << std::endl;
        return false;
    }

    std::cout << "Device successfully opened" << std::endl;
    return true;
}

std::string NIF::get_current_device()
{
    if (current_device)
    {
        return current_device->name;
    }
    else
    {
        return "";
    }
}

void NIF::sniff_traffic(int n_packets, char *filter_exp)
{
    // Compile the filter expression
    if (pcap_compile(handle, &fp, filter_exp, 0, netp) == -1)
    {
        std::cerr << "Couldn't parse filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        return;
    }
    // Apply the compiled filter
    if (pcap_setfilter(handle, &fp) == -1)
    {
        std::cerr << "Couldn't install filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        return;
    }

    // Start capturing packets
    pcap_loop(handle, n_packets, got_packet, NULL);
}

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    std::cout << "Length of packet: " << header->len << std::endl;
    std::cout << "Timestamp, sec: " << header->ts.tv_sec << std::endl;
    std::cout << "Timestamp, usec: " << header->ts.tv_usec << std::endl;

    auto *eth = reinterpret_cast<const ethernet_header *>(packet);

    // std::cout << "Destination MAC: "
    //           << std::hex << std::setw(2) << std::setfill('0')
    //           << (int)eth->ether_dhost[0] << ":" << (int)eth->ether_dhost[1] << ":"
    //           << (int)eth->ether_dhost[2] << ":" << (int)eth->ether_dhost[3] << ":"
    //           << (int)eth->ether_dhost[4] << ":" << (int)eth->ether_dhost[5] << std::endl;

    // std::cout << "Source MAC: "
    //           << std::hex << std::setw(2) << std::setfill('0')
    //           << (int)eth->ether_shost[0] << ":" << (int)eth->ether_shost[1] << ":"
    //           << (int)eth->ether_shost[2] << ":" << (int)eth->ether_shost[3] << ":"
    //           << (int)eth->ether_shost[4] << ":" << (int)eth->ether_shost[5] << std::endl;

    // Check if the frame is tagged
    if (ntohs(eth->ether_type) == 0x8100)
    {
        std::cout << "Frame is tagged" << std::endl;
        auto *tag_eth = reinterpret_cast<const tag_ethernet_header *>(packet);
        std::cout << "VLAN Identifier: 0x" << std::hex << (ntohs(tag_eth->tci) & 0x0FFF) << std::endl;
        std::cout << "Priority code point: 0x" << std::hex << ((ntohs(tag_eth->tci) >> 13) & 0x0007) << std::endl;
        std::cout << "EtherType: 0x" << std::hex << ntohs(tag_eth->ether_type) << std::endl;
    }
    else
    {
        std::cout << "Frame is not tagged" << std::endl;
        std::cout << "EtherType: 0x" << std::hex << ntohs(eth->ether_type) << std::endl;
    }
}
