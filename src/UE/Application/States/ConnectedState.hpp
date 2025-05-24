#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleTimeout() override;
    void handleDisconnect() override;
    void handleIncomingSMS(common::MessageId msgId,
                           common::PhoneNumber from,
                           common::PhoneNumber to,
                           const std::string& text) override;
    void handleCallRequest(common::MessageId msgId,
                           common::PhoneNumber from,
                           common::PhoneNumber to,
                           const std::string& enc) override;
    void handleViewSmsList() override;
    void handleViewSms(SmsRecord& sms) override;
    void handleSendSms(const common::PhoneNumber& from, const common::PhoneNumber& to, const std::string& text) override;
    void handleSmsResponse(bool status) override;
    void handleCallDrop(common::PhoneNumber from, common::PhoneNumber to) override;
    void handleCallAccept(common::PhoneNumber from, common::PhoneNumber to) override;
    void handleUnknownRecipient() override;
    void handleSendCallRequest(common::PhoneNumber from, common::PhoneNumber to) override;
    void handleCallDropped() override;

private:
    common::PhoneNumber m_callingNumber;
    common::PhoneNumber m_myNumber;
    bool m_ringing = false;
};

}
