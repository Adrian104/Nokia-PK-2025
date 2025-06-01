#include "Application.hpp"
#include "States/NotConnectedState.hpp"

namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer)
    : context{iLogger, bts, user, timer}, logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>();
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
}

void Application::handleSib(common::BtsId btsId)
{
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleDisconnect()
{
    context.state->handleDisconnect();
}

void Application::handleIncomingSMS(common::MessageId msgId,
                                    const common::PhoneNumber &peer,
                                    const std::string &text)
{
    context.state->handleIncomingSMS(msgId, peer, text);
}

void Application::handleCallRequest(common::MessageId msgId,
                                    const common::PhoneNumber &peer,
                                    const std::string &enc)
{
    context.state->handleCallRequest(msgId, peer, enc);
}

void Application::handleViewSmsList()
{
    context.state->handleViewSmsList();
}

void Application::handleViewSms(SmsRecord &sms)
{
    context.state->handleViewSms(sms);
}

void Application::handleSendSms(const common::PhoneNumber &peer, const std::string &text)
{
    context.state->handleSendSms(peer, text);
}

void Application::handleSmsResponse(bool status)
{
    context.state->handleSmsResponse(status);
}

void Application::handleCallDrop()
{
    context.state->handleCallDrop();
}

void Application::handleCallAccept()
{
    context.state->handleCallAccept();
}

void Application::handleUnknownRecipient()
{
    context.state->handleUnknownRecipient();
}

void Application::handleCallTalk(const common::PhoneNumber& sender, const std::string &message)
{
    context.state->handleCallTalk(sender, message);
}

void Application::handleSendCallTalk(const std::string &message)
{
    context.state->handleSendCallTalk(message);
}

void Application::handleSendCallRequest(const common::PhoneNumber &peer)
{
    context.state->handleSendCallRequest(peer);
}

void Application::handleCallDropped()
{
    context.state->handleCallDropped();
}

void Application::handleCallAccepted()
{
    context.state->handleCallAccepted();
}

IUeGui::AcceptClose Application::handleUEClose()
{
    logger.logInfo("Application close request");
    return context.state->handleUEClose();
}
}
