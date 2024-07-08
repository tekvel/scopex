#include "network_interface.h"
#include "main.h"

NIF::~NIF()
{
    if (devs)
    {
        pcap_freealldevs(devs);
        pcap_freecode(&fp);
        if (handle != NULL){
            pcap_close(handle);
        }
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
    handle = pcap_open_live(current_device->name, BUFSIZ, 1, 2, errbuf);
    if (handle == nullptr)
    {
        std::cerr << "Couldn't open device: " << errbuf << std::endl;
        return false;
    }
    // Set non-blocking mode
    if (pcap_setnonblock(handle, 1, errbuf) == -1) {
        std::cerr << "Couldn't set non-blocking mode:" << errbuf << std::endl;
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

void NIF::sniff_traffic(int n_packets, char *filter_exp, std::string callback, int timeout_ms)
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
    
    // Choose callback function
    pcap_handler callback_fn;
    if (callback == "parse_sv_streams")
    {
        callback_fn = parse_sv_streams;
    }
    else if (callback == "got_packet")
    {
        callback_fn = got_packet;
    }
    else
    {
        std::cerr << "Couldn't determine callback function" << std::endl;
        return;
    }

    auto start = std::chrono::steady_clock::now();
    int packet_count = 0;
    // Start capturing packets with timeout
    while (packet_count < n_packets)
    {
        int result = pcap_dispatch(handle, n_packets - packet_count, callback_fn, nullptr);

        if (result > 0)
        {
            packet_count += result;
        }
        else if (result == 0)
        {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

            if (elapsed >= timeout_ms)
            {
                std::cerr << "Timeout: No packets captured within the specified timeout period of " << timeout_ms << " ms." << std::endl;
                return;
            }

            // Busy-wait loop to simulate delay of 5 ms
            auto wait_start = std::chrono::steady_clock::now();
            while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - wait_start).count() < 5)
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

    const u_char *sv_data;

    if (ntohs(eth->ether_type) == 0x8100)
    {
        auto *tag_eth = reinterpret_cast<const tag_ethernet_header *>(packet);
        stream.APPID = ntohs(tag_eth->APPID);
        sv_data = packet + sizeof(tag_ethernet_header);
    }
    else
    {
        stream.APPID = ntohs(eth->APPID);
        sv_data = packet + sizeof(ethernet_header);
    }

    stream.F = 0;

    int offset = 0; // Initial offset for SV PDU parsing

    if (sv_data[offset] == 0x60)
    {
        u_char svPDU_len = sv_data[offset+1];
        if (svPDU_len > 0x80)
        {
            if (svPDU_len == 0x81)
            {
                u_char svPDU = sv_data[offset+2];
                offset += 3;    // Move to value of svPDU or tag of noASDU
            }
            else if (svPDU_len == 0x82)
            {
                u_int16_t svPDU = sv_data[offset+2];
                offset += 4;    // Move to value of svPDU or tag of noASDU
            }
        }
        else
        {
            u_char svPDU = sv_data[offset+1];
            offset += 2;    // Move to value of svPDU or tag of npASDU
        }
        
        if (sv_data[offset] == 0x80)
        {
            u_char noASDU = sv_data[offset+2];  // Number of ASDU
            stream.noASDU = noASDU;
            offset += 3;    // Move to tag of seqASDU

            if (sv_data[offset] == 0xA2)
            {
                u_char seqASDU_len = sv_data[offset+1]; // Length of seqASDU
                if (seqASDU_len > 0x80)
                {
                    if (seqASDU_len == 0x81)
                    {
                        u_char seqASDU = sv_data[offset+2];
                        offset += 3;    // Move to tag of ASDU
                    }
                    else if (seqASDU_len == 0x82)
                    {
                        u_int16_t seqASDU = sv_data[offset+2];
                        offset += 4;    // Move to tag of ASDU
                    }
                }
                else
                {
                    u_char seqASDU = sv_data[offset+1];
                    offset += 2;    // Move to tag of ASDU
                }

                u_char ASDU_len = 0;
                for (int i = 0; i < noASDU; ++i)
                {
                    if (sv_data[offset + i*ASDU_len] == 0x30)
                    {
                        ASDU_len = sv_data[offset + i*ASDU_len + 1] + 2;    // + 2 means plus one tag byte and one length byte

                        if (sv_data[offset + i*ASDU_len + 2] == 0x80)
                        {
                            u_char svID_len = sv_data[offset + i*ASDU_len + 3];

                            std::vector<u_char> svID(svID_len);
                            memcpy(svID.data(), (sv_data + offset + i*ASDU_len + 4), svID_len * sizeof(u_char));
                            stream.svID = svID;

                            if (sv_data[offset + i*ASDU_len + 4 + svID_len] == 0x82)
                            {
                                u_int16_t smpCnt;
                                memcpy(&smpCnt, (sv_data + offset + i*ASDU_len + 6 + svID_len), 2);
                                smpCnt = ntohs(smpCnt);

                                if (sv_data[offset + i*ASDU_len + 8 + svID_len] == 0x83)
                                {
                                    u_int32_t confRev;
                                    memcpy(&confRev, (sv_data + offset + i*ASDU_len + 10 + svID_len), 4);
                                    confRev = ntohl(confRev);

                                    if (sv_data[offset + i*ASDU_len + 14 + svID_len] == 0x85)
                                    {
                                        u_char smpSync = sv_data[offset + i*ASDU_len + 16 + svID_len];

                                        if (sv_data[offset + i*ASDU_len + 17 + svID_len] == 0x87)
                                        {
                                            u_char seqData_len = sv_data[offset + i*ASDU_len + 18 + svID_len];
                                            stream.DatSet = seqData_len / 8;
                                        }
                                        else
                                        {
                                            std::cerr << "Error: Expected tag 0x87 (dataset) not found" << std::endl;
                                            return;
                                        }
                                    }
                                    else
                                    {
                                        std::cerr << "Error: Expected tag 0x85 (smpSync) not found" << std::endl;
                                        return;
                                    }
                                }
                                else
                                {
                                    std::cerr << "Error: Expected tag 0x83 (confRev) not found" << std::endl;
                                    return;
                                }
                            }
                            else
                            {
                                std::cerr << "Error: Expected tag 0x82 (smpCnt) not found" << std::endl;
                                return;
                            }
                        }
                        else
                        {
                            std::cerr << "Error: Expected tag 0x80 (svID) not found" << std::endl;
                            return;
                        }
                    }
                    else
                    {   
                        std::cerr << "Number of ASDU: " << i << std::endl;
                        std::cerr << "Error: Expected tag 0x30 (ASDU) not found" << std::endl;
                        return;
                    }
                }
            }
            else
            {
                std::cerr << "Error: Expected tag 0xA2 (seqASDU) not found" << std::endl;
                return;
            }
        }
        else
        {
            std::cerr << "Error: Expected tag 0x80 (noASDU) not found" << std::endl;
            return;
        }
    }
    else
    {
        std::cerr << "Error: Expected tag 0x60 (svPDU) not found" << std::endl;
        return;
    }

    auto val = wxGetApp().sv_sub.sv_list_raw->insert(stream);

    if (val.second == true)
    {
        wxGetApp().sv_sub.sv_list_cnt->insert({reinterpret_cast<u_int64_t>(&(*val.first)), 1});
    }
    else
    {
        auto it = wxGetApp().sv_sub.sv_list_cnt->find(reinterpret_cast<u_int64_t>(&(*val.first)));
        if (it != wxGetApp().sv_sub.sv_list_cnt->end()) 
        {
            it->second++;
        }
        if (it->second%48 == 0)
        {
            auto it = wxGetApp().sv_sub.sv_list_prev_time->find(reinterpret_cast<u_int64_t>(&(*val.first)));
            if (it != wxGetApp().sv_sub.sv_list_prev_time->end()) 
            {
                double time_diff = header->ts.tv_usec - it->second;
                if (time_diff < 500000)
                {
                    u_int64_t F = wxGetApp().sv_sub.get_closer_freq(48000000.0/(time_diff)*stream.noASDU);
                    if (F != -1)
                    {
                        stream.F = F;
                        wxGetApp().sv_sub.sv_list->insert(stream);
                    }
                    else
                    {
                        std::cerr << "Error: Freqency value is not defined" << std::endl;
                    }
                }
                wxGetApp().sv_sub.sv_list_prev_time->erase(reinterpret_cast<u_int64_t>(&(*val.first)));
            }
            wxGetApp().sv_sub.sv_list_prev_time->insert({reinterpret_cast<u_int64_t>(&(*val.first)), header->ts.tv_usec});

        }
    }
}