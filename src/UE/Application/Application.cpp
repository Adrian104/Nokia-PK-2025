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
                                    common::PhoneNumber from,
                                    common::PhoneNumber to,
                                    const std::string& text)
{
    context.state->handleIncomingSMS(msgId, from, to, text);
}

void Application::handleViewSmsList()
{
    context.state->handleViewSmsList();
}

void Application::handleViewSms(SmsRecord& sms)
{
    context.state->handleViewSms(sms);
}

void Application::handleSendSms(const common::PhoneNumber& from, const common::PhoneNumber& to, const std::string& text)
{
    context.state->handleSendSms(from, to, text);
}

void Application::handleSmsResponse(bool status)
{
    context.state->handleSmsResponse(status);

}

}
