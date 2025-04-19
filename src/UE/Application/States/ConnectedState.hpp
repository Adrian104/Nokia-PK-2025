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
                           common::PhoneNumber from,
                           common::PhoneNumber to,
                           const std::string& text) override;
    void handleViewSmsList() override;
    void handleViewSms(SmsRecord& sms) override;
    void handleSendSms(const common::PhoneNumber& from, const common::PhoneNumber& to, const std::string& text) override;
};

}
