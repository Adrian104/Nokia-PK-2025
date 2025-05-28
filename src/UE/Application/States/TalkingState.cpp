#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, common::PhoneNumber from, common::PhoneNumber to)
    : BaseState(context, "TalkingState"), m_from(from), m_to(to)
{
    context.user.showTalk(from, to);
    context.timer.startTimer(std::chrono::seconds(120));
}

void TalkingState::handleUnknownRecipient()
{
    context.setState<ConnectedState>();
    context.user.showUnknownRecipient(m_from);
}

void TalkingState::handleCallDrop(common::PhoneNumber from, common::PhoneNumber to)
{
    context.logger.logInfo("TalkingState::handleCallDrop");
    context.bts.sendCallDrop(from, to);
    context.setState<ConnectedState>();
}

void TalkingState::handleCallDropped(common::PhoneNumber from)
{
    if (from == m_from)
        context.setState<ConnectedState>();
}

void TalkingState::handleCallTalk(common::MessageId msgId, common::PhoneNumber from, common::PhoneNumber to, const std::string &message)
{
    context.user.addCallMessage(from, to, message);
    context.timer.startTimer(std::chrono::seconds(120));
}

void TalkingState::handleSendCallTalk(common::PhoneNumber from, common::PhoneNumber to, const std::string &message)
{
    context.bts.sendCallTalk(from, to, message);
    context.timer.startTimer(std::chrono::seconds(120));
}

void TalkingState::handleTimeout()
{
    context.bts.sendCallDrop(m_from, m_to);
    context.setState<ConnectedState>();
    context.user.showConnected();
}

void TalkingState::handleCallRequest(common::MessageId msgId,
                                     common::PhoneNumber from,
                                     common::PhoneNumber to,
                                     const std::string &enc)
{}

void TalkingState::handleIncomingSMS(common::MessageId msgId,
                                       common::PhoneNumber from,
                                       common::PhoneNumber to,
                                       const std::string& text)
{
    std::string log = std::string("Received message from ")
    + std::to_string(from.value) + std::string(", content: ") + text;

    logger.logInfo(log);
    context.smsdb.addReceivedSms(from, to, text); // Received SMS(from,text) stored in SMS DB (postcondition 1)
    context.user.showNewMessageIndicator(); // User is informed new SMS arrived (postcondition 2)
}
}
