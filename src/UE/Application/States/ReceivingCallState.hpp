#pragma once

#include "BaseState.hpp"
namespace ue
{

class ReceivingCallState : public BaseState
{
public:
    ReceivingCallState(Context &context, const common::PhoneNumber& peer);
    void handleTimeout() override;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber& peer,
                           const std::string &enc) override;
    void handleCallDrop() override;
    void handleCallAccept() override;
    void handleCallDropped() override;
    IUeGui::AcceptClose handleUEClose() override;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &text) override;
    void handleDisconnect() override;

private:
    const ITimerPort::Duration timeoutDuration = std::chrono::seconds(30);
    common::PhoneNumber peer;

    void dropAndGoToMainMenu(const bool send_drop_msg = true) const;
};

}
