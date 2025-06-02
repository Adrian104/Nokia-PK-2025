#pragma once

#include "BaseState.hpp"

namespace ue
{

class ViewingSmsState : public BaseState
{
public:
    ViewingSmsState(Context &context, const SmsRecord &sms);
    void handleDisconnect() override;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &text) override;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &enc) override;
    IUeGui::AcceptClose handleUEClose() override;
    void handleCallDrop() override;

private:
    const SmsRecord sms; // The SMS to be viewed, set only in constructor
};

}