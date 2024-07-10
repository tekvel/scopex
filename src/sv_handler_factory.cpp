#include "sv_handler_factory.h"
#include "main.h"

// SVHandlerFactory class
SVHandlerFactory::SVHandlerFactory()
{
}

SVHandlerFactory::~SVHandlerFactory()
{
    DeleteHandlers();
}

void SVHandlerFactory::CreateHandler(long sv_index)
{
    if (handlers.find(sv_index) == handlers.end())
    {
        auto id = wxGetApp().sv_sub.selectedSV_ids->at(sv_index);
        auto it = wxGetApp().sv_sub.sv_list->begin();
        std::advance(it, id);

        if (it != wxGetApp().sv_sub.sv_list->end())
        {
            auto DatSet = it->DatSet;
            auto F = it->F;

            handlers[sv_index] = std::make_shared<SVHandler>(F, DatSet);
        }
    }
}

std::shared_ptr<SVHandler> SVHandlerFactory::GetSVHandler(long sv_index)
{
    if (handlers.find(sv_index) != handlers.end())
    {
        return handlers[sv_index];
    }
    return nullptr;
}

void SVHandlerFactory::DeleteHandlers()
{
    handlers.clear();
}
