#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
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
    void handleCallTalk(common::MessageId msgId, common::PhoneNumber from, common::PhoneNumber to, const std::string & message) override;
    void handleSendCallTalk(common::PhoneNumber from, common::PhoneNumber to, const std::string &message) override;
    void handleSendCallRequest(common::PhoneNumber from, common::PhoneNumber to) override;
    void handleCallDropped() override;
    void handleCallAccepted() override;

protected:
    Context& context;
    common::PrefixedLogger logger;
};

}
