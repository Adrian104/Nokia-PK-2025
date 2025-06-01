#include "SendingCallState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ReceivingCallState.hpp"
#include "TalkingState.hpp"
#include "UnknownPeerState.hpp"

namespace ue
{

SendingCallState::SendingCallState(Context &context, const common::PhoneNumber &peer)
    : BaseState(context, "SendingCallState"), peer(peer)
{
    context.logger.logInfo("Sending CallRequest to ", static_cast<int>(peer.value));
    context.user.showDialling(peer);
    context.bts.sendCallRequest(peer);
    context.timer.startTimer(std::chrono::seconds(60));
}

void SendingCallState::handleUnknownRecipient()
{
    context.logger.logInfo("Unknown Recipient");
    context.setState<UnknownPeerState>(peer,
                                       [](Context &unknownContext)
                                       {
                                           unknownContext.setState<ConnectedState>();
                                           // since dialing screen isn't a specific state we
                                           // have to call this method after changing state to
                                           // ConnectedState (which by default opens main menu)
                                           // If we did it before changing state, then it would
                                           // not work, because the ConnectedState state's
                                           // constructor would "override" this by opening main
                                           // menu screen
                                           unknownContext.user.showDialMode();
                                       });
}

void SendingCallState::handleCallDropped()
{
    context.timer.stopTimer();
    context.logger.logInfo("CallRequest dropped");
    context.setState<ConnectedState>();
}

void SendingCallState::handleCallDrop()
{
    context.timer.stopTimer();
    context.logger.logInfo("Dropping CallRequest to ", static_cast<int>(peer.value));
    context.bts.sendCallDrop(peer);
    context.setState<ConnectedState>();
}

void SendingCallState::handleCallAccepted()
{
    context.timer.stopTimer();
    context.logger.logInfo("CallRequest accepted by ", static_cast<int>(peer.value));
    context.setState<TalkingState>(peer);
}

void SendingCallState::handleTimeout()
{
    context.logger.logInfo("CallRequest timed out");
    context.bts.sendCallDrop(peer);
    context.setState<ConnectedState>();
}

void SendingCallState::handleCallRequest(common::MessageId msgId,
                                         const common::PhoneNumber &peer,
                                         const std::string &enc)
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(this->peer);
    context.setState<ReceivingCallState>(peer);
}

void SendingCallState::handleIncomingSMS(common::MessageId msgId,
                                         const common::PhoneNumber &peer,
                                         const std::string &text)
{
    logger.logInfo("Received message from ", peer.value, ", content: ", text);
    const auto number_of_this_UE = context.bts.getMyPhoneNumber();
    context.smsdb.addReceivedSms(
        peer,
        number_of_this_UE,
        text); // Received SMS(from,text) stored in SMS DB (postcondition 1)
    context.user
        .showNewMessageIndicator(); // User is informed new SMS arrived (postcondition 2)
}
IUeGui::AcceptClose SendingCallState::handleUEClose()
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(peer);
    return true;
}

void SendingCallState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

}
