#include "ReceivingCallState.hpp"

#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{
ReceivingCallState::ReceivingCallState(Context &context, const common::PhoneNumber &peer)
    : BaseState(context, "ReceivingCallState"), peer(peer)
{
    context.user.showIncomingCall(peer);
    context.timer.startTimer(timeoutDuration);
}
void ReceivingCallState::handleTimeout()
{
    context.logger.logInfo("Timeout reached");
    dropAndGoToMainMenu();
}

void ReceivingCallState::handleIncomingSMS(common::MessageId msgId,
                                           const common::PhoneNumber &peer,
                                           const std::string &text)
{
    logger.logInfo("Received message from ", peer.value, ", content: ", text);
    const auto number_of_this_UE = context.bts.getMyPhoneNumber();
    context.smsdb.addReceivedSms(peer, number_of_this_UE, text);
    context.user.showNewMessageIndicator();
}

void ReceivingCallState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

void ReceivingCallState::handleCallRequest(common::MessageId msgId,
                                           const common::PhoneNumber &peer,
                                           const std::string &enc)
{
    if (peer.value != this->peer.value)
    {
        logger.logInfo("Dropping new call request from ",
                       static_cast<int>(peer.value),
                       "when already receiving one call from ",
                       static_cast<int>(this->peer.value));
        context.bts.sendCallDrop(peer);
    }
}

void ReceivingCallState::handleCallDrop()
{
    context.logger.logInfo("User pressed red button");
    dropAndGoToMainMenu();
}

void ReceivingCallState::handleCallAccept()
{
    context.timer.stopTimer();
    context.logger.logInfo("User pressed green button");
    context.setState<TalkingState>(peer);
}
void ReceivingCallState::handleCallDropped()
{
    dropAndGoToMainMenu(false);
}

IUeGui::AcceptClose ReceivingCallState::handleUEClose()
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(peer);
    return true;
}

void ReceivingCallState::dropAndGoToMainMenu(const bool send_drop_msg) const
{
    context.timer.stopTimer();
    context.logger.logInfo("Dropping call from ", static_cast<int>(peer.value));
    if (send_drop_msg)
    {
        context.bts.sendCallDrop(peer);
    }
    context.setState<ConnectedState>();
}
}