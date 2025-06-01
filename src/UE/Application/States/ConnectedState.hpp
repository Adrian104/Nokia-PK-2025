#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleDisconnect() override;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber& peer,
                           const std::string& text) override;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber& peer,
                           const std::string& enc) override;
    void handleViewSmsList() override;
    void handleViewSms(SmsRecord& sms) override;
    void handleSendSms(const common::PhoneNumber& peer, const std::string& text) override;
    void handleSmsResponse(bool status) override;
    void handleSendCallRequest(const common::PhoneNumber &peer) override;
    IUeGui::AcceptClose handleUEClose() override;
};

}
