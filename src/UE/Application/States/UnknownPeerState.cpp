#include "UnknownPeerState.hpp"

#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ReceivingCallState.hpp"

namespace ue
{

UnknownPeerState::UnknownPeerState(
    Context &context,
    const common::PhoneNumber &peer,
    const std::function<void(Context &unknownContext)> &exitStateCallback)
    : BaseState(context, "UnknownPeerState"), peer(peer), exitStateCallback(exitStateCallback)
{
    context.user.showUnknownRecipient(peer);
    context.timer.startTimer(TIMEOUT_DURATION);
}

void UnknownPeerState::handleTimeout()
{
    context.logger.logInfo("Timeout reached");
    exitState();
}
void UnknownPeerState::handleIncomingSMS(common::MessageId msgId,
                                         const common::PhoneNumber &peer,
                                         const std::string &text)
{
    logger.logInfo("Received message from ", peer.value, ", content: ", text);
    const auto number_of_this_UE = context.bts.getMyPhoneNumber();
    context.smsdb.addReceivedSms(peer, number_of_this_UE, text);
    context.user.showNewMessageIndicator();
}
void UnknownPeerState::handleCallRequest(common::MessageId msgId,
                                         const common::PhoneNumber &peer,
                                         const std::string &enc)
{
    context.setState<ReceivingCallState>(peer);
}

IUeGui::AcceptClose UnknownPeerState::handleUEClose()
{
    context.timer.stopTimer();
    return true;
}

void UnknownPeerState::handleCallDrop()
{
    context.timer.stopTimer();
    context.logger.logInfo("User pressed red button");
    exitState();
}

void UnknownPeerState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

void UnknownPeerState::exitState()
{
    if (!runExitStateCallback())
    {
        context.timer.stopTimer();
        context.logger.logInfo(
            "No exitStateCallback provided, going back to ConnectedState by default");
        context.setState<ConnectedState>();
    }
}

bool UnknownPeerState::runExitStateCallback()
{
    if (exitStateCallback != nullptr)
    {
        exitStateCallback(context);
        return true;
    }
    return false;
}

}