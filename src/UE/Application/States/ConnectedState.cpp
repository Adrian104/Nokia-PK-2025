#include "ConnectedState.hpp"

#include "NotConnectedState.hpp"
#include "SendingCallState.hpp"
#include "TalkingState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
}

void ConnectedState::handleTimeout()
{
    m_ringing = false;
    context.bts.sendCallDrop(m_callingNumber, m_myNumber);
    context.user.showConnected();
}

void ConnectedState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::handleIncomingSMS(common::MessageId msgId,
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

void ConnectedState::handleCallRequest(common::MessageId msgId,
                       common::PhoneNumber from,
                       common::PhoneNumber to,
                       const std::string& enc)
{
    m_myNumber = to;
    m_callingNumber = from;
    m_ringing = true;

    context.user.showIncomingCall(from, to);
    context.timer.startTimer(std::chrono::seconds(30));
}

void ConnectedState::handleViewSmsList()
{
    logger.logInfo("View message list");
    context.user.showSmsList(context.smsdb);
}

void ConnectedState::handleViewSms(SmsRecord& sms)
{
    std::string log = std::string("View message from ") + std::to_string(sms.m_from.value) 
    + std::string(", to: ") + std::to_string(sms.m_to.value)
    + std::string(", message: ") + sms.m_message;
    logger.logInfo(log);
    context.user.showSms(sms);  
}

void ConnectedState::handleSendSms(const common::PhoneNumber& from, const common::PhoneNumber& to, const std::string& text)
{
    context.logger.logInfo("Sending SMS to ", (int) to.value, ", SMS content: ", text);

    context.smsdb.addSms(from, to, text);

    if (!context.bts.sendSms(from, to, text))
    {
        context.smsdb.markLastSmsSentAsFailed();
        context.logger.logInfo("Failed to send SMS to ", (int) to.value);
    }
    else
    {
        context.user.showConnected();
        context.logger.logInfo("SMS sent successfully to BTS");
    }
}

void ConnectedState::handleSmsResponse(bool status)
{
    if (!status)
    {
        context.smsdb.markLastSmsSentAsFailed();
        context.logger.logError("SMS delivery failed - unknown recipient");
    }
}

void ConnectedState::handleCallDrop(common::PhoneNumber from, common::PhoneNumber to)
{
    context.logger.logInfo("Dropping CallRequest from ", (int)from.value);
    m_ringing = false;
    context.timer.stopTimer();
    context.bts.sendCallDrop(from, to);
}

void ConnectedState::handleCallAccept(common::PhoneNumber from, common::PhoneNumber to)
{
    context.logger.logInfo("Accepting CallRequest from ", (int)from.value);
    m_ringing = false;
    context.timer.stopTimer();
    context.bts.sendCallAccept(from, to);
    context.setState<TalkingState>(from, to);
}

void ConnectedState::handleUnknownRecipient()
{
    handleSmsResponse(!m_ringing);
}

void ConnectedState::handleSendCallRequest(common::PhoneNumber from, common::PhoneNumber to)
{
    context.logger.logInfo("Sending CallRequest to ", (int)to.value);
    context.user.showDialling(from, to);
    context.bts.sendCallRequest(from, to);
    context.setState<SendingCallState>(from, to);
}

void ConnectedState::handleCallDropped(common::PhoneNumber from)
{
    context.logger.logInfo("CallRequest dropped");
    m_ringing = false;
    context.timer.stopTimer();
    context.user.showConnected();
}

}