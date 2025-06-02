#pragma once

#include "BaseState.hpp"

namespace ue
{

class ViewingSmsListState : public BaseState
{
public:
    ViewingSmsListState(Context &context);
    void handleDisconnect() override;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &text) override;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &enc) override;
    IUeGui::AcceptClose handleUEClose() override;
    void handleCallDrop() override;
    void handleViewSms(SmsRecord &sms) override;
};

}