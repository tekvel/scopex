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

#include "network_interface.h"
#include "main.h"

NIF::~NIF()
{
    if (devs)
    {
        pcap_freealldevs(devs);
        pcap_freecode(&fp);
        if (handle != NULL)
        {
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
        // return false;
    }
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

int NIF::start_capture(char *filter_exp, std::string callback)
{
    // Open capture device
    handle = pcap_open_live(current_device->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr)
    {
        std::cerr << "Couldn't open device: " << errbuf << std::endl;
        return -1;
    }
    // Set non-blocking mode
    if (pcap_setnonblock(handle, 1, errbuf) == -1) {
        std::cerr << "Couldn't set non-blocking mode:" << errbuf << std::endl;
        return -1;
    }
    std::cout << "Device successfully opened" << std::endl;

    // Compile the filter expression
    if (pcap_compile(handle, &fp, filter_exp, 0, netp) == -1)
    {
        std::cerr << "Couldn't parse filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        return -1;
    }
    // Apply the compiled filter
    if (pcap_setfilter(handle, &fp) == -1)
    {
        std::cerr << "Couldn't install filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        return -1;
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
    else if (callback == "process_sv_data")
    {
        callback_fn = process_sv_data;
    }
    else
    {
        std::cerr << "Couldn't determine callback function" << std::endl;
        return -1;
    }
    
    // Start the capture loop and handle errors
    isCapturing = true;
    int result = pcap_loop(handle, 0, callback_fn, nullptr);
    if (result == PCAP_ERROR_BREAK)
    {
        std::cout << "pcap_loop was manually interrupted" << std::endl;
    }
    else if (result < 0)
    {
        std::cerr << "Error occurred: " << pcap_geterr(handle) << std::endl;
        return -1;
    }

    // Close the session
    pcap_close(handle);
    handle = nullptr;
    return 0;
}

void NIF::stop_capture()
{
    if (isCapturing)
    {
        isCapturing = false;
        pcap_breakloop(handle);
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
    u_int16_t smpCnt;
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
        wxGetApp().sv_sub.max_smpCnt->insert({reinterpret_cast<u_int64_t>(&(*val.first)), smpCnt});
    }
    else
    {
        auto it = wxGetApp().sv_sub.max_smpCnt->find(reinterpret_cast<u_int64_t>(&(*val.first)));
        if (it != wxGetApp().sv_sub.max_smpCnt->end()) 
        {
            if (it->second < smpCnt)
            {
                it->second = smpCnt;
            }
        }
    }
}

void process_sv_data(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
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

    u_int16_t smpCnt;

    int offset = 0; // Initial offset for SV PDU parsing

    if (sv_data[offset] == 0x60)
    {
        u_char svPDU_len = sv_data[offset+1];
        if (svPDU_len > 0x80)
        {
            if (svPDU_len == 0x81)
            {
                offset += 3;    // Move to value of svPDU or tag of noASDU
            }
            else if (svPDU_len == 0x82)
            {
                offset += 4;    // Move to value of svPDU or tag of noASDU
            }
        }
        else
        {
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
                        offset += 3;    // Move to tag of ASDU
                    }
                    else if (seqASDU_len == 0x82)
                    {
                        offset += 4;    // Move to tag of ASDU
                    }
                }
                else
                {
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
                                            std::vector<u_int32_t> seqData(seqData_len/4);
                                            memcpy(seqData.data(), (sv_data + offset + i*ASDU_len + 19 + svID_len), seqData_len/4 * sizeof(u_int32_t));
                                            // 
                                            if (wxGetApp().sv_sub.find_sv(stream))
                                            {
                                                auto sv_handler_ptr = wxGetApp().sv_handler.GetSVHandler(*wxGetApp().sv_sub.selectedSV_id_main);
                                                auto it = wxGetApp().sv_sub.sv_list->begin();
                                                std::advance(it, *wxGetApp().sv_sub.selectedSV_id_main);

                                                long prev_smpCnt = static_cast<long>(sv_handler_ptr->prev_smpCnt);
                                                long max_smpCnt = static_cast<long>(it->F);
                                                auto smpCnt_diff = prev_smpCnt - static_cast<long>(smpCnt);

                                                if (smpCnt_diff > (max_smpCnt - 10))
                                                {   
                                                    std::cout << "Time in sec: " << header->ts.tv_sec << std::endl;
                                                    std::cout << "Time in nsec: " << header->ts.tv_usec << std::endl;

                                                    if (sv_handler_ptr->operating_list == 0)
                                                    {
                                                        sv_handler_ptr->operating_list = 1;
                                                    }
                                                    else
                                                    {
                                                        sv_handler_ptr->operating_list = 0;
                                                    }

                                                    wxGetApp().start = std::chrono::steady_clock::now();
                                                    
                                                    SVProcessThread *thread = new SVProcessThread;
                                                    if (thread->Create() != wxTHREAD_NO_ERROR)
                                                    {
                                                        std::cerr << "Can't create SVHandler 2 thread!" << std::endl;
                                                        return;
                                                    }
                                                    wxCriticalSectionLocker enter(wxGetApp().m_critsect);
                                                    wxGetApp().m_threads.Add(thread);
                                                    if (thread->Run() != wxTHREAD_NO_ERROR)
                                                    {
                                                        std::cerr << "Can't start SVHandler 2 thread!" << std::endl;
                                                        return;
                                                    }

                                                    sv_handler_ptr->reference_ts.first = header->ts.tv_sec;
                                                    sv_handler_ptr->reference_ts.second = header->ts.tv_usec;
                                                    sv_handler_ptr->reference_smpCnt = smpCnt;
                                                }
                                                else
                                                {
                                                    if (sv_handler_ptr->reference_ts.first == 0 && sv_handler_ptr->reference_ts.second == 0)
                                                    {
                                                        sv_handler_ptr->reference_ts.first = header->ts.tv_sec;
                                                        sv_handler_ptr->reference_ts.second = header->ts.tv_usec;
                                                        sv_handler_ptr->reference_smpCnt = smpCnt;
                                                    }
                                                    else
                                                    {
                                                        if (smpCnt != sv_handler_ptr->prev_smpCnt + 1)
                                                        {
                                                            auto time_diff = header->ts.tv_sec - sv_handler_ptr->reference_ts.first + header->ts.tv_usec/1000000.0 - sv_handler_ptr->reference_ts.second/1000000.0;
                                                            if (time_diff > 1)
                                                            {
                                                                sv_handler_ptr->reference_ts.first = header->ts.tv_sec;
                                                                sv_handler_ptr->reference_ts.second = header->ts.tv_usec;
                                                                sv_handler_ptr->reference_smpCnt = smpCnt;
                                                            }
                                                        }
                                                    }
                                                }
                                                sv_handler_ptr->prev_smpCnt = smpCnt;
                                                sv_handler_ptr->SV_data_raw[sv_handler_ptr->operating_list][smpCnt] = std::make_pair(smpCnt, seqData);
                                            }
                                            else
                                            {
                                                NIF nif;
                                                nif.noIrrelevantFrames += 1;
                                            }
                                            //
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
}