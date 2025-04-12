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

    context.smsdb.addReceivedSms(from, to, text);
    context.user.showNewMessageIndicator();
}
}
