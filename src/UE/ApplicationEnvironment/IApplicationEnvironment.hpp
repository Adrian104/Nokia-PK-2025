#pragma once
#include "ITransport.hpp"
#include "IUeGui.hpp"
#include "Logger/ILogger.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

using ILogger=common::ILogger;

class IApplicationEnvironment
{
public:
    virtual ~IApplicationEnvironment() {}

    virtual IUeGui& getUeGui() = 0;
    virtual ITransport & getTransportToBts() = 0;
    virtual common::ILogger& getLogger() = 0;

    virtual void startMessageLoop() = 0;

    virtual common::PhoneNumber getMyPhoneNumber() const = 0;

    virtual std::int32_t getProperty(std::string const& name, std::int32_t defaultValue) const = 0;
};

}
