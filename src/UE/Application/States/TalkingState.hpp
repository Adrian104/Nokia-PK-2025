#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    TalkingState(Context &context, const common::PhoneNumber &peer);
    void handleUnknownRecipient() override;
    void handleCallDrop() override;
    void handleCallDropped() override;
    void handleCallTalk(const common::PhoneNumber& sender, const std::string &message) override;
    void handleSendCallTalk(const std::string &message) override;
    void handleTimeout() override;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &enc) override;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &text) override;
    IUeGui::AcceptClose handleUEClose() override;
    void handleDisconnect() override;

private:
    common::PhoneNumber peer;
    const std::chrono::minutes TALKING_TIMEOUT{2};
};

}
