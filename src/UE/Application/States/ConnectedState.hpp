#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context &context,
                   bool smsSent = false,
                   const common::PhoneNumber &peer = {0},
                   const std::string &text = "");
    void handleDisconnect() override;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &text) override;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &enc) override;
    void handleViewSmsList() override;
    void handleViewSms(SmsRecord &sms) override;
    void handleSendSms(const common::PhoneNumber &peer, const std::string &text) override;
    void handleSmsResponse(bool status) override;
    void handleSendCallRequest(const common::PhoneNumber &peer) override;
    IUeGui::AcceptClose handleUEClose() override;
    void handleUnknownRecipient() override;

private:
    const bool
        smsSent; // This is used to determine if we are in the state
                 // after sending sms or not, so we can handle unknown recipient correctly

    const common::PhoneNumber peer; // This should only be set from constructor
                                    // (after sending sms from SendingSmsState)

    const std::string text; // This is used to set the sms text in the compose mode
                            // when we are in the state after sending sms and there was unknown
                            // recipient message
};

}
