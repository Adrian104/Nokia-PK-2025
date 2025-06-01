#pragma once

#include "BaseState.hpp"

namespace ue
{

class SendingCallState : public BaseState
{
public:
    SendingCallState(Context &context, const common::PhoneNumber& peer);

    void handleUnknownRecipient() override;
    void handleCallDropped() override;
    void handleCallAccepted() override;
    void handleTimeout() override;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber& peer,
                           const std::string &enc) override;
    void handleCallDrop() override;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &text) override;
    IUeGui::AcceptClose handleUEClose() override;
    void handleDisconnect() override;

private:
    const common::PhoneNumber peer;
};

}
