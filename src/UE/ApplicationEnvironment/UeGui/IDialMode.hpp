#pragma once

#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IUeGui::IDialMode
{
public:
    virtual ~IDialMode() = default;
    virtual common::PhoneNumber getPhoneNumber() const = 0;
};

}
