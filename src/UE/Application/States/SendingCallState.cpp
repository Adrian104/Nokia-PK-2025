#include "SendingCallState.hpp"
#include "ConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{

SendingCallState::SendingCallState(Context &context,
                                   common::PhoneNumber from,
                                   common::PhoneNumber to)
    : BaseState(context, "SendingCallState"), m_from(from), m_to(to)
{
    context.timer.startTimer(std::chrono::seconds(60));
}

void SendingCallState::handleUnknownRecipient()
{
    context.logger.logInfo("Unknown Recipient");
    context.timer.stopTimer();
    context.user.showUnknownRecipient(m_to);
    context.setState<ConnectedState>();
}

void SendingCallState::handleCallDropped(common::PhoneNumber from)
{
    context.logger.logInfo("CallRequest dropped");
    context.timer.stopTimer();
    context.setState<ConnectedState>();
}

void SendingCallState::handleCallDrop(common::PhoneNumber from, common::PhoneNumber to)
{
    context.logger.logInfo("Dropping CallRequest from ", (int)from.value);
    context.bts.sendCallDrop(m_to, m_from);
    context.timer.stopTimer();
    context.setState<ConnectedState>();
}

void SendingCallState::handleCallAccepted()
{
    context.logger.logInfo("CallRequest accepted by ", (int)m_to.value);
    context.timer.stopTimer();
    context.setState<TalkingState>(m_to, m_from);
}

void SendingCallState::handleTimeout()
{
    context.logger.logInfo("CallRequest timed out");
    context.bts.sendCallDrop(m_to, m_from);
    context.setState<ConnectedState>();
}

void SendingCallState::handleCallRequest(common::MessageId msgId,
                                         common::PhoneNumber from,
                                         common::PhoneNumber to,
                                         const std::string &enc)
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(m_to, m_from);
    context.timer.startTimer(std::chrono::seconds(30));
    context.setState<ConnectedState>();
    context.user.showIncomingCall(from, to);
}

}
