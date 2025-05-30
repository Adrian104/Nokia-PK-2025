#pragma once

#include "BaseState.hpp"

namespace ue
{

class SendingCallState : public BaseState
{
public:
    SendingCallState(Context &context, common::PhoneNumber from, common::PhoneNumber to);
    void handleUnknownRecipient() override;
    void handleCallDropped(common::PhoneNumber from) override;
    void handleCallAccepted() override;
    void handleTimeout() override;
    void handleCallRequest(common::MessageId msgId,
                           common::PhoneNumber from,
                           common::PhoneNumber to,
                           const std::string &enc) override;
    void handleCallDrop(common::PhoneNumber from, common::PhoneNumber to) override;
    void handleIncomingSMS(common::MessageId msgId,
                           common::PhoneNumber from,
                           common::PhoneNumber to,
                           const std::string& text) override;

private:
    common::PhoneNumber m_from;
    common::PhoneNumber m_to;
};

}
