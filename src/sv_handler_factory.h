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

#ifndef SV_HANDLER_FACTORY_H
#define SV_HANDLER_FACTORY_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "sv_handler.h"

class SVHandlerFactory
{
public:
    SVHandlerFactory();
    ~SVHandlerFactory();

    void CreateHandler(long sv_index);
    std::shared_ptr<SVHandler> GetSVHandler(long sv_index);
    size_t GetNumberOfHandlers();
    std::vector<long> GetListOfSVIndices();
    void DeleteHandlers();
    
private:
    std::unordered_map<long, std::shared_ptr<SVHandler>> handlers;
};

#endif
