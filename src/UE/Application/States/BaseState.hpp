#pragma once

#include "Context.hpp"
#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context &context, const std::string &name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnect() override = 0;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &text) override = 0;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &enc) override = 0;
    void handleViewSmsList() override;
    void handleViewSms(SmsRecord &sms) override;
    void handleSendSms(const common::PhoneNumber &peer, const std::string &text) override;
    void handleSmsResponse(bool status) override;
    void handleCallDrop() override;
    void handleCallAccept() override;
    void handleUnknownRecipient() override;
    void handleCallTalk(const common::PhoneNumber& sender, const std::string &message) override;
    void handleSendCallTalk(const std::string &message) override;
    void handleSendCallRequest(const common::PhoneNumber &peer) override;
    void handleCallDropped() override;
    void handleCallAccepted() override;
    IUeGui::AcceptClose handleUEClose() override = 0;

protected:
    Context &context;
    common::PrefixedLogger logger;
};

}
