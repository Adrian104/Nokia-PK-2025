#pragma once

#include "BaseState.hpp"

namespace ue
{

class SendingSmsState : public BaseState
{
public:
    SendingSmsState(Context &context, const std::string &text = "");
    void handleDisconnect() override;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &text) override;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &enc) override;
    IUeGui::AcceptClose handleUEClose() override;
    void handleSendSms(const common::PhoneNumber &peer, const std::string &text) override;
    void handleSmsResponse(bool status) override;
    void handleCallDrop() override;


private:
    common::PhoneNumber peer; // This should be only set right before sending sms to bts and
                              // passed to ConnectedState so it can handle unknown recipient
};

}
