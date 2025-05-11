#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    TalkingState(Context& context, common::PhoneNumber from, common::PhoneNumber to);
    void handleUnknownRecipient() override;
    void handleCallDrop(common::PhoneNumber from, common::PhoneNumber to) override;
    void handleCallAccept(common::PhoneNumber from, common::PhoneNumber to) override;

private:
    common::PhoneNumber m_from;
    common::PhoneNumber m_to;
};

}
