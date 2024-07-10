#ifndef SV_HANDLER_FACTORY_H
#define SV_HANDLER_FACTORY_H

#include <memory>
#include <unordered_map>

#include "sv_handler.h"

class SVHandlerFactory
{
public:
    SVHandlerFactory();
    ~SVHandlerFactory();

    void CreateHandler(long sv_index);
    std::shared_ptr<SVHandler> GetSVHandler(long sv_index);
    void DeleteHandlers();
    
private:
    std::unordered_map<long, std::shared_ptr<SVHandler>> handlers;
};

#endif
