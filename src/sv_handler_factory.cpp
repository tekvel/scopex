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
        auto it = wxGetApp().sv_sub.sv_list->begin();
        std::advance(it, sv_index);

        if (it != wxGetApp().sv_sub.sv_list->end())
        {
            auto DatSet = it->DatSet;
            auto F = it->F;

            handlers[sv_index] = std::make_shared<SVHandler>(F, DatSet);

            std::cout << "Handler is created!" << std::endl;
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

size_t SVHandlerFactory::GetNumberOfHandlers()
{
    return handlers.size();
}

std::vector<long> SVHandlerFactory::GetListOfSVIndices()
{
    std::vector<long> keys;

    for (auto &kv : handlers)
    {
        // std::cout << kv.first << std::endl;
        keys.push_back(kv.first);
    }
    return keys;
}
