#include "network_interface.h"
#include "main.h"

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

void NIF::sniff_traffic(int n_packets, char *filter_exp, int timeout_ms)
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

    auto start = std::chrono::steady_clock::now();
    int packet_count = 0;
    // Start capturing packets with timeout
    while (packet_count < n_packets)
    {
        int result = pcap_dispatch(handle, n_packets - packet_count, parse_sv_streams, nullptr);

        if (result > 0)
        {
            packet_count += result;
        }
        else if (result == 0)
        {

            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

            std::cout << "Elapsed time: " << elapsed << " ms" << std::endl;

            if (elapsed >= timeout_ms)
            {
                std::cerr << "Timeout: No packets captured within the specified timeout period of " << timeout_ms << " ms." << std::endl;
                return;
            }

            // Busy-wait loop to simulate delay of 20 ms
            auto wait_start = std::chrono::steady_clock::now();
            while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - wait_start).count() < 20)
            {
                // Busy-wait
            }
        }
        else
        {
            std::cerr << "Error in pcap_dispatch: " << pcap_geterr(handle) << std::endl;
            return;
        }
    }
}

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    std::cout << "Length of packet: " << std::dec << header->len << std::endl;
    std::cout << "Timestamp, sec: " << std::dec << header->ts.tv_sec << std::endl;
    std::cout << "Timestamp, usec: " << std::dec << header->ts.tv_usec << std::endl;

    auto *eth = reinterpret_cast<const ethernet_header *>(packet);

    std::cout << "Destination MAC: "
              << std::hex << std::setw(2) << std::setfill('0')
              << (int)eth->ether_dhost[0] << ":" << (int)eth->ether_dhost[1] << ":"
              << (int)eth->ether_dhost[2] << ":" << (int)eth->ether_dhost[3] << ":"
              << (int)eth->ether_dhost[4] << ":" << (int)eth->ether_dhost[5] << std::endl;

    std::cout << "Source MAC: "
              << std::hex << std::setw(2) << std::setfill('0')
              << (int)eth->ether_shost[0] << ":" << (int)eth->ether_shost[1] << ":"
              << (int)eth->ether_shost[2] << ":" << (int)eth->ether_shost[3] << ":"
              << (int)eth->ether_shost[4] << ":" << (int)eth->ether_shost[5] << std::endl;

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

void parse_sv_streams(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    auto *eth = reinterpret_cast<const ethernet_header *>(packet);

    SV_stream stream;

    std::copy(std::begin(eth->ether_dhost), std::end(eth->ether_dhost), std::begin(stream.ether_dhost));
    std::copy(std::begin(eth->ether_shost), std::end(eth->ether_shost), std::begin(stream.ether_shost));

    if (ntohs(eth->ether_type) == 0x8100)
    {
        stream.tagged = true;
        auto *tag_eth = reinterpret_cast<const tag_ethernet_header *>(packet);
        stream.ether_type = ntohs(tag_eth->ether_type);
    }
    else
    {
        stream.tagged = false;
        stream.ether_type = ntohs(eth->ether_type);
    }

    wxGetApp().sv_sub.sv_list->insert(stream);
}