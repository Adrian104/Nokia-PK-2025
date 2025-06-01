#pragma once

#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IUeGui::ISmsComposeMode
{
public:
    virtual ~ISmsComposeMode() = default;

    virtual common::PhoneNumber getPhoneNumber() const = 0;
    virtual std::string getSmsText() const = 0;
    virtual void clearSmsText()= 0;
};

}
