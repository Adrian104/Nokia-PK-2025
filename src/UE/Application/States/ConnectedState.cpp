#include "ConnectedState.hpp"

#include "NotConnectedState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
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

}
