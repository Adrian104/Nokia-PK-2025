#pragma once
#include "Context.hpp"
#include "IEventsHandler.hpp"
#include "Logger/ILogger.hpp"

namespace ue
{

class Application : public IEventsHandler
{
public:
    Application(common::PhoneNumber phoneNumber,
                common::ILogger &iLogger,
                IBtsPort &bts,
                IUserPort &user,
                ITimerPort &timer);
    ~Application();

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
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
    void handleCallDrop() override;
    void handleCallAccept() override;
    void handleUnknownRecipient() override;
    void handleCallTalk(const common::PhoneNumber& sender, const std::string &message) override;
    void handleSendCallTalk(const std::string &message) override;
    void handleSendCallRequest(const common::PhoneNumber &peer) override;
    void handleCallDropped() override;
    void handleCallAccepted() override;
    IUeGui::AcceptClose handleUEClose() override;

private:
    Context context;
    common::PrefixedLogger logger;
};

}
