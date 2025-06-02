#include "SendingSmsState.hpp"

#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ReceivingCallState.hpp"
#include "UnknownPeerState.hpp"

namespace ue
{

SendingSmsState::SendingSmsState(Context &context, const std::string &text) : BaseState(context, "SendingSmsState")
{
    context.user.showSmsComposeMode(text);
}

void SendingSmsState::handleCallRequest(common::MessageId msgId,
                                        const common::PhoneNumber &peer,
                                        const std::string &enc)
{
    context.setState<ReceivingCallState>(peer);
}

void SendingSmsState::handleIncomingSMS(common::MessageId msgId,
                                        const common::PhoneNumber &peer,
                                        const std::string &text)
{
    logger.logInfo("Received message from ", peer.value, ", content: ", text);
    const auto number_of_this_UE = context.bts.getMyPhoneNumber();
    context.smsdb.addReceivedSms(
        peer,
        number_of_this_UE,
        text);
    context.user
        .showNewMessageIndicator();
}

IUeGui::AcceptClose SendingSmsState::handleUEClose()
{
    return true;
}

void SendingSmsState::handleSendSms(const common::PhoneNumber &peer, const std::string &text)
{
    const auto number_of_this_UE = context.bts.getMyPhoneNumber();
    this->peer = peer;
    context.smsdb.addSms(number_of_this_UE, peer, text);
    if (!context.bts.sendSms(peer, text))
    {
        context.smsdb.markLastSmsSentAsFailed();
        context.logger.logInfo("Failed to send SMS to ", static_cast<int>(peer.value));
        context.setState<UnknownPeerState>(peer,
                                           [this, text](Context &unknownContext)
                                           { unknownContext.setState<SendingSmsState>(text);});
    }
    else
    {
        context.logger.logInfo("SMS sent successfully to BTS");
        context.setState<ConnectedState>(true, peer, text);
    }
}

void SendingSmsState::handleSmsResponse(bool status)
{
    if (!status)
    {
        context.smsdb.markLastSmsSentAsFailed();
        context.logger.logError("SMS delivery failed - unknown recipient");
    }
}

void SendingSmsState::handleCallDrop()
{
    context.setState<ConnectedState>();
}

void SendingSmsState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

}
