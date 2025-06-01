#include "TalkingState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "UnknownPeerState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, const common::PhoneNumber &peer)
    : BaseState(context, "TalkingState"), peer(peer)
{
    context.logger.logInfo("Accepting call from ", static_cast<int>(this->peer.value));
    context.bts.sendCallAccept(peer);
    context.user.showTalk();
    context.timer.startTimer(TALKING_TIMEOUT);
}

void TalkingState::handleUnknownRecipient()
{
    context.setState<UnknownPeerState>(peer);
}

void TalkingState::handleCallDrop()
{
    context.timer.stopTimer();
    context.logger.logInfo("TalkingState::handleCallDrop");
    context.bts.sendCallDrop(peer);
    context.setState<ConnectedState>();
}

void TalkingState::handleCallDropped()
{
    context.setState<ConnectedState>();
}

void TalkingState::handleCallTalk(const common::PhoneNumber &sender,
                                  const std::string &message)
{
    context.timer.stopTimer();
    context.user.addCallMessage(sender, message);
    context.timer.startTimer(TALKING_TIMEOUT);
}

void TalkingState::handleSendCallTalk(const std::string &message)
{
    context.timer.stopTimer();
    context.bts.sendCallTalk(peer, message);
    context.timer.startTimer(TALKING_TIMEOUT);
}

void TalkingState::handleTimeout()
{
    context.bts.sendCallDrop(peer);
    context.setState<ConnectedState>();
}

void TalkingState::handleCallRequest(common::MessageId msgId,
                                     const common::PhoneNumber &peer,
                                     const std::string &enc)
{
    if (peer != this->peer)
    {
        logger.logInfo("Dropping new call request from ",
                       static_cast<int>(peer.value),
                       "when already talking with ",
                       static_cast<int>(this->peer.value));
        context.bts.sendCallDrop(peer);
    }
}

void TalkingState::handleIncomingSMS(common::MessageId msgId,
                                     const common::PhoneNumber &peer,
                                     const std::string &text)
{
    logger
        .logInfo("Received message from ", static_cast<int>(peer.value), ", content: ", text);
    const auto number_of_this_UE = context.bts.getMyPhoneNumber();
    context.smsdb.addReceivedSms(
        peer,
        number_of_this_UE,
        text); // Received SMS(from,text) stored in SMS DB (postcondition 1)
    context.user
        .showNewMessageIndicator(); // User is informed new SMS arrived (postcondition 2)
}
IUeGui::AcceptClose TalkingState::handleUEClose()
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(peer);
    return true;
}
void TalkingState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}
}
