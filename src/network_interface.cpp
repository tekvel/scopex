#include "network_interface.h"

std::shared_ptr<std::vector<std::string>> nif::get_device_list()
{
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

bool nif::select_device(int id)
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

    if (pcap_lookupnet(current_device->name, &netp, &maskp, errbuf) == -1)
    {
        std::cerr << "Error in pcap_lookupnet: " << errbuf << std::endl;
        return false;
    }

    handle = pcap_open_live(current_device->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr)
    {
        std::cerr << "Couldn't open device: " << errbuf << std::endl;
        return false;
    }

    std::cout << "Device successfully opened" << std::endl;
    return true;
}