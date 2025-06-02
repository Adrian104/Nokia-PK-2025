#include "ConnectedState.hpp"

#include "NotConnectedState.hpp"
#include "ReceivingCallState.hpp"
#include "SendingCallState.hpp"
#include "SendingSmsState.hpp"
#include "UnknownPeerState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context,
                               bool smsSent,
                               const common::PhoneNumber &peer,
                               const std::string &text)
    : BaseState(context, "ConnectedState"), smsSent(smsSent), peer(peer), text(text)
{
    context.user.showConnected();
}

void ConnectedState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::handleIncomingSMS(common::MessageId msgId,
                                       const common::PhoneNumber &peer,
                                       const std::string &text)
{
    logger.logInfo("Received message from ", std::to_string(peer.value), ", content: ", text);
    const auto number_of_this_UE = context.bts.getMyPhoneNumber();

    context.smsdb.addReceivedSms(
        peer,
        number_of_this_UE,
        text); // Received SMS(from,text) stored in SMS DB (postcondition 1)
    context.user
        .showNewMessageIndicator(); // User is informed new SMS arrived (postcondition 2)
}

void ConnectedState::handleCallRequest(common::MessageId msgId,
                                       const common::PhoneNumber &peer,
                                       const std::string &enc)
{
    context.setState<ReceivingCallState>(peer);
}

void ConnectedState::handleViewSmsList()
{
    logger.logInfo("View message list");
    context.user.showSmsList(context.smsdb);
}

void ConnectedState::handleViewSms(SmsRecord &sms)
{
    std::string log = std::string("View message from ") + std::to_string(sms.m_from.value) +
                      std::string(", to: ") + std::to_string(sms.m_to.value) +
                      std::string(", message: ") + sms.m_message;
    logger.logInfo(log);
    context.user.showSms(sms);
}

void ConnectedState::handleSendSms(const common::PhoneNumber &peer, const std::string &text)
{
    // Peer and text are dummy default values, when called from UserPort showConnected screen
    // we reuse this method to change state to SendingSmsState
    context.setState<SendingSmsState>();
}

void ConnectedState::handleSmsResponse(bool status)
{
    if (!status && smsSent)
    {
        context.smsdb.markLastSmsSentAsFailed();
        context.logger.logError("SMS delivery failed - unknown recipient");
        context.setState<UnknownPeerState>(peer,
                                           [this](Context &unknownContext)
                                           { unknownContext.setState<ConnectedState>(); });
    }
}

void ConnectedState::handleSendCallRequest(const common::PhoneNumber &peer)
{
    if (peer == context.bts.getMyPhoneNumber())
    {
        context.logger.logError("Cannot dial to self");
        context.setState<UnknownPeerState>(peer,
                                           [](Context &unknownContext)
                                           {
                                               unknownContext.setState<ConnectedState>();
                                               // since dialing screen isn't a specific state
                                               // we have to call this method
                                               // after changing state to ConnectedState (which
                                               // by default opens main menu) If we did it
                                               // before changing state, then it would not
                                               // work, because the ConnectedState state's
                                               // constructor would "override" this by opening
                                               // main menu screen
                                               unknownContext.user.showDialMode();
                                           });
    }
    else
    {
        context.setState<SendingCallState>(peer);
    }
}
IUeGui::AcceptClose ConnectedState::handleUEClose()
{
    return true;
}

void ConnectedState::handleUnknownRecipient()
{
    if (smsSent)
    {
        context.smsdb.markLastSmsSentAsFailed();
        auto textCopy = this->text;
        context.setState<UnknownPeerState>(
            peer,
            [textCopy](Context &unknownContext)
            { unknownContext.setState<SendingSmsState>(textCopy); });
    }
}

}